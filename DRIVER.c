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

#define CLOCKWISE 	| (1<<DIR)
#define CCLOCKWISE 	& ~(1<<DIR)

uint16_t DRIVER_position = 0;
int8_t DRIVER_dir = 1;
uint8_t DRIVER_div = 1;

/**
 * Инициализация библиотеки двигателя
 **/
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
	DRIVER_dir = -1;
}

// Направление двигателя назад
void DRIVER_backward()
{
	PORTD &= ~(1<<DIR);
	DRIVER_dir = 1;
}

int8_t DRIVER_setdir(int8_t dir)
{
	if(dir == 1) //back
	{
		PORTD &= ~(1<<DIR);
		DRIVER_dir = 1;
		return 1;
	}
	else if(dir == -1) //fwd
	{
		PORTD |= (1<<DIR);
		DRIVER_dir = -1;
		return 0;
	}
	else
	{
		PORTD &= ~(1<<DIR);
		DRIVER_dir = 1;
		return -1;
	}
}

// Установка делителя шага
int8_t DRIVER_setdiv(uint8_t div)
{
	// 1    2    4    8
	// 1    1/2  1/4  1/8
	// 1/16 не работает
	if(div == 1)
	{
		PORTD &= ~(1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
		DRIVER_div = 1;
	}
	else if(div == 2)
	{
		PORTD |= (1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
		DRIVER_div = 2;
	}
	else if(div == 4)
	{
		PORTD &= ~(1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
		DRIVER_div = 4;
	}
	else if(div == 8)
	{
		PORTD |= (1<<MS1);
		PORTD |= (1<<MS2);
		PORTD &= ~(1<<MS3);
		DRIVER_div = 8;
	}
	else
	{
		PORTD &= ~(1<<MS1);
		PORTD &= ~(1<<MS2);
		PORTD &= ~(1<<MS3);
		DRIVER_div = 1;
		div = 1;
	}
	return div;
}

void DRIVER_moveto(uint16_t start)
{
	int8_t dir;
	uint16_t steps = 0;

	if(DRIVER_position < start)
	{
		dir = 1;
		DRIVER_setdir(dir);
		steps = start - DRIVER_position;
	}
	else if(DRIVER_position > start)
	{
		dir = -1;
		DRIVER_setdir(dir);
		steps = DRIVER_position - start;
	}
	/*else
	{
		return;
	}*/

	while(DRIVER_position != start)
	{
		DRIVER_setdiv(1);
		/*if(DRIVER_position < 1)
			DRIVER_setdiv(2, 0);
		if(DRIVER_position < 0.5)
			DRIVER_setdiv(4, 0);
		if(DRIVER_position < 0.25)
			DRIVER_setdiv(8, 0);*/
		
		DRIVER_step();
		_delay_ms(1.4);
		steps--;
	}

	/*if(dir == 1)
		return;

	DRIVER_setdiv(1);

	for(st = 3; st > 0; st--)
	{
		DRIVER_step();
		_delay_ms(1.4);
	}

	DRIVER_setdir(dir);

	for(st = 3; st > 0; st--)
	{
		DRIVER_step();
		_delay_ms(1.4);
	}*/
}

uint16_t DRIVER_info()
{
	return DRIVER_position;
}

void DRIVER_reset()
{
	DRIVER_position = 0;
}