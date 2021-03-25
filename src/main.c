#include "lib/system.h"
#include "lib/cfg.h"
#include "lib/ADC.h"
#include "lib/DRIVER.h"
#include "lib/USART.h"

#define EN_PIN		D2
#define MS1_PIN		D3
#define MS2_PIN		D4
#define MS3_PIN		D5
#define STEP_PIN	D6
#define DIR_PIN		D7
#define ENDER_PIN	D8
#define ROTOR_PIN	D9
#define SCMD_PIN	D10

typedef unsigned short sys_code16_t;

typedef struct sys_funk_s
{
	char* name;
	uint16_t id;
	void (*funk)(sys_code16_t);
} sys_funk_t;

/****************
 * MESURE
 ****************/
void mf_begin(sys_code16_t value)
{
	DRIVER_moveto(mv_start.value);

	uint16_t div = DRIVER_setdiv(8);
	div_step_uint32_t x0 = mv_start.value;
	div_step_uint32_t x1 = mv_end.value;
	div_step_uint32_t steps = (x1 - x0) * div;
	uint16_t speed = (1000.f / (float)dv_speed.value) / 0.1;
	DRIVER_backward();
	sys_mesure(steps, mv_count.value, speed);
}

/****************
 * DRIVER
 ****************/
void df_tozero(sys_code16_t value)
{
	DRIVER_forward();	
	DRIVER_setdiv(1);
	USART_println("Moving...");
	while(ports_getpin(ENDER_PIN))
	{
		_delay_ms(2);
		USART_print("Ender");
		DRIVER_step();
	}
	USART_println("END  ");

	DRIVER_backward();
	DRIVER_setdiv(8);
	while(ports_getpin(ROTOR_PIN))
	{
		_delay_ms(1.4);
		USART_print("Rotor");
		DRIVER_step();
	}
	USART_println("ZERO");
	DRIVER_reset();
}

/****************
 * CONFIG
 ****************/
//Функция для терминала (хз работает или нет)
void cft_set(sys_code16_t value)
{
	char str[16];
	char name[16];
	uint16_t val = 0;
	//USART_println("var name:");
	USART_scanln(str, 16);
	str_parse(str, name, 16, &val);
	cfgt_set(name, &val);
	sprintf(str, "id: %s val: %d", name, val);
	USART_println(str);
}

//Функция для клиентской проги (используется эта)
void cfc_set(sys_code16_t value)
{
	char str[64];
	uint16_t id = value;
	uint16_t val = USART_get();
	cfgc_set(&id, &val);
	sprintf(str, "(%x) %d", id, val);
	USART_println(str);
}

/****************
 * FILTER
 ****************/
void ff_a(sys_code16_t value) { sys_filter(1); }
void ff_b(sys_code16_t value) { sys_filter(2); }
void ff_c(sys_code16_t value) { sys_filter(3); }
void ff_d(sys_code16_t value) { sys_filter(4); }
void ff_e(sys_code16_t value) { sys_filter(5); }
void ff_f(sys_code16_t value) { sys_filter(6); }
void ff_z(sys_code16_t value) { sys_filter(0); }

/****************
 * TEST
 ****************/
void tf_conn(sys_code16_t value)
{
	//USART_println("OLD WORLD NEWS");
	USART_println("**Connected**");
}

void tf_pin(sys_code16_t value)
{
	if(ports_getpin(ENDER_PIN))
		USART_println("Ender 1");
	else
		USART_println("Ender 0");
	if(ports_getpin(ROTOR_PIN))
		USART_println("Rotor 1");
	else
		USART_println("Rotor 0");
}

void tf_test(sys_code16_t value)
{
	sys_filter(1);
}

/****************
 * SYS
 ****************/
