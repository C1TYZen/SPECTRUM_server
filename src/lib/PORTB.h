/***********************
 * PORTB
 ***********************/
void PORTB_init();

uint8_t PORTB_getpin(int);
void PORTB_writepin(int, int);
void PORTB_switchpin(int);
void PORTB_pinmod(int, int);
uint8_t PORTB_test();