/****************
 * ADC
 ****************/
#include "system.h"
#include "ADC.h"

/*
 * Подготавливаем модуль АЦП.
 * 1. Выбираем внешнее опорное напряжение (REFS1, REFS0 = 0, 1)
 * 2. Включаем АЦП (бит ADEN) и задаем делитель частоты 128
 * 16000000/128 = 125000
 */
void ADC_init()
{
	ADMUX &= ~(1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	DDRC = 0;
	PORTC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC2) & ~(1<<PC3);
}

/*
 * 1. Устанавливаем канал считывания 0-7.
 * 2. AND с 00000111 будет держасть ch между 0 и 7.
 * 3. Чистим 3 нижних бита перед OR.
 * 4. Запускаем гуся-хуйся записав 1 в ADSC...
 * 5. ...и ждем, пока гусь-хуюсь завершится (когда ADSC станет 0).
 * 6. Принудительно очищаем флаг прерывания.
 * 7. Возвращаем 10-битный результат измерения.
 */
uint16_t ADC_read(uint8_t ch)
{
	ch &= 0b00000111; //0 < ch < 7
	ADMUX = (ADMUX & 0xF8) | ch; //0xF8 = 1111 1000
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
 
	return (ADC);
}