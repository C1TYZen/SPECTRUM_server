/***********************
 * DRIVER
 ***********************/
#define DRIVER_DEFAULT_DIV 0
#define DRIVER_BCK 1
#define DIRVER_FWD -1

typedef struct
{
	int EN;
	int MS1;
	int MS2;
	int MS3;
	int STEP;
	int DIR;
} driver_config;

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
