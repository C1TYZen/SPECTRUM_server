/***********************
 * MAIN
 ***********************/
#include "SERVER.h"

int main() {
	// Зааааапущаем все нужные библиотеки
	USART_init();
	ADC_init();
	DRIVER_init();
	PORTB_init();

	uint16_t command = 0;
	long mesureRange0;
	long mesureRange1;
	uint16_t mesureCount;
	long stepCount = 100;
	
	while(1) {
		// Очистка буфера должна быть здесь
		// При вызове очистки буфера из другого места
		// в клиент перестают приходить сообщения сервера
		USART_flush();
		command = USART_getcommand();

		if(command == CMD_MB)
		{
			_delay_ms(50);
			USART_flush();
			uint16_t mesure = 0;
			long steps = stepCount;
			DRIVER_backward();

			while(steps > 0) {
				mesure = USART_getmessage();
				if(mesure == CMD_DI) // di
					break;
				mesure = ADC_read(0);
				USART_sendoncomp(mesure);
				DRIVER_step();
				steps--;
				// 1.6 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
				_delay_ms(1.4);
				// ... и теперь это нахер не нужно.
				// теперь это все нахер не нужно
				// нужно подавать импульсы через определенные
				// промежутки времени
			}
			// sm - 28019 - stops reciever
			USART_sendoncomp(CMD_MS);
		}
		// mr - 29293
		if(command == CMD_MR)
		{
			mesureRange0 = USART_get3bytes();
			mesureRange1 = USART_get3bytes();
			stepCount = mesureRange1 - mesureRange0;
			USART_println("mr SET");
		}
		// st - 29811
		if(command == CMD_ST)
		{
			stepCount = USART_get2bytes();
			USART_println("st SET");
		}
		// mc - 25453
		if(command == CMD_MC)
		{
			mesureCount = USART_get2bytes();
			USART_println("mc SET");
		}
		// cc - 25443
		if(command == CMD_CC)
		{
			//USART_println("OLD WORLD NEWS");
			USART_println("**Connected**");
			command = 0;
		}

		//DRIVER

		// ds - 29540
		if(command == CMD_DS) 
		{
			DRIVER_move(stepCount);
		}
		// dd - 25700
		if(command == CMD_DD)
		{
			DRIVER_chdir();
		}
		// dv - 30308
		if(command == CMD_DV)
		{
			DRIVER_stepdiv(USART_get());
		}
		// df - 26212
		if(command == CMD_DF)
		{
			DRIVER_forward();
			USART_println("forward");
			DRIVER_move(stepCount);
		}
		// db - 25188
		if(command == CMD_DB)
		{
			DRIVER_backward();
			USART_println("backward");
			DRIVER_move(stepCount);
		}

		//TEST
		//tp
		if(command == CMD_TP)
		{
			if(PORTB_getpin(D8))
				USART_println("Ender 1");
			else
				USART_println("Ender 0");
			if(PORTB_getpin(D9))
				USART_println("Rotor 1");
			else
				USART_println("Rotor 0");
		}

		if(command == CMD_DC) 
		{
			// Здесь задержками являются вызовы функции print.
			// Так двигатель крутится мягче всего.
			DRIVER_forward();	
			DRIVER_stepdiv(1);
			USART_println("Callibrating...");
			while(PORTB_getpin(D8))
			{
				USART_print("Ender");
				DRIVER_step();
			}
			USART_println("END  ");

			DRIVER_backward();
			DRIVER_stepdiv(3);
			while(PORTB_getpin(D9))
			{
				USART_print("Rotor");
				DRIVER_step();
			}
			USART_println("CALLIBRATED");
		}

		command = 0;
	}

	return 0;
}