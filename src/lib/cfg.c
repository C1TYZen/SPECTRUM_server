#include "system.h"
#include "cfg.h"

//VARS
cfg_var_t mv_start =
{
	.name = "mv_start",
	.id = CVAR_MA,
	.value = 0
};
cfg_var_t mv_end =
{
	.name = "mv_end",
	.id = CVAR_MZ,
	.value = 100
};
cfg_var_t mv_count = 
{
	.name = "mv_count",
	.id = CVAR_MC,
	.value = 1
};
cfg_var_t dv_speed =
{
	.name = "dv_speed",
	.id = CVAR_DS,
	.value = 700 // шагов в секунду (700 ~ 1.4ms delay)
};
cfg_var_t fv_num = 
{
	.name = "fv_num",
	.id = CVAR_FN,
	.value = 1
};
cfg_var_t fv_step = 
{
	.name = "fv_step",
	.id = CVAR_FS,
	.value = 0
};

// Переменная конца списка переменных 0_о
cfg_var_t z_endvar = 
{
	.name = "z_endvar",
	.id = 0,
	.value = -1
};

cfg_var_t *cfg_var_list[] = 
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
	for(; strcmp("z_endvar", cfg_var_list[num]->name); num++) {}
	return num;
}

// Установка значения перменной
void cfgt_set(char *name, uint16_t *value)
{
	int i = 0;
	int num = cfg_numofvars();
	for(; i < num; i++)
	{
		if(!strcmp(cfg_var_list[i]->name, name))
		{
			cfg_var_list[i]->value = *value;
			break;
		}
	}
}

void cfgc_set(uint16_t *id, uint16_t *value)
{
	int i = 0;
	int num = cfg_numofvars();
	for(; i < num; i++)
	{
		if(cfg_var_list[i]->id == *id)
		{
			cfg_var_list[i]->value = *value;
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
		if(!strcmp(cfg_var_list[i]->name, name))
		{
			return cfg_var_list[i]->value;
		}
	}
}