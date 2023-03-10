/** Funksiýalar bilen işleýär*/
#include <stdlib.h>
#include <string.h>

#include "../main/glob.h"
#include "../cmd/user_def_type.h"
#include "fn.h"
#include "fn_helpers.h"
#include "../fns.h"
#include "../translator_to_c.h"

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
int is_fn_exist(wchar_t *fn_name)
{
    int i, len;
    for (i=0; i<FUNCS_NUM; ++i)
    {
        len = wcslen(fn_name)>wcslen(FUNCS[i].name)?wcslen(fn_name):wcslen(FUNCS[i].name);
        if (wcsncmp(fn_name, FUNCS[i].name, len)==0)
            return 1;
    }
    return 0;
}



/** Funksiýanyň maglumatlaryny gaýtarýar. Maglumatlaryny gaýtarmaly funksiýa ady boýunça alynýar
    Bu funksiýa hökmän is_fn_exist() funksiýa şowly bolan soň çagyrylmaly

    \fnName - maglumatlary alynmaly funksiýanyň ady
**/
func *fn_get_by_name(wchar_t *fn_name)
{
    int i, len;
    for (i=0; i<FUNCS_NUM; ++i)
    {
        len = wcslen(fn_name)>wcslen(FUNCS[i].name)?wcslen(fn_name):wcslen(FUNCS[i].name);
        if (wcsncmp(fn_name, FUNCS[i].name, len)==0)
        {
            return &FUNCS[i];
        }
    }
    // TODO : Print_err: FUNC NOT FOUND
    return &FUNCS[FUNCS_NUM-1];
}


void make_user_def_fn_args(parenthesis *paren, wchar_t **line, int *llen)
{
    wchar_t *space = L" ",
            *dot_space = L", ";
    int i, elem, elem_type_class, elem_type;

    wcsadd_on_heap( line, llen, space );

    parenthesis_elem *p_es = get_paren_elems(paren->elems);
    for(i=0; i<paren->elems_num; ++i)
    {
        get_parenthesis_elem_type( &p_es[ i ], &elem, &elem_type_class, &elem_type );
        if ( elem==TOKEN_ITEM && elem_type_class==TOK_CLASS_CONST_DATA && elem_type==STRING_CONST_DATA_TOK_NUM )
        {
            trans_to_c_move_fn_arg_to_malloc( &p_es[ i ].tok );
            trans_to_c_write_last_fn_arg( line, llen );
        }
        else
        {
            write_paren_item_c_code( &p_es[i], line, llen );
        }

        if (i<paren->elems_num-1)
        {
            wcsadd_on_heap( line, llen, dot_space );
        }

    }
}


