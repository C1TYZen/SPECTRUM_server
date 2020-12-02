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

uint32_t DRIVER_position = 0;
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
	DRIVER_position += DRIVER_dir * (8/DRIVER_div);
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

void DRIVER_mvf(uint32_t trg)
{
	DRIVER_setdir(-1);
	while(DRIVER_position > trg)
	{
		DRIVER_setdiv(1);
		if((DRIVER_position - trg) < 8)
			DRIVER_setdiv(2);
		if((DRIVER_position - trg) < 4)
			DRIVER_setdiv(4);
		if((DRIVER_position - trg) < 2)
			DRIVER_setdiv(8);

		DRIVER_step();
		_delay_ms(1.4);
	}
}

void DRIVER_mvb(uint32_t trg)
{
	DRIVER_setdir(1);
	while(DRIVER_position < trg)
	{
		DRIVER_setdiv(1);
		if((trg - DRIVER_position) < 8)
			DRIVER_setdiv(2);
		if((trg - DRIVER_position) < 4)
			DRIVER_setdiv(4);
		if((trg - DRIVER_position) < 2)
			DRIVER_setdiv(8);

		DRIVER_step();
		_delay_ms(1.4);
	}
}

void DRIVER_moveto(uint32_t start)
{
	start *= 8;
	if(DRIVER_position < start)
	{
		DRIVER_mvb(start);
	}
	else if(DRIVER_position > start)
	{
		DRIVER_mvf(start);
	}
	else
		return;
}

uint32_t DRIVER_info()
{
	return DRIVER_position;
}

void DRIVER_reset()
{
	DRIVER_position = 0;
}