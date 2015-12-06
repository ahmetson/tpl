/** Funksiýalar bilen işleýär
 *
**/
#include <stdlib.h>
#include <string.h>

#include "../main/glob.h"
#include "fn.h"
int FUNC_CLASS_3RD_PARTY = 0;
int FUNC_TYPE_STD        = 1;

int FUNC_CLASS_USER_DEF  = 1;

void add_func_to_list(func f)
{
    ++FUNCS_NUM;
    long size = sizeof(*FUNCS)*FUNCS_NUM;
    FUNCS = realloc(FUNCS, size);
    FUNCS[FUNCS_NUM-1] = f;

    // Argumentler üçin ýadyň ýerini ulaltmaly
    size = sizeof(*FUNC_ARGS)*FUNCS_NUM;
    FUNC_ARGS = realloc(FUNC_ARGS, size);
    if (f.args_num<=1)
        size = sizeof(**FUNC_ARGS);
    else
        size = sizeof(**FUNC_ARGS)*f.args_num;
    FUNC_ARGS[FUNCS_NUM-1] = NULL;

    FUNC_ARGS[FUNCS_NUM-1] = realloc(FUNC_ARGS[FUNCS_NUM-1], size);

    FUNCS[FUNCS_NUM-1].args = FUNCS_NUM-1;
}

func_arg *get_fn_args(int args_num)
{
    return FUNC_ARGS[args_num];
}

void add_arg_to_last_func(func_arg arg, int pos)
{
    FUNC_ARGS[FUNCS_NUM-1][pos] = arg;

}


/** Funksiýanyň ady häzir TPL'e belli bolan funksiýalaryň arasynda gözlenilýär.

    \fnName - gözlemeli funksiýanyň ady
**/
int is_fn_exist(char *fn_name)
{
    int i, len;
    for (i=0; i<FUNCS_NUM; ++i)
    {
        len = strlen(fn_name)>strlen(FUNCS[i].name)?strlen(fn_name):strlen(FUNCS[i].name);
        if (strncmp(fn_name, FUNCS[i].name, len)==0)
            return 1;
    }
    return 0;
}



/** Funksiýanyň maglumatlaryny gaýtarýar. Maglumatlaryny gaýtarmaly funksiýa ady boýunça alynýar
    Bu funksiýa hökmän is_fn_exist() funksiýa şowly bolan soň çagyrylmaly

    \fnName - maglumatlary alynmaly funksiýanyň ady
**/
func *fn_get_by_name(char *fn_name)
{
    int i, len;
    for (i=0; i<FUNCS_NUM; ++i)
    {
        len = strlen(fn_name)>strlen(FUNCS[i].name)?strlen(fn_name):strlen(FUNCS[i].name);
        if (strncmp(fn_name, FUNCS[i].name, len)==0)
        {
            return &FUNCS[i];
        }
    }
    // TODO : Print_err: FUNC NOT FOUND
    return &FUNCS[FUNCS_NUM-1];
}



