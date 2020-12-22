/***********************
 * PORTD
 ***********************/
#define D2 		PD2
#define D3 		PD3
#define D4 		PD4
#define D5 		PD5
#define D6 		PD6
#define D7 		PD7

void PORTD_init();

uint8_t PORTD_getpin(int pin);
void PORTD_writepin(int pin, int bit);

void PORTD_pinmod(int pin, int mod);

uint8_t PORTD_test();