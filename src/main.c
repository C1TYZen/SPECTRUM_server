/*
	Интерфейс для функций установки значений:
	function()
	{
		принять_значение()
		выполнить действие()
		отправить_строку_подтверждения()
	}
 */

/***********************
 * MAIN
 ***********************/
#include "lib/system.h"
#include "lib/ADC.h"
#include "lib/DRIVER.h"
#include "lib/PORTB.h"
#include "lib/USART.h"

#define SRF 	D10

#define ENDER 	D8
#define ROTOR 	D9
#define PIN_MOD_IN 	0
#define PIN_MOD_OUT 1

//FILTER
#define NUM_MAX 6
#define NUM_MIN 1

/* Заготовка под человека
typedef struct
{
	uint16_t start;
	uint16_t steps;
	uint16_t mps;
	uint8_t div;
	uint8_t dir;
} config_info;
*/

void mesure(uint32_t st, uint16_t mc)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	uint8_t i;

	while(1)
	{
		mesure = 0;
		for(i = mc; i > 0; i--)
			mesure += ADC_read(0);
		mesure /= mc;
		USART_send(mesure);
		//USART_send(DRIVER_info()/8);

		msg = USART_getmessage();
		if((msg == CMD_DI) || (st < 1))
			break;
		DRIVER_step();
		st--;
		_delay_ms(1.4);
		// 1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
	}
	USART_flush();
	USART_send(CMD_MS);
}

void filter(int n) 
{
	// Читаем состояние линии (в неактивном состоянии оба контроллера
	// настраивают линию на "вход" и подтягивают к +5В)
	int SRFS = PORTB_getpin(SRF);
	int k = 0;

	//Если нога подтянута к земле со стороны БС(блока светофильтров), 
	//то значит он занят - ничего не предпринимаем...
	if (SRFS == 0)
		return;
	// ...иначе опускаем вывод, сообщая светофильтру, 
	// что сейчас будет  передаваться команда
	PORTB_writepin(SRF, 0);
	PORTB_pinmod(SRF, 1);
	_delay_ms(2);
	// В цикле передаем команду
	for (k = 1; k <= 6; k++)
	{
		if (n > 0) 
		{
			PORTB_writepin(SRF, 1);
			n--;
		} 
		else 
		{
			PORTB_writepin(SRF, 0);
		}
		_delay_ms(2);
	}
	// Настраиваем вывод на вход по окончанию передачи
	PORTB_pinmod(SRF, 0);
	_delay_ms(300);
	// Записываем состояние фильтра
	return n;
}

int main() {
	uint16_t command = 0;
	uint16_t cfg_mesure_start;
	uint16_t cfg_steps = 100;
	uint16_t cfg_mesure_count = 1;
	uint8_t cfg_filter = 0;
	uint8_t cfg_div = 1;
	uint8_t cfg_dir = 1;

	driver_config drv_cfg = 
	{
		.EN = PD2,
		.MS1 = PD3,
		.MS2 = PD4,
		.MS3 = PD5,
		.STEP = PD6,
		.DIR = PD7
	};

	// Зааааапущаем все библиотеки
	USART_init();
	ADC_init();
	DRIVER_init();
	PORTB_init();
	
	while(1) {
		command = USART_get();

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
			while(PORTB_getpin(ENDER))
			{
				_delay_ms(1.4);
				USART_print("Ender");
				DRIVER_step();
			}
			USART_println("END  ");

			DRIVER_backward();
			DRIVER_setdiv(8);
			while(PORTB_getpin(ROTOR))
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
			if(PORTB_getpin(ENDER))
				USART_println("Ender 1");
			else
				USART_println("Ender 0");
			if(PORTB_getpin(ROTOR))
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

		command = 0;
		USART_flush();
	}

	return 0;
}