/***********************
 * PORTD
 ***********************/
void PORTD_init();

uint8_t PORTD_getpin(int);
void PORTD_writepin(int, int);
void PORTD_switchpin(int);
void PORTD_pinmod(int, int);
uint8_t PORTD_test();