/** Ýaýlaryň tipleri bilen işleýän funksiýalar we maglumatlar*/
#include "types.h"
#include "../parenthesis.h"
#include "../cmd/array.h"

int PAREN_TYPE_EMPTY        = 0;
int PAREN_TYPE_FNS_ARGS     = 1;
int PAREN_TYPE_DEF_FNS_ARGS = 2;
int PAREN_TYPE_DEF_TYPE     = 3;


char *PAREN_TYPES_WORDS[] = {
    "Boş",
    "Funksiýanyň argumentleri",
    "Funksiýanyň argumentlerinipň yglan edilmegi"
    "Komandany başga tipe geçirýär"
};

int (*PAREN_TYPES[])(parenthesis *paren) = {
    paren_type_is_empty,
    paren_type_is_fns_args,
    paren_type_is_def_fns_args,
    paren_type_is_def_type
};

int paren_type_is_empty(parenthesis *paren)
{
    if (!paren->elems_num)
    {
        paren->type = PAREN_TYPE_EMPTY;
        return 1;
    }
    return 0;
}

int paren_type_is_fns_args(parenthesis *paren)
{
    if (paren_type_is_def_type(paren))
        return 0;
    if (paren->elems_num)
    {
        parenthesis_elem *pe = get_paren_elems(paren->elems);
        int i, tmp_c, tmp_t;
        for (i=0; i<paren->elems_num; ++i)
        {
            if (pe[i].type==CMD_ITEM && !CMD_RETURN_TYPE[pe[i].cmd.cmd_class][pe[i].cmd.cmd_type](&pe[i].cmd, &tmp_c, &tmp_t))
            {
                return 0;
            }
        }
        paren->type = PAREN_TYPE_FNS_ARGS;
        return 1;
    }
    return 0;
}

int paren_type_is_def_fns_args(parenthesis *paren)
{
    if (paren->elems_num)
    {
        parenthesis_elem *pe = get_paren_elems(paren->elems);
        int i, tmp_c, tmp_t, another = 0;
        for (i=0; i<paren->elems_num; ++i)
        {
            if (pe[i].type==CMD_ITEM && !CMD_RETURN_TYPE[pe[i].cmd.cmd_class][pe[i].cmd.cmd_type](&(pe[i].cmd), &tmp_c, &tmp_t))
            {
                if (pe[i].cmd.cmd_class!=CMD_CLASS_DEF_VAR &&
                    (pe[i].cmd.cmd_class!=CMD_CLASS_ARR && pe[i].cmd.cmd_type==CMD_CLASS_ARR_DEF))
                {
                    another = 1;
                }
            }
            else
            {
                another = 1;
            }
        }
        if (another)
            return 0;
        paren->type = PAREN_TYPE_DEF_FNS_ARGS;
        return 1;
    }
    return 0;
}

/** Funksiýanyň yglan edilende argumentleriniň maglumatlaryny görkezýär*/
int paren_type_is_def_type(parenthesis *paren)
{
    if (paren->elems_num==1)
    {
        parenthesis_elem *pe = get_paren_elems(paren->elems);
        //int tmp_c, tmp_t, another = 0;
        if (pe[0].type==TOKEN_ITEM && pe[0].tok.type_class==TOK_CLASS_DEF_TYPE)
        {
            paren->type = PAREN_TYPE_DEF_TYPE;
            return 1;
        }
    }
    return 0;
}

int empty_paren_return_type(parenthesis *paren, int *ret_class, int *ret_num)
{
    *ret_class = TOK_CLASS_UNDEFINED;
    return 0;
}

int paren_fns_args_return_type(parenthesis *paren, int *ret_class, int *ret_num)
{
    if (paren->elems_num==1)
    {
        parenthesis_elem *p_es = get_paren_elems(paren->elems);

        if (p_es[0].type==PAREN_ITEM)
            return PAREN_RETURN_TYPE[p_es[0].paren.type](&p_es[0].paren, ret_class, ret_num);
        else if (p_es[0].type==CMD_ITEM)
            return CMD_RETURN_TYPE[p_es[0].cmd.cmd_class][p_es[0].cmd.cmd_type](&p_es[0].cmd, ret_class, ret_num);
        else if (p_es[0].type==TOKEN_ITEM)
            return return_tok_type(&p_es[0].tok, ret_class, ret_num);
    }
    *ret_class = TOK_CLASS_UNDEFINED;
    return 0;
}

int is_param_item_int(parenthesis *par)
{
    int i, ret_class = -1, ret_num = -1;
    parenthesis_elem *p_es = get_paren_elems(par->elems);
    for (i=0; i<par->elems_num; ++i)
    {
        if (p_es[0].type==PAREN_ITEM)
            return is_param_item_int(&p_es[0].paren);
        else if (p_es[0].type==CMD_ITEM)
            CMD_RETURN_TYPE[p_es[0].cmd.cmd_class][p_es[0].cmd.cmd_type](&p_es[0].cmd, &ret_class, &ret_num);
        else if (p_es[0].type==TOKEN_ITEM)
            return_tok_type(&p_es[0].tok, &ret_class, &ret_num);
        if (ret_class==TOK_CLASS_CONST_DATA && ret_num==INT_CONST_DATA_TOK_NUM)
            continue;
        return 0;
    }
    return 1;
}


