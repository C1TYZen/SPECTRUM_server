// #define EN		PD2
// #define MS1		PD3
// #define MS2		PD4
// #define MS3		PD5
// #define STEP 	PD6
// #define DIR 		PD7

// #define DRIVER_PORT 	PORTD
// #define DRIVER_DDR	DDRD

#define DRIVER_DEFAULT_DIV 	 0
#define DRIVER_BCK 			 1
#define DIRVER_FWD 			-1

typedef struct driver_port_cfg_s
{
	int en;
	int ms1;
	int ms2;
	int ms3;
	int step;
	int dir;
} driver_port_cfg_t;

// Тип для хранения количества полных шагов
typedef uint16_t fullstep_pos16b;
// Тип для хранения количества шагов с делителем 8
typedef uint32_t div8step_pos32b;

void DRIVER_init(int en, int ms1, int ms2, int ms3, int step, int dir);

void DRIVER_step();
void DRIVER_chdir();
void DRIVER_forward();
void DRIVER_backward();

int8_t DRIVER_setdir(int8_t);
int8_t DRIVER_setdiv(uint8_t);

void DRIVER_mvf(fullstep_pos16b);
void DRIVER_mvb(fullstep_pos16b);
void DRIVER_moveto(div8step_pos32b);

div8step_pos32b DRIVER_getpos();
void DRIVER_reset();
