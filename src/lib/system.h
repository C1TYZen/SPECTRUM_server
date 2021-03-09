#define __AVR_ATmega168P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

//Функции
#define CMD_MB	0x626d // начать измерение
#define CMD_DZ  0x7a64 // двигатель на ноль
#define CMD_CC	0x6363 // проверить соединение
#define CMD_CS  0x7363 // установка значения переменной
#define CMD_TP	0x7074 // тестирование пинов
#define CMD_TF  0x6674 // тестовая функция

//Сообщения
#define CMD_MS	0x736d // остановить измерение
#define CMD_MI  0x696d // прервать измерение

//Переменные
#define CMD_MA  0x616d // начало измерения
#define CMD_MZ  0x7a6d // конец измерения
#define CMD_MC  0x636d // количество измерений
#define CMD_DS  0x7364 // скорость двигателя
#define CMD_FN  0x6e66 // номер фильтра
#define CMD_FS  0x7366 // шаг установки фильтра

//Порты
// D2 		PD2
// D3 		PD3
// D4 		PD4
// D5 		PD5
// D6 		PD6
// D7 		PD7
// D8		PB0
// D9		PB1
// D10		PB2
// D11		PB3
// D12		PB4

#define D2 		2
#define D3 		3
#define D4 		4
#define D5 		5
#define D6 		6
#define D7 		7
#define D8		8
#define D9		9
#define D10		10
#define D11		11
#define D12		12

#define PINMOD_IN	0
#define PINMOD_OUT	1

/****************
 * PORTS
 ****************/
void ports_init();
uint8_t ports_getpin(int pin_num);
void ports_writepin(int pin_num, int bit);
void ports_switch(int pin_num);
void ports_pinmod(int pin_num, int PINMOD);

/****************
 * UTILS
 ****************/
void str_parse(char* string, char* name, size_t str_size, int* optinons);