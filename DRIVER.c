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

long DRIVER_position;
int DRIVER_dir = 1;

 /*
  * Инициализация библиотеки двигателя
  */
void DRIVER_init()
{
	DDRD |= (1<<EN)|(1<<MS1)|(1<<MS2)|(1<<MS3)|(1<<STEP)|(1<<DIR);
	PORTD |= (1<<DIR); // Вперед
	PORTD &= ~(8<<MS1);
	PORTD |= (1<<MS1);
}

// Шаг двигателя
void DRIVER_step()
{
	PORTD ^= (1<<STEP);
	DRIVER_position += DRIVER_dir;
}

// Направление двигателя вперед
void DRIVER_forward()
{
	PORTD |= (1<<DIR);
	DRIVER_dir = 1;
}

// Направление двигателя назад
void DRIVER_backward()
{
	PORTD &= ~(1<<DIR);
	DRIVER_dir = -1;
}

// Установка делителя шага
void DRIVER_stepdiv(uint8_t div)
{
	// 1    2    4    8
	// 1    1/2  1/4  1/8
	// 1/16 не работает
	if(div == 1)
	{
		PORTD &= ~(1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
	}

	if(div == 2)
	{
		PORTD |= (1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
	}

	if(div == 4)
	{
		PORTD &= ~(1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
	}
	if(div == 8)
	{
		PORTD |= (1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
	}
}

void DRIVER_moveto(long r1)
{
	long st;
	if(DRIVER_position < r1)
	{
		DRIVER_backward();
		st = r1 - DRIVER_position;
	}
	else
	{
		DRIVER_forward();
		st = DRIVER_position - r1;
	}
	while(st > 0)
	{
		DRIVER_step();
		st--;
		_delay_ms(1.4);
	}
}

void DRIVER_info()
{
	USART_send(DRIVER_position, 3);
}

void DRIVER_reset()
{
	DRIVER_position = 0;
}