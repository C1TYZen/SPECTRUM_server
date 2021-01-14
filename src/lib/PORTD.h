/***********************
 * PORTD
 ***********************/
void PORTD_init();

uint8_t PORTD_getpin(int pin);
void PORTD_writepin(int pin, int bit);
void PORTD_switchpin(int pin);
void PORTD_pinmod(int pin, int mod);
uint8_t PORTD_test();