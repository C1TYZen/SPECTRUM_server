#include <avr/io.h>
#include <avr/interrupt.h>
//#include <stdlib.h>
#include <util/delay.h>

//PORTB
#define D8		PB0
#define D9		PB1
#define D10		PB2
#define D11		PB3
#define D12		PB4

//commands
#define CMD_MB	25197
#define CMD_MC	25453
#define CMD_MR	29293
#define CMD_MS	29549

#define CMD_DC	25444
#define CMD_DD	25700
#define CMD_DS	29540
#define CMD_DV	30308
#define CMD_DF	26212
#define CMD_DB	25188
#define CMD_DI	26980

#define CMD_CC	25443
#define CMD_ST	29811
#define CMD_TP	28788

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
void DRIVER_move(long steps);
void DRIVER_chdir();
void DRIVER_forward();
void DRIVER_backward();
void DRIVER_stepdiv(uint8_t div);

/***********************
 * PORTB
 ***********************/
void PORTB_init();
