#include "lib/system.h"

#define PWR_sens	5       
#define GND_sens	6 

#define sens_0		D8	//датчик нулевого положения
#define sens_c		D9	//датчик текущего положения

#define SRF 	2

int comand_reg = 1;
int SRFS;

volatile unsigned int num = 0;
volatile unsigned int pos = 0;

const int num_max = 6;
const int num_min = 1;

void step(int steps)
{
	int i = 0;
	if(steps > 0)
	{
		DRIVER_backward();
		for(i = 0; i < steps; i++)
		{
			DRIVER_step();
		}
	}
	else
	{
		steps = abs(steps);
		DRIVER_forward();
		for(i = 0; i < steps; i++)
		{
			DRIVER_step();
		}
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
				while((pos > num) && (digitalRead(sens_c) == 1))
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
				while((pos < num) && (digitalRead(sens_c) == 1))
				{
					step(1);
				}
				pos++;
			}
		}
	}
}

void main() {
	DRIVER_init();
	USART_init();

	/*
	pinMode(PWR_sens, 1);
	pinMode(GND_sens, 1);
	digitalWrite(PWR_sens, 1);
	digitalWrite(GND_sens, 0);

	pinMode(sens_0, 0);
	pinMode(sens_c, 0);
	pinMode(SRF, 0);
	*/
	
	zero_position();

	while(1)
	{
		SRFS = PORTB_getpin(SRF);

		//считывание номера позиции
		if(SRFS == 0)
		{
			int k = 0;
			_delay_ms(1);
			comand_reg = 0;
			for(k = 1; k <= 6 ; k++)
			{
				_delay_ms(2);
				SRFS = PORTB_getpin(SRF);
				if(SRFS == 1)
				{
					comand_reg++;
				}
			}

			//ожидание конца передачи
			SRFS = PORTB_getpin(SRF);
			while(SRFS == 0)
			{
				SRFS = PORTB_getpin(SRF);
			}
			
			digitalWrite(SRF,0);
			pinMode(SRF, 1); 
			num = comand_reg;
			filter_position();
			pinMode(SRF, 0);
		}
	}
}
