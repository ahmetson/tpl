/** Funksiýalar bilen işleýär
 *
**/
#include <stdlib.h>
#include <string.h>

#include "../main/glob.h"
#include "../cmd/user_def_type.h"
#include "fn.h"
#include "fn_helpers.h"

/** Funksiýe yglan edilende, içinde bolup bilýan komandalar:
    Klasy we tipini görkezmek üçin, sanaw iki basgançakly bolmaly.

    HEMME YGLAN EDILÝÄN MAGLUMATLAR LOKAL BOLMALY*/

int unvalid_fn_inside_cmds[][2] = {
    {CONST_CMD_CLASS_UTYPE, 0},
    {CONST_CMD_CLASS_FN_DEF,  CMD_UTYPE_DEF_TYPE}
};

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
    if (f.args_num)
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


void make_user_def_fn_args(parenthesis *paren, char **line, int *llen)
{
    char *args = NULL;
    int args_len = strlen(" ")+1;
    args = realloc(args, args_len);
    strncpy(args, " ", strlen(" ")+1);
    int i;

    for(i=0; i<paren->elems_num; ++i)
    {
        parenthesis_elem *p_es = get_paren_elems(paren->elems);

        if (p_es[i].type==TOKEN_ITEM)
            TOK_GET_C_CODE[p_es[i].tok.potentional_types[0].type_class]
                                [p_es[i].tok.potentional_types[0].type_num](&p_es[i].tok, &args, &args_len);
        else if(p_es[i].type==CMD_ITEM)
            CMD_GET_C_CODE[p_es[i].cmd.cmd_class]
                                [p_es[i].cmd.cmd_type](&p_es[i].cmd, &args, &args_len);

        if (i<paren->elems_num-1)
        {
            args_len+=strlen(", ");
            args = realloc(args, args_len);
            strncat(args, ", ", strlen(", "));
        }

        //else if(p_es[i].type==PAREN_ITEM)
         //   tmp = PAREN_GET_C_CODE[p_es[i].paren.type](&p_es[i].paren);
    }
    *llen += args_len;
    *line = realloc(*line, *llen);
    strncat(*line, args, strlen(args));

    free(args);
}


