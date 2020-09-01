#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

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
void DRIVER_stepdiv(uint8_t div);