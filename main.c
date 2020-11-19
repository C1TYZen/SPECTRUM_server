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
#include "SERVER.h"

#define ENDER D8
#define ROTOR D9

void mesure(uint16_t st)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	uint16_t zero = 0;

	while(1)
	{
		msg = USART_getmessage();
		if(msg == CMD_DI)
			break;
		mesure = ADC_read(0);
		USART_send(mesure);
		DRIVER_step();
		_delay_ms(1.4);
		// 1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами

		if(st < 1)
			break;
		st--;
	}
	USART_flush();
	USART_send(CMD_MS);
}

int main() {
	uint16_t command = 0;
	uint16_t mesure_start;
	uint16_t steps = 100;
	uint16_t mesure_count;

	// Зааааапущаем все нужные библиотеки
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
			mesure_count = USART_get();
			USART_println("mps\tSET");
		}

		//do****************
		if(command == CMD_MB) //Измерение
		{
			DRIVER_moveto(mesure_start);

			DRIVER_setdiv(DRIVER_INSTALLED_DIV, 0);
			DRIVER_backward();
			mesure(steps);
		}

		if(command == CMD_MF) //Поиск
		{
			mesure(steps - 1);
		}

		//DRIVER
		//set***************
		if(command == CMD_DV) //Делитель
		{
			DRIVER_setdiv((uint8_t)USART_get(), 1);
			USART_println("divider\tSET");
		}

		if(command == CMD_DM) //Установка начала измерения
		{
			mesure_start = USART_get();
			USART_println("start\tSET");
		}

		if(command == CMD_DD) //Направление двигателя
		{
			DRIVER_setdir((int8_t)USART_get());
			USART_println("dir\tSET");
		}

		//do****************
		if(command == CMD_DS) 
		{
			DRIVER_step(steps);
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
			// Здесь задержками являются вызовы функции print.
			// Так двигатель крутится мягче всего.
			DRIVER_forward();	
			DRIVER_setdiv(1, 0);
			USART_println("Callibrating...");
			while(PORTB_getpin(ENDER))
			{
				_delay_ms(1.4);
				USART_print("Ender");
				DRIVER_step();
			}
			USART_println("END  ");

			DRIVER_backward();
			DRIVER_setdiv(8, 0);
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
			steps = USART_get();
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