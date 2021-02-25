#include "system.h"
#include "ADC.h"
#include "DRIVER.h"
#include "PORTD.h"
#include "PORTB.h"
#include "USART.h"

#define EN_PIN		D2
#define MS1_PIN		D3
#define MS2_PIN		D4
#define MS3_PIN		D5
#define STEP_PIN	D6
#define DIR_PIN		D7
#define ENDER_PIN	D8
#define ROTOR_PIN	D9
#define SCMD_PIN	D10

void ports_init()
{
	PORTD_init();
	PORTB_init();
}

uint8_t ports_getpin(int pin)
{
	if(pin<8)
		PORTD_getpin(pin);
	else
		PORTB_getpin(pin-8);
}

void ports_writepin(int pin, int bit)
{
	if(pin<8)
		PORTD_writepin(pin, bit);
	else
		PORTB_writepin(pin-8, bit);
}

void ports_switch(int pin)
{
	if(pin<8)
		PORTD_switchpin(pin);
	else
		PORTB_switchpin(pin-8);
}

void ports_pinmod(int pin, int mod)
{
	if(pin<8)
		PORTD_pinmod(pin, mod);
	else
		PORTB_pinmod(pin-8, mod);
}

/***********************
 * BUILTINS
 ***********************/
//m
cfg_var_t mv_start =
{
	.name = "mv_start",
	.s_name = "ms",
	.value = 0
};
cfg_var_t mv_end =
{
	.name = "mv_end",
	.s_name = "me",
	.value = 100
};
cfg_var_t mv_count = 
{
	.name = "mv_count",
	.s_name = "mc",
	.value = 1
};

//d
cfg_var_t dv_div = 
{
	.name = "dv_div",
	.s_name = "dd",
	.value = 1
};

//f
cfg_var_t fv_num = 
{
	.name = "fv_num",
	.s_name = "fn",
	.value = 1
};
cfg_var_t fv_step = 
{
	.name = "fv_step",
	.s_name = "fs",
	.value = 0
};

cfg_funk_t bltn_funk[] =
{
	//Mesure
	{
		.name 	= "mf_begin",
		.s_name = "mb",
		.funk 	= &mf_begin
	},
	{
		.name 	= "mf_stop",
		.s_name = "ms",
		.funk 	= &mf_stop
	},
	//Driver
	{
		.name 	= "df_back",
		.s_name = "db",
		.funk 	= &df_back
	},
	{
		.name 	= "df_forward",
		.s_name = "df",
		.funk 	= &df_forward
	},
	{
		.name 	= "df_calibrate",
		.s_name = "dc",
		.funk 	= &df_calibrate
	},
	{
		.name 	= "df_position",
		.s_name = "dp",
		.funk 	= &df_position
	},
	{
		.name 	= "df_step",
		.s_name = "ds",
		.funk 	= &df_step
	},
	{
		.name 	= "df_interrupt",
		.s_name = "di",
		.funk 	= &df_interrupt
	},
	//Tests
	{
		.name 	= "tf_conn",
		.s_name = "cc",
		.funk 	= &tf_conn
	},
	{
		.name 	= "tf_mesure",
		.s_name = "tm",
		.funk 	= &tf_mesure
	},
	{
		.name 	= "tf_pin",
		.s_name = "tp",
		.funk 	= &tf_pin
	},
	{
		.name 	= "tf_test",
		.s_name = "tt",
		.funk 	= &tf_test
	}
};

//Mesure
void mf_begin(uint16_t value)
{
	DRIVER_moveto(mv_start.value);

	dv_div.value = DRIVER_setdiv(dv_div.value);
	DRIVER_backward();
	uint32_t steps = \
		(uint32_t)(abs(mv_end.value - mv_start.value)) * (uint32_t)dv_div.value;
	mesure(steps, mv_count.value);
}
void mf_stop(uint16_t value) {}

//Driver
void df_back(uint16_t value)
{
	DRIVER_backward();
}
void df_forward(uint16_t value)
{
	DRIVER_forward();
}
void df_calibrate(uint16_t value)
{
	DRIVER_forward();	
	DRIVER_setdiv(1);
	USART_println("Callibrating...");
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
	USART_println("CALLIBRATED");
	DRIVER_reset();
}
void df_position(uint16_t value) {}
void df_step(uint16_t value)
{
	DRIVER_step(value);
}
void df_interrupt(uint16_t value) {}

//Tests
void tf_conn(uint16_t value)
{
	//USART_println("OLD WORLD NEWS");
	USART_println("**Connected**");
}
void tf_mesure(uint16_t value) {}
void tf_pin(uint16_t value)
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
void tf_test(uint16_t value)
{
	char str[16];
	int num = sizeof(bltn_funk)/sizeof(cfg_funk_t);
	sprintf(str, "%d builtin funks____", num);
	USART_println(str);
	int i = 0;
	for(; i < num; i++)
	{
		sprintf(str, "%s", bltn_funk[i].name);
		USART_println(str);
	}
}

void cfg_set(char *name, char *value) {}