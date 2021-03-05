#include "system.h"
#include "cfg.h"

//VARS
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
cfg_var_t dv_speed =
{
	.name = "dv_speed",
	.s_name = "ds",
	.value = 700 //steps per second
};
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

// Переменная конца списка переменных 0_о
cfg_var_t z_endvar = 
{
	.name = "z_endvar",
	.s_name = "zv",
	.value = -1
};

cfg_var_t *cfg_vars[] = 
{
	&mv_start,
	&mv_end,
	&mv_count,
	&dv_speed,
	&fv_num,
	&fv_step,

	&z_endvar,
};

// Количество переменных
int cfg_numofvars()
{
	int num = 0;
	for(; strcmp("z_endvar", cfg_vars[num]->name); num++) {}
	return num;
}

// Установка значения перменной
void cfg_set(char *name, uint16_t *value)
{
	int i = 0;
	int num = cfg_numofvars();
	for(; i < num; i++)
	{
		if(!strcmp(cfg_vars[i]->name, name))
		{
			cfg_vars[i]->value = *value;
			break;
		}
	}
}

// Значение переменной
uint16_t cfg_search(char *name)
{
	int i = 0;
	int num = cfg_numofvars();
	for(; i < num; i++)
	{
		if(!strcmp(cfg_vars[i]->name, name))
		{
			return cfg_vars[i]->value;
		}
	}
}