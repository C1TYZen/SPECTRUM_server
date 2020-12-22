#include "lib/system.h"
#include "lib/DRIVER.h"
#include "lib/PORTB.h"
#include "lib/PORTD.h"
#include "lib/USART.h"

#define PWR_sens	D8       
#define GND_sens	D9 

#define sens_0		D10	//датчик нулевого положения
#define sens_c		D11	//датчик текущего положения

#define SRF 		D2

volatile unsigned int num = 0;
volatile unsigned int pos = 0;

void step(int steps)
{
	int i = 0;
	if(steps > 0)
	{
		DRIVER_backward();
		for(i = 0; i < steps; i++)
			DRIVER_step();
	}
	else
	{
		steps = abs(steps);
		DRIVER_forward();
		for(i = 0; i < steps; i++)
			DRIVER_step();
	}  
}
			
void zero_position()
{
	DRIVER_backward();
	_delay_ms(1500);
	if(PORTB_getpin(sens_0) == 0)
	{
		step(1500);
	}
	while(PORTB_getpin(sens_0) == 1)
	{
		step(1);
	}
	num = 1;   
	pos = 1;
	_delay_ms(1000);
}

void filter_position()
{
	while(pos != num)
	{
		if(pos > num)
		{
			while(pos > num)
			{
				step(-800);
				while((pos > num) && (PORTB_getpin(sens_c) == 1))
				{
					step(-1);
				}
				pos--;
			}
			step(-100); 
		}
		else
		{
			while(pos < num)
			{
				step(1000);
				while((pos < num) && (PORTB_getpin(sens_c) == 1))
				{
					step(1);
				}
				pos++;
			}
		}
	}
}

void main() {
	driver_config drv_cfg = 
	{
		.EN = 	PD2,
		.MS1 = 	PD3,
		.MS2 = 	PD4,
		.MS3 = 	PD5,
		.STEP = PD6,
		.DIR = 	PD7
	};

	DRIVER_init();
	//USART_init();
	PORTB_init();
	PORTD_init();

	PORTB_pinmod(PWR_sens, 1);
	PORTB_pinmod(GND_sens, 1);
	PORTB_writepin(PWR_sens, 1);
	PORTB_writepin(GND_sens, 0);

	PORTB_pinmod(sens_0, 0);
	PORTB_pinmod(sens_c, 0);
	PORTB_pinmod(SRF, 0);

	zero_position();

	int comand_reg = 1;
	int SRFS;
	int k = 0;

	while(1)
	{
		SRFS = PORTB_getpin(SRF);

		//считывание номера позиции
		if(SRFS == 0)
		{
			_delay_ms(1);
			comand_reg = 0;
			for(k = 1; k <= 6 ; k++)
			{
				_delay_ms(2);
				SRFS = PORTB_getpin(SRF);
				if(SRFS == 1)
					comand_reg++;
			}

			//ожидание конца передачи
			SRFS = PORTB_getpin(SRF);
			while(SRFS == 0)
				SRFS = PORTB_getpin(SRF);
			
			PORTB_writepin(SRF,0);
			PORTB_pinmod(SRF, 1); 
			num = comand_reg;
			filter_position();
			PORTB_pinmod(SRF, 0);
		}
	}
}
