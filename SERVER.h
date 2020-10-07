#define __AVR_ATmega168P__
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <stdlib.h>
#include <util/delay.h>

// Команды
#define CMD_MB	25197 // начать измерение
#define CMD_MC	25453 // установка количества измерений
#define CMD_MR	29293 // установка диапазона измерений
#define CMD_MS	29549 // остановить измерение
#define CMD_MF	26221 // поиск

#define CMD_DB	25188 // движение двигателя назад
#define CMD_DC	25444 // калибровка
#define CMD_DD	25700 // изменить направление вращения драйвера
#define CMD_DF	26212 // движение двигателя вперед
#define CMD_DI	26980 // остановка двигателя
#define CMD_DS	29540 // шаг двигателя
#define CMD_DV	30308 // установка делителя шага

#define CMD_CC	25443 // проверить соединение
#define CMD_ST	29811 // установка количества шагов
#define CMD_TP	28788 // тестирование пинов

/***********************
 * USART
 ***********************/
void USART_init();
void USART_flush();
uint8_t USART_get();
uint16_t USART_getnow();

uint16_t USART_get2bytes();
long USART_get3bytes();
void USART_send(uint8_t data);
void USART_send2bytes(uint16_t data);

void USART_println(char* string);
void USART_putbyteview(uint8_t data);

/***********************
 * ADC
 ***********************/
void ADC_init();
uint16_t ADC_read(uint8_t ch);

/***********************
 * DRIVER
 ***********************/
void DRIVER_init();
void DRIVER_step();
void DRIVER_chdir();
void DRIVER_forward();
void DRIVER_backward();
void DRIVER_stepdiv(uint8_t div);

/***********************
 * PORTB
 ***********************/
#define D8		PB0
#define D9		PB1
#define D10		PB2
#define D11		PB3
#define D12		PB4

void PORTB_init();