#include "system.h"
#include "PORTD.h"
#include "PORTB.h"

void ports_init()
{
	PORTD_init();
	PORTB_init();
}

uint8_t ports_getpin(int pin)
{
	if(pin<8)
		PORTD_getpin(pin);
	else
		PORTB_getpin(pin-8);
}

void ports_writepin(int pin, int bit)
{
	if(pin<8)
		PORTD_writepin(pin, bit);
	else
		PORTB_writepin(pin-8, bit);
}

void ports_switch(int pin)
{
	if(pin<8)
		PORTD_switchpin(pin);
	else
		PORTB_switchpin(pin-8);
}

void ports_pinmod(int pin, int mod)
{
	if(pin<8)
		PORTD_pinmod(pin, mod);
	else
		PORTB_pinmod(pin-8, mod);
}