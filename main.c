#include "SERVER.h"

/***********************
 * MAIN
 ***********************/

int main() {
	// Зааааапущаем все нужные библиотеки
	USART_init();
	ADC_init();
	DRIVER_init();

	uint16_t command;

	long mesureRange0;
	long mesureRange1;
	uint16_t mesureCount;
	long stepCount = 100;

	// USART_println("ArADC v0.6a");
	
	while(1) {
		command = USART_get2bytes();

		// bm - 28002
		if(command == 28002) {
			_delay_ms(50);
			USART_flush();
			uint16_t mes = 0;
			long i = stepCount;
			while(i > 0) {
				mes = ADC_read(0);
				USART_send2bytes(mes);
				DRIVER_step();
				i--;
				// 1.6 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
				_delay_ms(1.4);
				// ... и теперь это нахер не нужно.
			}
			// sm - 28019 - stops reciever
			USART_send2bytes(28019);
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
		}

		//DRIVER

		// ds - 29540
		if(command == 29540) {
			DRIVER_step();
		}

		// dd - 25700
		if(command == 25700) {
			DRIVER_chdir();
		}
	}

	return 0;
}