#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

/***********************
 * USART
 ***********************/
// Скорость обмена данными по USART в бит/с
// 76800 подобрано по таблице
#define BAUDRATE 76800
// Значение регистра UBRR0, соответствующее выбранной скорости
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

/********************************************************
 * 1. Устанавливаем скорость обмена по USART
 * 2. Включаем возможность приема и передачи по интерфейсу,
 * а также разрешаем прерывание после каждого получения данных
 * 3. Определяем следующие настройки USART: 8 бит
 * в одном сообщении, 1 стоповый бит, без проверки четности
 ********************************************************/
void USART_init() {
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)BAUD_PRESCALLER;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

// очистка буфера
void USART_flush() {
	while (!(UCSR0A&(1<<UDRE0))) {
		int dummy = UDR0;
	}
}
/********************************************************
 * 1. Функция приема данных по USART (из регистра-буфера)
 * 2. Ждем "наполнения" регистра буфера-приемника...
 * 3. ...и возвращаем новые не прочитанные данные
 * сразу после их появления
 ********************************************************/
uint8_t USART_get() {	
	while (!(UCSR0A&(1<<RXC0))) {}
	return UDR0;
}

uint16_t USART_getnow() {
	uint16_t cmd = 0;
	cmd = UDR0;
	cmd += (UDR0<<8);
	return cmd;
}

// Функция приема 2 байт данных
uint16_t USART_get2bytes() {
	uint16_t cmd = 0;
	while (!(UCSR0A&(1<<RXC0))) {}
	cmd = UDR0;
	while (!(UCSR0A&(1<<RXC0))) {}
	cmd += (UDR0<<8);
	return cmd;
}

long USART_get3bytes() {
	long cmd = 0;
	while (!(UCSR0A&(1<<RXC0))) {}
	cmd = UDR0;
	cmd = (cmd<<8);
	while (!(UCSR0A&(1<<RXC0))) {}
	cmd += UDR0;
	cmd = (cmd<<8);
	while (!(UCSR0A&(1<<RXC0))) {}
	cmd += UDR0;
	return cmd;
}

/********************************************************
 * 1. Функция отправки 1 байта данных data по USART
 * 2. Ждем, пока буферный регистр пересылки не очистится,
 * чтобы передать очередной байт данных...
 * 3. ...и записываем байт в регистр. Отправка начнется
 * автоматически после его заполнения
 ********************************************************/
void USART_send(uint8_t data) {
	while (!(UCSR0A&(1<<UDRE0))) {}
	UDR0 = data;
}

// Функция отправки 2 байт данных
void USART_send2bytes(uint16_t data) {
	while (!(UCSR0A&(1<<UDRE0))) {}
	UDR0 = data;
	while (!(UCSR0A&(1<<UDRE0))) {}
	UDR0 = data>>8;
}

/********************************************************
 * Функция отправки строки через USART.
 * Входной аргумент - указатель на строку string.
 * В цикле последовательно отправляем строку байт за байтом.
 ********************************************************/
void USART_println(char* string) {
	while (*string != 0) {
		USART_send(*string);
		string++;
	}
	USART_send('\r');
	USART_send('\n');
}

/********************************************************
 * Функция предназначена для формирования и отправки чисел
 * в двоичном представлении.
 ********************************************************/
void USART_putbyteview(uint8_t data) {
	uint8_t cur = 8;
	while(cur!=0) {
	cur--;
	if((data>>cur) & 1) USART_send('1');
	if(!((data>>cur) & 1)) USART_send('0');
	}
}

/***********************
 * ADC
 ***********************/

/*********************************************************
 * Подготавливаем модуль АЦП.
 * 1. Выбираем внутреннее опорное напряжение МК AVcc
 * 2. Включаем АЦП (бит ADEN) и задаем делитель частоты 128
 * 16000000/128 = 125000
 *********************************************************/
void ADC_init() {
	ADMUX &= ~(1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	DDRC &= ~(1<<PC0);
	PORTC |= (1<<PC0);
}

/*********************************************************
 * 1. Устанавливаем канал считывания 0-7.
 * 2. AND с 00000111 будет держасть ch между 0 и 7.
 * 3. Чистим 3 нижних бита перед OR.
 * 4. Запускаем гуся-хуйся записав 1 в ADSC...
 * 5. ...и ждем, пока гусь-хуюсь завершится (когда ADSC станет 0).
 * 6. Принудительно очищаем флаг прерывания.
 * 7. Возвращаем 10-битный результат измерения.
 *********************************************************/
uint16_t ADC_read(uint8_t ch) {
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8) | ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
 
	return (ADC);
}

/***********************
 * TIMER
 ***********************/
 
/* ******************************************************
 * Настройка таймера требуется для работы 
 * функции millis();
 * 1. Разрешаем прерывание от данного таймера 
 * 2. Задаем делитель частоты 1024 и запускаем таймер
 ********************************************************/
void TIMER_init() {
	TIMSK0 |= (1<<TOIE0);
	TCCR0B |= (1<<CS02) | (1<<CS00);
}

/***********************
 * DRIVER
 ***********************/
#define DRIVER_DDR  DDRD
#define DRIVER_PORT PORTD
#define STEP  PD2
#define DIR   PD3
 
void DRIVER_init() {
	DRIVER_DDR |= (1<<STEP)|(1<<DIR);
	DRIVER_PORT |= (1<<STEP);
	DRIVER_PORT |= (1<<DIR);
}

void DRIVER_step() {
	DRIVER_PORT ^= (1<<STEP);
}

void DRIVER_chdir() {
	DRIVER_PORT ^= (1<<DIR);
}

/***********************
 * MAIN
 ***********************/

void conf(uint16_t var, uint16_t def) {
	var = def;
}

int main() {
	// Зааааапущаем все нужные библиотеки
	USART_init();
	ADC_init();
	// TIMER_init();
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