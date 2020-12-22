/***********************
 * PORTB
 ***********************/
#include "system.h"
#include "PORTB.h"

void PORTB_init()
{
	DDRB &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2) & ~(1<<PB3) & ~(1<<PB4);
	PORTB &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2) & ~(1<<PB3) & ~(1<<PB4);
}

uint8_t PORTB_getpin(int pin)
{
	if ((PINB & (1<<pin)) == 0)
		return 0;
	else
		return 1;
}

void PORTB_writepin(int pin, int bit)
{
	if(bit == 1)
		PORTB |= (1<<pin);
	if(bit == 0)
		PORTB &= ~(1<<pin);
}

void PORTB_pinmod(int pin, int mod)
{
	if(mod == 1)
		DDRB |= (1<<pin);
	if(mod == 0)
		DDRB &= ~(1<<pin);
}