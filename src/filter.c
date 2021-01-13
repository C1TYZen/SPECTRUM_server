#include "lib/system.h"
#include "lib/DRIVER.h"
#include "lib/PORTB.h"
#include "lib/PORTD.h"
#include "lib/USART.h"

#define DIR 	PD3
#define STEP 	PD4
#define MS3		PD7
#define MS2		PB0
#define MS1		PB1
#define EN		PB2

#define PWR_sens	D5
#define GND_sens	D6

#define sens_0		D11	//датчик нулевого положения
#define sens_c		D12	//датчик текущего положения

#define SRF 		D2

int pos = 0;

void step(int steps)
{
	int i = 0;
	if(steps > 0)
	{
		DRIVER_backward();
		for(i = 0; i < steps; i++)
			DRIVER_step();
	}
	if(steps < 0)
	{
		steps *= -1;
		DRIVER_forward();
		for(i = 0; i < steps; i++)
			DRIVER_step();
	}  
}
			
void zero_position()
{
	_delay_ms(1500);
	DRIVER_backward();
	if(PORTB_getpin(sens_0) == 0)
		step(1500);

	while(PORTB_getpin(sens_0) == 1)
		step(1);
		
	pos = 1;
	_delay_ms(1000);
}

void filter_position(int num)
{
	while(pos != num)
	{
		if(pos > num)
		{
			while(pos > num)
			{
				step(-800); //Чо здесь происходит 0_о
				while(PORTB_getpin(sens_c) == 1)
					step(-1);
				pos--;
			}
			step(-100); 
		}
		else
		{
			while(pos < num)
			{
				step(1000); //А здесь почему тыща шагов?
				while(PORTB_getpin(sens_c) == 1)
					step(1);
				pos++;
			}
		}
	}
}

void main()
{
	DRIVER_init();
	PORTB_init();
	PORTD_init();
	USART_init();

	//**** Какое то питание
	PORTD_pinmod(PWR_sens, PINMOD_OUT);
	PORTD_pinmod(GND_sens, PINMOD_OUT);
	PORTD_writepin(PWR_sens, 1);
	PORTD_writepin(GND_sens, 0);
	//****

	PORTD_pinmod(sens_0, 0);
	PORTD_pinmod(sens_c, 0);

	PORTD_pinmod(SRF, 0);

	zero_position();

	uint8_t SRFS = 1;

	while(1)
	{
		SRFS = PORTD_getpin(SRF);

		if(SRFS == 1)
		{
			_delay_ms(2);
			SRFS = 0;
			int i = 1;
			for(; i <= 6; i++)
			{
				SRFS += PORTD_getpin(SRF);
				_delay_ms(50);
			}

			// char s[10];
			// sprintf(s, "%d", SRFS);
			// USART_println(s);
			
			filter_position(SRFS);
			SRFS = 0;
			_delay_ms(200); //Чтобы последний бит не вызывал еще одну итерацию
		}
	}
}
