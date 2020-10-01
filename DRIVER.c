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
	//0    1    2    3    8
	//1    1/2  1/4  1/8  1/16
	PORTD |= (1<<MS1);

}

void DRIVER_step()
{
	PORTD ^= (1<<STEP);
}

void DRIVER_move(long steps)
{
	uint16_t message = 0;
	long step_temp = 3;
	while (step_temp >= 0) {
		message = USART_getmessage();
		if(message == 26980) // di
			break;
		USART_sendoncomp((uint16_t)15);
		DRIVER_step();
		step_temp--;
		_delay_ms(2);
	}
	USART_sendoncomp(28019);
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

	/*if((div == 1)||(div == 2)||(div == 3)) {
		PORTD &= ~(8<<MS1);
		PORTD |= (div<<MS1);
	} else {
		PORTD &= ~(8<<MS1);
	}*/
	// 1/16 шага не работает
}