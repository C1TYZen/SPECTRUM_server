/***********************
 * MAIN
 ***********************/
#include "SERVER.h"

#define ENDER D8
#define ROTOR D9

void mesure(long st)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	for(;st > 0; st--)
	{
		msg = USART_getmessage();
		if(msg == CMD_DI)
			break;
		mesure = ADC_read(0);
		USART_send(mesure, 2);
		DRIVER_step();
		_delay_ms(1.4);
		// 1.4 mc подобрано потом, кровью и индийскими сусликами аутистами-аутсорсерами
	}
	USART_flush();
	USART_send(CMD_MS, 2);
}

int main() {
	// Зааааапущаем все нужные библиотеки
	USART_init();
	ADC_init();
	DRIVER_init();
	PORTB_init();

	uint16_t command = 0;
	long mesure_range0;
	long mesure_range1;
	long steps = 100;
	float count_steps = 0;
	uint16_t mesure_count;
	long position;
	
	while(1) {
		command = USART_get(2);

		//MESURE
		if(command == CMD_MB)
		{
			//DRIVER_moveto(mesure_range0);
			DRIVER_backward();
			mesure(steps);
		}

		if(command == CMD_MF)
		{
			mesure(steps);
		}

		if(command == CMD_MR)
		{
			mesure_range0 = USART_get(3);
			mesure_range1 = USART_get(3);
			if(mesure_range1 < mesure_range0)
			{
				long buf = mesure_range0;
				mesure_range0 = mesure_range1;
				mesure_range1 = buf;
			}
			steps = mesure_range1 - mesure_range0;
			USART_println("range SET");
		}

		if(command == CMD_MC)
		{
			mesure_count = USART_get(2);
			USART_println("mps SET");
		}

		//DRIVER
		if(command == CMD_DS) 
		{
			DRIVER_step(steps);
		}

		if(command == CMD_DV)
		{
			uint8_t temp = (uint8_t)USART_get(2);
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

		if(command == CMD_DC) 
		{
			// Здесь задержками являются вызовы функции print.
			// Так двигатель крутится мягче всего.
			DRIVER_forward();	
			DRIVER_stepdiv(1);
			USART_println("Callibrating...");
			count_steps = 0;
			while(PORTB_getpin(ENDER))
			{
				USART_print("Ender");
				DRIVER_step();
				count_steps++;
			}
			USART_println("END  ");

			DRIVER_backward();
			DRIVER_stepdiv(8);
			while(PORTB_getpin(ROTOR))
			{
				USART_print("Rotor");
				DRIVER_step();
				count_steps -= 1.0/8.0;
			}
			USART_println("CALLIBRATED");
			USART_send((long)count_steps, 3);
			DRIVER_reset();
		}

		if(command == CMD_DP)
		{
			DRIVER_info();
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
		if(command == CMD_ST)
		{
			steps = USART_get(2);
			USART_println("steps SET");
		}

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