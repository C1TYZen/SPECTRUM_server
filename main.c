/***********************
 * MAIN
 ***********************/
#include "SERVER.h"

void move(long steps)
{
	_delay_ms(50);
	USART_flush();
	uint16_t msg = 0;

	while(steps > 0)
	{
		msg = USART_getmessage();
		if(msg == CMD_DI)
			break;
		DRIVER_step();
		steps--;
		_delay_ms(1.4);
		// 1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
	}
	USART_sendoncomp(CMD_MS);
}

void mesure(long steps)
{
	_delay_ms(50);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;

	while(steps > 0)
	{
		msg = USART_getmessage();
		if(msg == CMD_DI)
			break;
		mesure = ADC_read(0);
		USART_sendoncomp(mesure);
		DRIVER_step();
		steps--;
		_delay_ms(1.4);
		// 1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
	}
	USART_sendoncomp(CMD_MS);
}

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
		command = USART_getcommand();

		//MESURE
		if(command == CMD_MB)
		{
			DRIVER_backward();
			mesure(stepCount);
		}

		if(command == CMD_MF)
		{
			mesure(stepCount);
		}

		if(command == CMD_MR)
		{
			mesureRange0 = USART_get3bytes();
			mesureRange1 = USART_get3bytes();
			stepCount = mesureRange1 - mesureRange0;
			USART_println("range SET");
		}

		if(command == CMD_MC)
		{
			mesureCount = USART_get2bytes();
			USART_println("mps SET");
		}

		//DRIVER
		if(command == CMD_DS) 
		{
			DRIVER_step(stepCount);
		}

		if(command == CMD_DD)
		{
			DRIVER_chdir();
		}

		if(command == CMD_DV)
		{
			uint8_t temp = (uint8_t)USART_get2bytes();
			DRIVER_stepdiv(temp);
			USART_println("divider SET");
		}

		if(command == CMD_DF)
		{
			DRIVER_forward();
			USART_println("forward");
		}

		if(command == CMD_DB)
		{
			DRIVER_backward();
			USART_println("backward");
		}

		//TEST
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

		//OTHER
		if(command == CMD_ST)
		{
			stepCount = USART_get2bytes();
			USART_println("steps SET");
		}

		if(command == CMD_CC)
		{
			//USART_println("OLD WORLD NEWS");
			USART_println("**Connected**");
			command = 0;
		}

		command = 0;
		USART_flush();
	}

	return 0;
}