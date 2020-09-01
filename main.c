/***********************
 * MAIN
 ***********************/
#include "SERVER.h"

int main() {
	// Зааааапущаем все нужные библиотеки
	USART_init();
	ADC_init();
	DRIVER_init();

	uint16_t command = 0;

	long mesureRange0;
	long mesureRange1;
	uint16_t mesureCount;
	long stepCount = 100;

	// USART_println("ArADC v0.6a");
	
	while(1) {
		command = USART_getcommand();

		// bm - 28002
		if(command == 28002) {
			_delay_ms(50);
			USART_flush();
			uint16_t mes = 0;
			long i = stepCount;

			while(i > 0) {
				mes = ADC_read(0);
				USART_sendoncomp(mes);
				DRIVER_step();
				i--;
				// 1.6 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
				_delay_ms(1.4);
				// ... и теперь это нахер не нужно.
				// теперь это все нахер не нужно
				// нужно подавать импульсы через определенные
				// промежутки времени
			}
			// sm - 28019 - stops reciever
			USART_sendoncomp(28019);
		}

		// mr - 29293
		if(command == 29293) {
			mesureRange0 = USART_get3bytes();
			mesureRange1 = USART_get3bytes();
			stepCount = mesureRange1 - mesureRange0;
			USART_println("mr SET");
		}

		// st - 29811
		if(command == 29811) {
			stepCount = USART_get2bytes();
			USART_println("st SET");
		}

		// mc - 25453
		if(command == 25453) {
			mesureCount = USART_get2bytes();
			USART_println("mc SET");
		}

		// cc - 25443
		if(command == 25443) {
			//USART_println("OLD WORLD NEWS");
			USART_println("**Connected**");
			command = 0;
		}

		//DRIVER

		// ds - 29540
		if(command == 29540) {
			while (stepCount >= 0) {
				DRIVER_step();
				stepCount--;
				_delay_ms(2);
			}
		}

		// dd - 25700
		if(command == 25700) {
			DRIVER_chdir();
		}
		// dv - 30308
		if(command == 30308) {
			DRIVER_stepdiv(USART_get());
		}
		USART_flush();
		command = 0;
	}

	return 0;
}