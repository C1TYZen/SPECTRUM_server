/***********************
 * DRIVER
 ***********************/
#include "SERVER.h"

#define EN		PD2
#define MS1		PD3
#define MS2		PD4
#define MS3		PD5
#define STEP 	PD6
#define DIR 	PD7
 
void DRIVER_init() {
	DDRD |= (1<<EN)|(1<<MS1)|(1<<MS2)|(1<<MS3)|(1<<STEP)|(1<<DIR);
	PORTD |= (1<<DIR);
	PORTD &= ~(8<<MS1);
	//0    1    2    3    8
	//1    1/2  1/4  1/8  1/16
	PORTD |= (1<<MS1);

}

void DRIVER_step() {
	PORTD ^= (1<<STEP);
}

void DRIVER_chdir() {
	PORTD ^= (1<<DIR);
}

void DRIVER_stepdiv(uint8_t div) {
	//0    1    2    3    8
	//1    1/2  1/4  1/8  1/16
	if((div == 0)||(div == 1)||(div == 2)||(div == 3)||(div == 8)) {
		PORTD &= ~(8<<MS1);
		PORTD |= (div<<MS1);
	} else {
		PORTD &= ~(8<<MS1);
	}
}