sys_funk_t sys_cfunk[] =
{
	//Mesure________________
	{
		.name 	= "mf_begin",
		.id 	= CMD_MB,
		.funk 	= &mf_begin
	},
	//Driver________________
	{
		.name 	= "df_tozero",
		.id 	= CMD_DZ,
		.funk 	= &df_tozero
	},
	//Config________________
	{
		.name 	= "cf_set",
		.id 	= CMD_CS,
		.funk 	= &cfc_set
	},
	//Filter_______________
	{
		.name = "ff_a",
		.id = CMD_FA,
		.funk = &ff_a
	},
	{
		.name = "ff_b",
		.id = CMD_FB,
		.funk = &ff_b
	},
	{
		.name = "ff_c",
		.id = CMD_FC,
		.funk = &ff_c
	},
	{
		.name = "ff_d",
		.id = CMD_FD,
		.funk = &ff_d
	},
	{
		.name = "ff_e",
		.id = CMD_FE,
		.funk = &ff_e
	},
	{
		.name = "ff_f",
		.id = CMD_FF,
		.funk = &ff_f
	},
	{
		.name = "ff_z",
		.id = CMD_FZ,
		.funk = &ff_z
	},
	//Tests________________
	{
		.name 	= "tf_conn",
		.id 	= CMD_CC,
		.funk 	= &tf_conn
	},
	{
		.name 	= "tf_pin",
		.id 	= CMD_TP,
		.funk 	= &tf_pin
	},
	{
		.name 	= "tf_test",
		.id 	= CMD_TF,
		.funk 	= &tf_test
	}
};

void sys_init()
{
	ports_init();
	ADC_init();
	USART_init();
	DRIVER_init(EN_PIN, MS1_PIN, MS2_PIN, MS3_PIN, STEP_PIN, DIR_PIN);

	ports_pinmod(SCMD_PIN, PINMOD_OUT);
	ports_writepin(SCMD_PIN, 0);
}

int sys_numoffunks()
{
	return sizeof(sys_cfunk) / sizeof(sys_funk_t);
}

void sys_mesure(div_step_uint32_t steps, uint16_t m_count, uint16_t speed)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	uint8_t i = 0;
	uint16_t delay_counter = 0;

	while(1)
	{
		//Mesuring
		mesure = 0;
		for(i = m_count; i > 0; i--)
			mesure += ADC_read(0);
		mesure /= m_count;
		USART_send(mesure);

		//Thinking
		msg = USART_getmessage();
		if((msg == CMD_MI) || (steps < 1))
			break;

		//testing
		if((fv_step.value != 0) && (DRIVER_getpos() == fv_step.value))
			sys_filter(fv_num.value);
		//testing
		
		//Walking
		DRIVER_step();
		steps--;
		//_delay_ms(1.4);
		//1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
		for(delay_counter = 0; delay_counter < speed; delay_counter++)
			_delay_ms(0.1);
	}
	USART_flush();
	USART_send(CMD_MS);
}

void sys_filter(int n) //НЕ ЛЕЗЬ, РАБОТАЕТ!
{
	int k = 0;
	ports_writepin(SCMD_PIN, 1);

	for (k = 1; k <= 7; k++)
	{
		if (n > 0) 
		{
			ports_writepin(SCMD_PIN, 1);
			n--;
		} 
		else 
		{
			ports_writepin(SCMD_PIN, 0);
		}
		_delay_ms(50);
	}
	ports_pinmod(SCMD_PIN, PINMOD_IN);
	_delay_ms(50);
	while(ports_getpin(SCMD_PIN) == 0) {}
	USART_println("OK");
	ports_pinmod(SCMD_PIN, PINMOD_OUT);
	ports_writepin(SCMD_PIN, 0);
}

void sys_dummy_filter(int n)
{ 
	_delay_ms(1000);
}

/****************
 * MAIN
 ****************/
int main()
{
	sys_init();

	uint16_t cmd = 0;
	int i = 0;
	int num = sys_numoffunks();

	while(1) {
		cmd = USART_get();

		for(i = 0; i < num; i++)
		{
			if(cmd == sys_cfunk[i].id)
			{
				sys_cfunk[i].funk(0);
				//USART_println("correct");
				break;
			}

			if((cmd == (*cfg_var_list[i]).id) && (i < cfg_numofvars()))
			{
				cfc_set(cmd);
				//USART_println("HERE");
				break;
			}
		}
		cmd = 0;
		USART_flush();
	}

	return 0;
}