/***********************
 * USART
 ***********************/
#include "SERVER.h"

#define F_CPU 16000000
// Скорость обмена данными по USART в бит/с
// 76800 подобрано по таблице /papers/AVR Baud Rate Tables.htm
#define BAUDRATE 76800
// Значение регистра UBRR0, соответствующее выбранной скорости
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

/*
 * 1. Устанавливаем скорость обмена по USART
 * 2. Включаем возможность приема и передачи по интерфейсу,
 * а также разрешаем прерывание после каждого получения данных
 * 3. Определяем следующие настройки USART: 8 бит
 * в одном сообщении, 1 стоповый бит, без проверки четности
 */
void USART_init()
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)BAUD_PRESCALLER;
	//UCSR A

	//UCSR B
	UCSR0B = (1<<RXCIE0) | (1<<TXEN0) | (1<<RXEN0);
	//UCSR C
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

// Очистка буфера (выключение и включение рессивера)
void USART_flush()
{
	UCSR0B &= ~(1<<RXEN0);
	UCSR0B |= (1<<RXEN0);
}

/*
 * Чтение данных по USART (из регистра-буфера)
 * 1. Ждем "наполнения" регистра буфера-приемника...
 * 2. ...и возвращаем новые не прочитанные данные
 *    сразу после их появления
 */
uint8_t USART_read()
{	
	while (!(UCSR0A&(1<<RXC0))) {}
	return UDR0;
}

// Чтение пакета байт
int32_t USART_get(uint8_t bytes)
{
	long cmd = 0;
	long buf = 0;
	uint8_t i = 0;
	for(i = 0; i < bytes; i++)
	{
		while (!(UCSR0A&(1<<RXC0))) {}
		buf = UDR0;
		cmd += (buf<<(8 * i));
		buf = 0;
	}
	return cmd;
}

// Чтение байта без ожидания заполнения буфера
uint8_t USART_readnow()
{
	return UDR0;
}

// Прием мнгновенного сообщения
uint16_t USART_getmessage()
{
	uint16_t cmd = 0;
	cmd = UDR0;
	cmd += (UDR0<<8);
	return cmd;
}

// Чтение строки
void USART_readln(char* str)
{
	int i;
	for(i = 0; i < 100; i++)
	{
		*str = USART_read();
		if(*str == ';')
			break;
		str++;
	}
}

/*
 * Отправка 1 байта данных data по USART
 * 1. Ждем, пока буферный регистр пересылки не очистится,
 * чтобы передать очередной байт данных...
 * 2. ...и записываем байт в регистр. Отправка начнется
 * автоматически после его заполнения
 */
void USART_write(uint8_t data)
{
	while (!(UCSR0A&(1<<UDRE0))) {}
	UDR0 = data;
}

// Отправка пакета байт
void USART_send(long data, uint8_t bytes)
{
	uint8_t i = 0;
	for(i = 0; i < bytes; i++)
	{
		data = (data>>(8 * i));
		while (!(UCSR0A&(1<<UDRE0))) {}
		UDR0 = data;	
	}
}

/*
 * Отправка строки через USART.
 * Входной аргумент - указатель на строку string.
 * В цикле последовательно отправляем строку байт за байтом.
 */
void USART_println(char* string)
{
	while (*string != 0) {
		USART_write(*string);
		string++;
	}
	USART_write('\r');
	USART_write('\n');
}

// Отправка строки без перевода на следующую
void USART_print(char* string)
{
	while (*string != 0) {
		USART_write(*string);
		string++;
	}
	USART_write('\r');
}

// Формирование отправка чисел в двоичном виде
void USART_putbyteview(uint8_t data)
{
	uint8_t cur = 8;
	while(cur!=0)
	{
		cur--;
		if((data>>cur) & 1) USART_write('1');
		if(!((data>>cur) & 1)) USART_write('0');
	}
}