#include "system.h"
#include "PORTB.h"
#include "PORTD.h"

void ports_init()
{
	PORTB_init();
	PORTD_init();
}

uint8_t ports_getpin(int pin)
{
	
}

void ports_writepin(int pin, int bit)
{
	if(bit == 1)
		PORTD |= (1<<pin);
	if(bit == 0)
		PORTD &= ~(1<<pin);
}

void ports_pinmod(int pin, int mod)
{
	if(mod == 1)
		DDRD |= (1<<pin);
	if(mod == 0)
		DDRD &= ~(1<<pin);
}