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
        if (paren->elems[0].type==PAREN_ITEM)
            return PAREN_RETURN_TYPE[paren->elems[0].paren.type](&paren->elems[0].paren, ret_class, ret_num);
        else if (paren->elems[0].type==CMD_ITEM)
            return CMD_RETURN_TYPE[paren->elems[0].cmd.cmd_class][paren->elems[0].cmd.cmd_type](&paren->elems[0].cmd, ret_class, ret_num);
        else if (paren->elems[0].type==TOKEN_ITEM)
            return TOK_RETURN_TYPE[paren->elems[0].tok.potentional_types[0].type_class][paren->elems[0].tok.potentional_types[0].type_num](&paren->elems[0].tok, ret_class, ret_num);
    }
    *ret_class = TOK_CLASS_UNDEFINED;
    return 0;
}

