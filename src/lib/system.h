#define __AVR_ATmega168P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Команды
//set
#define CMD_MC	0x636d // установка количества измерений
//do
#define CMD_MB	0x626d // начать измерение
#define CMD_MR	0x726d // установка диапазона измерений
#define CMD_MS	0x736d // остановить измерение
#define CMD_MF	0x666d // поиск

//set
#define CMD_DD  0x6464 // установка направления
#define CMD_DM  0x6d64 // установка начала измерения
#define CMD_DV	0x7664 // установка делителя шага
//do
#define CMD_DS	0x7364 // шаг двигателя
#define CMD_DI	0x6964 // остановка двигателя
#define CMD_DB	0x6264 // направление двигателя назад
#define CMD_DF	0x6664 // направление двигателя вперед
#define CMD_DC	0x6364 // калибровка
#define CMD_DP  0x7064 // вывод информации о положении двигателя

//set
#define CMD_ST	0x7473 // установка количества шагов
//do
#define CMD_CC	0x6363 // проверить соединение
#define CMD_TP	0x7074 // тестирование пинов

//filter
#define CMD_FA  0x6166 // filter 1
#define CMD_FB  0x6266 // filter 2
#define CMD_FC  0x6366 // filter 3
#define CMD_FD  0x6466 // filter 4
#define CMD_FE  0x6566 // filter 5
#define CMD_FF  0x6666 // filter 6
#define CMD_FG 	0x6766 // filter zero position

#define PINMOD_IN   0
#define PINMOD_OUT  1

//Порты

// D2 	    PD2
// D3 	    PD3
// D4 	    PD4
// D5 	    PD5
// D6 	    PD6
// D7 	    PD7
// D8	    PB0
// D9	    PB1
// D10	    PB2
// D11	    PB3
// D12	    PB4

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

void ports_init();

uint8_t ports_getpin(int pin);
void ports_writepin(int pin, int bit);
void ports_switch(int pin);
void ports_pinmod(int pin, int mod);