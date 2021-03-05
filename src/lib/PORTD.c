/****************
 * PORTD
 ****************/
#include "system.h"
#include "PORTD.h"

void PORTD_init()
{
	DDRD &= ~(1<<PD0) & ~(1<<PD1) & ~(1<<PD2) & ~(1<<PD3) & ~(1<<PD4);
	PORTD &= ~(1<<PD0) & ~(1<<PD1) & ~(1<<PD2) & ~(1<<PD3) & ~(1<<PD4);
}

uint8_t PORTD_getpin(int pin)
{
	if ((PIND & (1<<pin)) == 0)
		return 0;
	else
		return 1;
}

void PORTD_writepin(int pin, int bit)
{
	if(bit == 1)
		PORTD |= (1<<pin);
	if(bit == 0)
		PORTD &= ~(1<<pin);
}

void PORTD_switchpin(int pin)
{
	PORTD ^= (1<<pin);
}

void PORTD_pinmod(int pin, int mod)
{
	if(mod == 1)
		DDRD |= (1<<pin);
	if(mod == 0)
		DDRD &= ~(1<<pin);
}