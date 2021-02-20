#include "lib/system.h"
#include "lib/DRIVER.h"
#include "lib/USART.h"

// #define DIR 		PD3
// #define STEP 	PD4
// #define MS3		PD7
// #define MS2		PB0
// #define MS1		PB1
// #define EN		PB2

#define MCMD_PIN	D2 //Пин команд
#define DIR_PIN		D3
#define STEP_PIN	D4

#define SENS_PWR	D5
#define SENS_GND	D6

#define MS3_PIN		D7
#define MS2_PIN		D8
#define MS1_PIN		D9
#define EN_PIN		D10

#define SENS_0		D11	//Датчик нулевого положения
#define SENS_C		D12	//Датчик текущего положения

#define ON 1
#define OFF 0

//!!!ПРОТЕСТИРОВАТЬ!!!
void motor(int onoff) 
{
	if(onoff == ON)
		ports_writepin(EN_PIN, 0);

	_delay_ms(2);

	if(onoff == OFF)
		ports_writepin(EN_PIN, 1);
}
//!!!

void step(int steps)
{
	int i = 0;
	
	DRIVER_forward();
	for(i = 0; i < steps; i++)
	{
		DRIVER_step();
		_delay_ms(2);
	}
}
			
void zero_position()
{
	ports_writepin(EN_PIN, 0);
	_delay_ms(5);

	while(ports_getpin(SENS_0) == 1)
		step(1);
		
	_delay_ms(5);
	ports_writepin(EN_PIN, 1);
}

void filter_position(int pos, int num)
{
	int i;

	if(pos < num)
	{
		num -= pos;
	}
	else
	{
		num += 6 - pos;
	}

	ports_writepin(EN_PIN, 0);
	_delay_ms(5);
	for(i = 0; i < num; i++)
	{
		step(200);
		while(ports_getpin(SENS_C) == 1)
			step(1);
	}
	_delay_ms(5);		
	ports_writepin(EN_PIN, 1);
}

void main()
{
	ports_init();
	DRIVER_init(EN_PIN, MS1_PIN, MS2_PIN, MS3_PIN, STEP_PIN, DIR_PIN);
	USART_init();

	//Питание оптопары
	ports_pinmod(SENS_PWR, PINMOD_OUT);
	ports_pinmod(SENS_GND, PINMOD_OUT);
	ports_writepin(SENS_PWR, 1);
	ports_writepin(SENS_GND, 0);

	ports_pinmod(SENS_0, PINMOD_IN);
	ports_pinmod(SENS_C, PINMOD_IN);

	ports_pinmod(MCMD_PIN, PINMOD_IN);

	ports_writepin(EN_PIN, 1);

	zero_position();
	int pos = 1;
	uint8_t cmd = 0;
	_delay_ms(50);

	while(1)
	{
		cmd = ports_getpin(MCMD_PIN);

		if(cmd == 1)
		{
			_delay_ms(2);
			cmd = 0;
			int i = 1;
			for(; i <= 7; i++)
			{
				cmd += ports_getpin(MCMD_PIN);
				_delay_ms(50); //Задержки для синхронизации
			}

			ports_pinmod(MCMD_PIN, PINMOD_OUT);
			ports_writepin(MCMD_PIN, 0);

			cmd &= 0b00000111; //cmd никогда не будет > 7 или < 0
			
			if(cmd == 7)
			{
				zero_position();
				pos = 1;
			}
			else
			{
				filter_position(pos, cmd);
				pos = cmd;
			}
			
			char s[10];
			sprintf(s, "%d", cmd);
			USART_println(s);
			
			ports_writepin(MCMD_PIN, 1);
			_delay_ms(200); //Чтобы последний бит не вызывал еще одну итерацию
			ports_pinmod(MCMD_PIN, PINMOD_IN);
		}

		cmd = 0;
	}
}
