/***********************
 * MAIN
 ***********************/
#include "lib/system.h"
#include "lib/ADC.h"
#include "lib/DRIVER.h"
#include "lib/USART.h"

#define EN_PIN		D2
#define MS1_PIN		D3
#define MS2_PIN		D4
#define MS3_PIN		D5
#define STEP_PIN	D6
#define DIR_PIN		D7
#define ENDER_PIN	D8
#define ROTOR_PIN	D9
#define SCMD_PIN	D10

void mesure(uint32_t steps, uint16_t mesure_count)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	uint8_t i = 0;

	while(1)
	{
		//Mesuring
		mesure = 0;
		for(i = mesure_count; i > 0; i--)
			mesure += ADC_read(0);
		mesure /= mesure_count;
		USART_send(mesure);

		//Thinking
		msg = USART_getmessage();
		if((msg == CMD_DI) || (steps < 1))
			break;
		
		//Walking
		DRIVER_step();
		steps--;
		_delay_ms(1.4);
		//1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
	}
	USART_flush();
	USART_send(CMD_MS);
}

void filter(int n) //Не трогай, работает.
{
	int k = 0;
	ports_writepin(SCMD_PIN, 1);

	for (k = 1; k <= 7; k++)
	{
		if (n > 0) 
		{
			ports_writepin(SCMD_PIN, 1);
			n--;
		} 
		else 
		{
			ports_writepin(SCMD_PIN, 0);
		}
		_delay_ms(50);
	}
	ports_pinmod(SCMD_PIN, PINMOD_IN);
	_delay_ms(50);
	while(ports_getpin(SCMD_PIN) == 0) {}
	USART_println("OK");
	ports_pinmod(SCMD_PIN, PINMOD_OUT);
	ports_writepin(SCMD_PIN, 0);
}

int main()
{
	uint16_t 	command 			= 0;
	uint16_t 	cfg_mesure_start 	= 0;
	uint16_t 	cfg_steps 			= 100;
	uint16_t 	cfg_mesure_count 	= 1;
	uint8_t 	cfg_filter 			= 0;
	uint8_t 	cfg_div 			= 1;
	uint8_t 	cfg_dir 			= 1;

	//Зааааапущаем все библиотеки
	ports_init();
	ADC_init();
	USART_init();
	DRIVER_init(EN_PIN, MS1_PIN, MS2_PIN, MS3_PIN, STEP_PIN, DIR_PIN);

	ports_pinmod(SCMD_PIN, PINMOD_OUT);
	ports_writepin(SCMD_PIN, 0);
	
	while(1) {
		command = USART_get();

		if(command == CMD_TF)
		{
			tf_test(42);
		}

		//MESURE
		//set***************
		if(command == CMD_MC)
		{
			cfg_mesure_count = USART_get();
			USART_println("mps\tSET");
		}

		//do****************
		if(command == CMD_MB) //Измерение
		{
			DRIVER_moveto(cfg_mesure_start);

			cfg_div = DRIVER_setdiv(cfg_div);
			DRIVER_backward();
			uint32_t steps = (uint32_t)cfg_steps * (uint32_t)cfg_div;
			mesure(steps, cfg_mesure_count);
		}

		if(command == CMD_MT) //ТЕСТОВОЕ измерение
		{
			DRIVER_moveto(0);

			DRIVER_setdiv(1);
			DRIVER_backward();
			uint32_t steps = (uint32_t)1000 * (uint32_t)1;
			mesure(steps, 1);
		}

		//DRIVER
		//set***************
		if(command == CMD_DV) //Делитель
		{
			cfg_div = (uint8_t)USART_get();
			USART_println("divider\tSET");
		}

		if(command == CMD_DM) //Начало измерения
		{
			cfg_mesure_start = USART_get();
			USART_println("start\tSET");
		}

		if(command == CMD_DD) //Направление двигателя
		{
			cfg_dir = (int8_t)USART_get();
			USART_println("dir\tSET");
		}

		//do****************
		if(command == CMD_DS) 
		{
			DRIVER_step(cfg_steps);
		}

		if(command == CMD_DF)
		{
			DRIVER_forward();
		}

		if(command == CMD_DB)
		{
			DRIVER_backward();
		}

		if(command == CMD_DC) 
		{
			DRIVER_forward();	
			DRIVER_setdiv(1);
			USART_println("Callibrating...");
			while(ports_getpin(ENDER_PIN))
			{
				_delay_ms(1.4);
				USART_print("Ender");
				DRIVER_step();
			}
			USART_println("END  ");

			DRIVER_backward();
			DRIVER_setdiv(8);
			while(ports_getpin(ROTOR_PIN))
			{
				_delay_ms(1.4);
				USART_print("Rotor");
				DRIVER_step();
			}
			USART_println("CALLIBRATED");
			DRIVER_reset();
		}

		//TEST
		if(command == CMD_TP)
		{
			if(ports_getpin(ENDER_PIN))
				USART_println("Ender 1");
			else
				USART_println("Ender 0");
			if(ports_getpin(ROTOR_PIN))
				USART_println("Rotor 1");
			else
				USART_println("Rotor 0");
		}

		//OTHER
		//set***************
		if(command == CMD_ST)
		{
			cfg_steps = USART_get();
			USART_println("steps\tSET");
		}

		//do****************
		if(command == CMD_CC)
		{
			//USART_println("OLD WORLD NEWS");
			USART_println("**Connected**");
		}

		//FILTER
		if(command == CMD_FA)
		{
			filter(1);
		}
		if(command == CMD_FB)
		{
			filter(2);
		}
		if(command == CMD_FC)
		{
			filter(3);
		}
		if(command == CMD_FD)
		{
			filter(4);
		}
		if(command == CMD_FE)
		{
			filter(5);
		}
		if(command == CMD_FF)
		{
			filter(6);
		}
		if(command == CMD_FZ)
		{
			filter(7);
		}

		command = 0;
		USART_flush();
	}

	return 0;
}