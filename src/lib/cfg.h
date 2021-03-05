typedef struct cfg_var_s
{
	char* name;
	char* s_name; //Short name
	uint16_t value;
} cfg_var_t;

cfg_var_t mv_start;
cfg_var_t mv_end;
cfg_var_t mv_count;
cfg_var_t dv_speed;
cfg_var_t fv_num;
cfg_var_t fv_step;

// Нулевая переменная
cfg_var_t z_endvar;

int cfg_numofvars();
void cfg_set(char *name, uint16_t *value);
uint16_t cfg_search(char *name);