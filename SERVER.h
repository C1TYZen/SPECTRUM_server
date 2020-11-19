#define __AVR_ATmega168P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

// Команды
//set
#define CMD_MC	25453 // установка количества измерений
//do
#define CMD_MB	25197 // начать измерение
#define CMD_MR	29293 // установка диапазона измерений
#define CMD_MS	29549 // остановить измерение
#define CMD_MF	26221 // поиск

//set
#define CMD_DD  25700 // установка направления
#define CMD_DM  28004 // установка начала измерения
#define CMD_DV	30308 // установка делителя шага
//do
#define CMD_DS	29540 // шаг двигателя
#define CMD_DI	26980 // остановка двигателя
#define CMD_DB	25188 // направление двигателя назад
#define CMD_DF	26212 // направление двигателя вперед
#define CMD_DC	25444 // калибровка
#define CMD_DP  28772 // вывод информации о положении двигателя

//set
#define CMD_ST	29811 // установка количества шагов
//do
#define CMD_CC	25443 // проверить соединение
#define CMD_TP	28788 // тестирование пинов

/***********************
 * USART
 ***********************/
void USART_init();
void USART_flush();

uint8_t USART_read();
uint16_t USART_get();
uint8_t USART_readnow();
uint16_t USART_getmessage();
void USART_readln(char* str);

void USART_write(uint8_t data);
void USART_send(uint16_t data);
void USART_println(char* string);
void USART_print(char* string);
void USART_putbyteview(uint8_t data);

/***********************
 * ADC
 ***********************/
void ADC_init();
uint16_t ADC_read(uint8_t ch);

/***********************
 * DRIVER
 ***********************/
#define DRIVER_INSTALLED_DIV 0
#define DRIVER_BCK 1
#define DIRVER_FWD -1

void DRIVER_init();
void DRIVER_step();
void DRIVER_chdir();
void DRIVER_forward();
void DRIVER_backward();
int8_t DRIVER_setdir(int8_t dir);
int8_t DRIVER_setdiv(uint8_t div, uint8_t save);
void DRIVER_moveto(uint16_t r1);

uint16_t DRIVER_info();

/***********************
 * PORTB
 ***********************/
#define D8		PB0
#define D9		PB1
#define D10		PB2
#define D11		PB3
#define D12		PB4

void PORTB_init();
uint8_t PORTB_getpin(int pin);