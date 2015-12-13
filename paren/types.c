/** Ýaýlaryň tipleri bilen işleýän funksiýalar we maglumatlar
**/
#include "types.h"
#include "../parenthesis.h"

int PAREN_TYPE_EMPTY    = 0;
int PAREN_TYPE_FNS_ARGS = 1;

char *PAREN_TYPES_WORDS[] = {
    "Boş",
    "Funksiýanyň argumentleri"
};

int (*PAREN_TYPES[])(parenthesis *paren) = {
    paren_type_is_empty,
    paren_type_is_fns_args
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
    if (paren->elems_num)
    {
        paren->type = PAREN_TYPE_FNS_ARGS;
        return 1;
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
            return TOK_RETURN_TYPE[p_es[0].tok.potentional_types[0].type_class][p_es[0].tok.potentional_types[0].type_num](&p_es[0].tok, ret_class, ret_num);
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
            TOK_RETURN_TYPE[p_es[0].tok.potentional_types[0].type_class][p_es[0].tok.potentional_types[0].type_num](&p_es[0].tok, &ret_class, &ret_num);
        if (ret_class==TOK_CLASS_CONST_DATA && ret_num==INT_CONST_DATA_TOK_NUM)
            continue;
        return 0;
    }
    return 1;
}


