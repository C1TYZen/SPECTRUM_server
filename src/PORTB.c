/***********************
 * PORTB
 ***********************/
#include "SERVER.h"

#define ENDER D8
#define ROTOR D9

/*
 * Подтяжка к питанию должна быть выключена (в PORTB должен быть записан 0)
 * В противном случае пины не реагируют на оптопару. Черт знает почему.
 * Пины не подключенные пины, биты которых старше чем бит пина, который опущен в ноль
 * также опускаются. Необходимо, что бы все используемые пины были подключены.
 */
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

uint8_t PORTB_test()
{
	if(PORTB_getpin(ENDER))
		return 81;
	else
		return 80;
	if(PORTB_getpin(ROTOR))
		return 91;
	else
		return 90;
}