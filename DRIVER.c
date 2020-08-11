#include "SERVER.h"

/***********************
 * DRIVER
 ***********************/
#define DRIVER_DDR  DDRD
#define DRIVER_PORT PORTD
#define STEP  PD2
#define DIR   PD3
 
void DRIVER_init() {
	DRIVER_DDR |= (1<<STEP)|(1<<DIR);
	DRIVER_PORT |= (1<<STEP);
	DRIVER_PORT |= (1<<DIR);
}

void DRIVER_step() {
	DRIVER_PORT ^= (1<<STEP);
}

void DRIVER_chdir() {
	DRIVER_PORT ^= (1<<DIR);
}