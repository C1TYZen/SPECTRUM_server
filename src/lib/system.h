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

//filter
// #define CMD_FA  24934 // filter 1 0x6166
// #define CMD_FB  25190 // filter 2 0x6266
// #define CMD_FC  25446 // filter 3 0x6366
// #define CMD_FD  25702 // filter 4 0x6466
// #define CMD_FE  25958 // filter 5 0x6566
// #define CMD_FF  26214 // filter 6 0x6666

#define CMD_FA 0x6166
#define CMD_FB 0x6266
#define CMD_FC 0x6366
#define CMD_FD 0x6466
#define CMD_FE 0x6566
#define CMD_FF 0x6666

// fa - 0x6166
// fb - 0x6266
// fc - 0x6366
// fd - 0x6466
// fe - 0x6566
// ff - 0x6666
