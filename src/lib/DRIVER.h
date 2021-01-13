/***********************
 * DRIVER
 ***********************/
#define EN		PD2
#define MS1		PD3
#define MS2		PD4
#define MS3		PD5
#define STEP 	PD6
#define DIR 	PD7

#define DRIVER_PORT PORTD
#define DRIVER_DDR	DDRD

#define DRIVER_DEFAULT_DIV 0
#define DRIVER_BCK 1
#define DIRVER_FWD -1

void DRIVER_init();

void DRIVER_step();
void DRIVER_chdir();
void DRIVER_forward();
void DRIVER_backward();

int8_t DRIVER_setdir(int8_t dir);
int8_t DRIVER_setdiv(uint8_t div);

void DRIVER_mvf(uint32_t trg);
void DRIVER_mvb(uint32_t trg);
void DRIVER_moveto(uint32_t r1);

uint32_t DRIVER_info();
void DRIVER_reset();
