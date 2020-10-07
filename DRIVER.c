/***********************
 * DRIVER
 ***********************/
#include "SERVER.h"

#define EN		PD2
#define MS1		PD3
#define MS2		PD4
#define MS3		PD5
#define STEP 	PD6
#define DIR 	PD7
 
void DRIVER_init()
{
	DDRD |= (1<<EN)|(1<<MS1)|(1<<MS2)|(1<<MS3)|(1<<STEP)|(1<<DIR);
	PORTD |= (1<<DIR); // Вперед
	PORTD &= ~(8<<MS1);
	PORTD |= (1<<MS1);
}

void DRIVER_step()
{
	PORTD ^= (1<<STEP);
}

void DRIVER_chdir()
{
	PORTD ^= (1<<DIR);
}

void DRIVER_forward()
{
	PORTD |= (1<<DIR);
}

void DRIVER_backward()
{
	PORTD &= ~(1<<DIR);
}

void DRIVER_stepdiv(uint8_t div)
{
	// 0    1    2    3
	// 1    1/2  1/4  1/8
	// 1/16 не работает
	if(div == 0)
	{
		PORTD &= ~(1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
	}

	if(div == 1)
	{
		PORTD |= (1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
	}

	if(div == 2)
	{
		PORTD &= ~(1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
	}
	if(div == 3)
	{
		PORTD |= (1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
	}
}