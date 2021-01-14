/***********************
 * DRIVER
 ***********************/
#include "system.h"
#include "DRIVER.h"

uint32_t DRIVER_position = 0;
int8_t DRIVER_dir = 1;
uint8_t DRIVER_div = 1;
driver_port_cfg cfg;

/**
 * Инициализация библиотеки двигателя
 **/
void DRIVER_init(int en, int ms1, int ms2, int ms3, int step, int dir)
{
	cfg.en 		= en;
	cfg.ms1 	= ms1;
	cfg.ms2 	= ms2;
	cfg.ms3 	= ms3;
	cfg.step 	= step;
	cfg.dir 	= dir;

	ports_pinmod(cfg.en, PINMOD_OUT);
	ports_pinmod(cfg.ms1, PINMOD_OUT);
	ports_pinmod(cfg.ms2, PINMOD_OUT);
	ports_pinmod(cfg.ms3, PINMOD_OUT);
	ports_pinmod(cfg.step, PINMOD_OUT);
	ports_pinmod(cfg.dir, PINMOD_OUT);

	ports_writepin(cfg.dir, 1);
	ports_writepin(cfg.ms1, 0);
	ports_writepin(cfg.ms2, 0);
	ports_writepin(cfg.ms3, 0);
	ports_writepin(cfg.ms1, 1);

	// DRIVER_DDR |= (1<<EN)|(1<<MS1)|(1<<MS2)|(1<<MS3)|(1<<STEP)|(1<<DIR);
	// DRIVER_PORT |= (1<<DIR); // Вперед
	// DRIVER_PORT &= ~(8<<MS1);
	// DRIVER_PORT |= (1<<MS1);
}

// Шаг двигателя
void DRIVER_step()
{
	// DRIVER_PORT ^= (1<<STEP);
	ports_switch(cfg.step);
	DRIVER_position += DRIVER_dir * (8/DRIVER_div);
}

// Смотреть со стороны приборной панели
// Направление двигателя вперед
void DRIVER_forward()
{
	// DRIVER_PORT |= (1<<DIR);
	ports_writepin(cfg.dir, 1);
	DRIVER_dir = -1;
}

// Направление двигателя назад
void DRIVER_backward()
{
	// DRIVER_PORT &= ~(1<<DIR);
	ports_writepin(cfg.dir, 0);
	DRIVER_dir = 1;
}
//////////////////////////////////////

// Смотреть со стороны крепления ротора
// Направление вращения вправо
void DRIVER_dexter()
{
	// DRIVER_PORT &= ~(1<<DIR);
	ports_writepin(cfg.dir, 0);
	DRIVER_dir = 1;
}

// Направление врщения влево
void DRIVER_sinister()
{
	// DRIVER_PORT |= (1<<DIR);
	ports_writepin(cfg.dir, 1);
	DRIVER_dir = -1;
}
//////////////////////////////////////

int8_t DRIVER_setdir(int8_t dir)
{
	if(dir == 1) //back
	{
		// DRIVER_PORT &= ~(1<<DIR);
		ports_writepin(cfg.dir, 0);
		DRIVER_dir = 1;
		return 1;
	}
	else if(dir == -1) //fwd
	{
		// DRIVER_PORT |= (1<<DIR);
		ports_writepin(cfg.dir, 1);
		DRIVER_dir = -1;
		return 0;
	}
	else
	{
		// DRIVER_PORT &= ~(1<<DIR);
		ports_writepin(cfg.dir, 0);
		DRIVER_dir = 1;
		return -1;
	}
}

// Установка делителя шага
int8_t DRIVER_setdiv(uint8_t div)
{
	// 1    2    4    8
	// 1    1/2  1/4  1/8
	// 1/16 не работает
	if(div == 1)
	{
		// DRIVER_PORT &= ~(1<<MS1);
		// DRIVER_PORT &= ~(1<<MS2);
		// DRIVER_PORT &= ~(1<<MS3);
		ports_writepin(cfg.ms1, 0);
		ports_writepin(cfg.ms2, 0);
		ports_writepin(cfg.ms3, 0);
		DRIVER_div = 1;
	}
	else if(div == 2)
	{
		// DRIVER_PORT |= (1<<MS1);
		// DRIVER_PORT &= ~(1<<MS2);
		// DRIVER_PORT &= ~(1<<MS3);
		ports_writepin(cfg.ms1, 1);
		ports_writepin(cfg.ms2, 0);
		ports_writepin(cfg.ms3, 0);
		DRIVER_div = 2;
	}
	else if(div == 4)
	{
		// DRIVER_PORT &= ~(1<<MS1);
		// DRIVER_PORT |= (1<<MS2);
		// DRIVER_PORT &= ~(1<<MS3);
		ports_writepin(cfg.ms1, 0);
		ports_writepin(cfg.ms2, 1);
		ports_writepin(cfg.ms3, 0);
		DRIVER_div = 4;
	}
	else if(div == 8)
	{
		// DRIVER_PORT |= (1<<MS1);
		// DRIVER_PORT |= (1<<MS2);
		// DRIVER_PORT &= ~(1<<MS3);
		ports_writepin(cfg.ms1, 1);
		ports_writepin(cfg.ms2, 1);
		ports_writepin(cfg.ms3, 0);
		DRIVER_div = 8;
	}
	else
	{
		// DRIVER_PORT &= ~(1<<MS1);
		// DRIVER_PORT &= ~(1<<MS2);
		// DRIVER_PORT &= ~(1<<MS3);
		ports_writepin(cfg.ms1, 0);
		ports_writepin(cfg.ms2, 0);
		ports_writepin(cfg.ms3, 0);
		DRIVER_div = 1;
		div = 1;
	}
	return div;
}

void DRIVER_mvf(uint32_t trg)
{
	DRIVER_setdir(-1);
	while(DRIVER_position > trg)
	{
		DRIVER_setdiv(1);
		if((DRIVER_position - trg) < 8)
			DRIVER_setdiv(2);
		if((DRIVER_position - trg) < 4)
			DRIVER_setdiv(4);
		if((DRIVER_position - trg) < 2)
			DRIVER_setdiv(8);

		DRIVER_step();
		_delay_ms(1.4);
	}
}

void DRIVER_mvb(uint32_t trg)
{
	DRIVER_setdir(1);
	while(DRIVER_position < trg)
	{
		DRIVER_setdiv(1);
		if((trg - DRIVER_position) < 8)
			DRIVER_setdiv(2);
		if((trg - DRIVER_position) < 4)
			DRIVER_setdiv(4);
		if((trg - DRIVER_position) < 2)
			DRIVER_setdiv(8);

		DRIVER_step();
		_delay_ms(1.4);
	}
}

void DRIVER_moveto(uint32_t start)
{
	start *= 8;
	if(DRIVER_position < start)
	{
		DRIVER_mvb(start);
	}
	else if(DRIVER_position > start)
	{
		DRIVER_mvf(start);
	}
	else
		return;
}

uint32_t DRIVER_info()
{
	return DRIVER_position;
}

void DRIVER_reset()
{
	DRIVER_position = 0;
}