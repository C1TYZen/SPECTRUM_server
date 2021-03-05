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
	uint16_t steps = abs(mv_end.value - mv_start.value) * div;
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
		_delay_ms(1.4);
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

void df_step(sys_code16_t value)
{
	DRIVER_step(value);
}

/****************
 * CONFIG
 ****************/
void cf_set(sys_code16_t value)
{
	char str[16];
	char name[16];
	uint16_t val = 0;
	USART_println("var name:");
	USART_scanln(str, 16);
	str_parse(str, name, 16, &val);
	cfg_set(name, &val);
}

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
	char str[16];
	uint16_t dv_div = DRIVER_setdiv(8);
	sprintf(str, "steps: %d", \
		((uint16_t)(abs(mv_end.value - mv_start.value)) * (uint16_t)dv_div));
	USART_println(str);
}

/****************
 * SYS
 ****************/
sys_funk_t sys_funk[] =
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
	{
		.name 	= "df_step",
		.id 	= CMD_DS,
		.funk 	= &df_step
	},
	//Config________________
	{
		.name 	= "cf_set",
		.id 	= CMD_CS,
		.funk 	= &cf_set
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
	return sizeof(sys_funk) / sizeof(sys_funk_t);
}

void sys_mesure(uint16_t steps, uint16_t m_count, uint16_t speed)
{
	_delay_ms(2);
	USART_flush();
	uint16_t mesure = 0;
	uint16_t msg = 0;
	uint8_t i = 0;

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
		if((msg == CMD_DI) || (steps < 1))
			break;
		
		//Walking
		DRIVER_step();
		steps--;
		_delay_ms(1.4);
		//1.4 mc подобрано потом, кровью и индийскими сусликами аутсорсерами
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

/****************
 * MAIN
 ****************/
int main()
{
	uint16_t cmd = 0;

	sys_init();
	
	while(1) {
		cmd = USART_get();

		char str[16];
		int i = 0;
		int num = sys_numoffunks();
		for(; i < num; i++)
		{
			if(cmd == sys_funk[i].id)
			{
				sys_funk[i].funk(0);
				//USART_println("correct");
				break;
			}
		}
		cmd = 0;
		USART_flush();
	}

	return 0;
}