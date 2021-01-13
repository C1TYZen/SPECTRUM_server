/***********************
 * PORTB
 ***********************/
void PORTB_init();

uint8_t PORTB_getpin(int pin);
void PORTB_writepin(int pin, int bit);

void PORTB_pinmod(int pin, int mod);

uint8_t PORTB_test();