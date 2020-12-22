/***********************
 * PORTB
 ***********************/
#define D8		PB0
#define D9		PB1
#define D10		PB2
#define D11		PB3
#define D12		PB4

void PORTB_init();

uint8_t PORTB_getpin(int pin);
void PORTB_writepin(int pin, int bit);

void PORTB_pinmod(int pin, int mod);

uint8_t PORTB_test();