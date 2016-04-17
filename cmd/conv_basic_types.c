#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conv_basic_types.h"
#include "../paren/types.h"
#include "../main/glob.h"
#include "../main/inf.h"
#include "../translator_to_c.h"
#include "../fns.h"
#include "../error.h"

wchar_t *conv_c_codes[CMD_CONV_TYPES][3] = {
    {L"_tpl_conv_if(",  L"_tpl_conv_ic(",  L"_tpl_conv_ia("},
    {L"_tpl_conv_fi(",  L"_tpl_conv_fc(",  L"_tpl_conv_fa("},
    {L"_tpl_conv_ci(",  L"_tpl_conv_cf(",  L"_tpl_conv_ca("},
    {L"_tpl_conv_ai(",  L"_tpl_conv_af(",  L"_tpl_conv_ac("}
};

int is_cmd_conv_basic_type(command *cmd)
{
    if (!cmd->items_num)
        return 0;
    command_item *fci = get_cmd_item(cmd->items, 0);
    if (fci->type==PAREN_ITEM && fci->paren.type==PAREN_TYPE_DEF_TYPE)
    {
        cmd_conv_basic_type_mod(cmd, 0);
    }
    else
    {
        return 0;
    }

    if (cmd->items_num>1)
    {
        cmd_conv_basic_type_mod(cmd, 1);

        if (cmd->items_num>2)
        {
            return 0;
        }
    }
    return 1;
}


void cmd_conv_basic_type_mod(command *cmd, int item_num)
{
    if (!item_num)
    {
        cmd->is_compl = 0;
        cmd->cmd_class=CMD_CLASS_CONV_BASIC_TYPES;
        cmd->cmd_type =CMD_CLASS_CONV_BASIC_TYPE_NUM;
    }
    else
    {
        cmd->is_compl = 1;
    }
}


/** IÇINI SOŇ GOÝARYS*/
int semantic_cmd_conv_basic_type(command *cmd)
{
    command_item *fci = get_cmd_item(cmd->items, 0);
    parenthesis_elem *pe = get_paren_elems(fci->paren.elems_num);

    command_item *sci = get_cmd_item(cmd->items, 1);
    if ( sci->type==CMD_ITEM )
        check_semantics( &sci->cmd );
    else if ( sci->type==PAREN_ITEM && sci->paren.elems_num==1 )
    {
        parenthesis_elem *pi = get_paren_elems( sci->paren.elems );
        if ( pi[ 0 ].type==CMD_ITEM )
            check_semantics( &pi[ 0 ].cmd );
    }
    int sclass = -1, stype = -1;
    if (sci->type==CMD_ITEM && CMD_RETURN_TYPE[sci->cmd.cmd_class][sci->cmd.cmd_type](&sci->cmd, &sclass, &stype) ||
        sci->type==TOKEN_ITEM && TOK_RETURN_TYPE[sci->tok.type_class][sci->tok.potentional_types[0].type_num](&sci->tok, &sclass, &stype)||
        sci->type==PAREN_ITEM && PAREN_RETURN_TYPE[sci->paren.type](&sci->paren, &sclass, &stype))
    {
        if (sclass!=TOK_CLASS_CONST_DATA)
        {
            CUR_PART = 7;
            print_err(CODE7_MUST_BE_DEF_TYPE, (token *)inf_get_parens_last_token(&fci->paren));
        }
    }
    else
    {
        CUR_PART = 7;
        print_err(CODE7_MUST_RETURN_TYPE, (token *)inf_get_parens_last_token(&fci->paren));
    }

    return 1;
}



int cmd_arr_conv_basic_return_type(command *cmd, int *type_class, int *type_num)
{
    if (!cmd->is_compl)
        return 0;

    command_item *fci = get_cmd_item(cmd->items, 0);
    parenthesis_elem *pe = get_paren_elems(fci->paren.elems);
    *type_class = TOK_CLASS_DEF_TYPE;
    *type_num   = pe[0].tok.potentional_types[0].type_num;

    set_def_type_alias_const_data(type_class, type_num);
    return 1;
}



/** Faýla degişli kody C koda ýazýar **/
void cmd_conv_basic_type_c_code(command *cmd, wchar_t **l, int *llen)
{
    int f, s;
    get_conv_type_num(cmd, &f, &s);
    wcsadd_on_heap( l, llen, conv_c_codes[f][s] );

    command_item *e2 = get_cmd_item(cmd->items,1);
    cmd_item_get_c_code( e2, l, llen );

    wchar_t *c = L")";
    wcsadd_on_heap( l, llen, c );
}


void get_conv_type_num(command *cmd, int *rclass, int *rtype)
{
    command_item *fci = get_cmd_item(cmd->items, 0);
    parenthesis_elem *pe = get_paren_elems(fci->paren.elems);

    int fclass = pe[0].tok.type_class,
        ftype  = pe[0].tok.potentional_types[0].type_num;
    if (fclass==TOK_CLASS_DEF_TYPE)
        set_def_type_alias_const_data(&fclass, &ftype);


    command_item *sci = get_cmd_item(cmd->items, 1);
    int sclass = -1, stype = -1;
    if (sci->type==CMD_ITEM && CMD_RETURN_TYPE[sci->cmd.cmd_class][sci->cmd.cmd_type](&sci->cmd, &sclass, &stype) ||
        sci->type==TOKEN_ITEM && TOK_RETURN_TYPE[sci->tok.type_class][sci->tok.potentional_types[0].type_num](&sci->tok, &sclass, &stype)||
        sci->type==PAREN_ITEM && PAREN_RETURN_TYPE[sci->paren.type](&sci->paren, &sclass, &stype));
    if (sclass==TOK_CLASS_DEF_TYPE)
        set_def_type_alias_const_data(&sclass, &stype);

    if (stype==CHAR_CONST_DATA_TOK_NUM) /// HARP
    {
        *rclass = 2;
        if (ftype==INT_CONST_DATA_TOK_NUM)
            *rtype = 0;
        else if (ftype==FLOAT_CONST_DATA_TOK_NUM)
            *rtype = 1;
        else if (ftype==STRING_CONST_DATA_TOK_NUM)
            *rtype = 2;
    }
    else if (stype==INT_CONST_DATA_TOK_NUM) /// SAN
    {
        *rclass = 0;
        if (ftype==CHAR_CONST_DATA_TOK_NUM)
            *rtype = 1;
        else if (ftype==FLOAT_CONST_DATA_TOK_NUM)
            *rtype = 0;
        else if (ftype==STRING_CONST_DATA_TOK_NUM)
            *rtype = 2;
    }
    else if (stype==FLOAT_CONST_DATA_TOK_NUM) /// DROB
    {
        *rclass = 1;
        if (ftype==CHAR_CONST_DATA_TOK_NUM)
            *rtype = 1;
        else if (ftype==INT_CONST_DATA_TOK_NUM)
            *rtype = 0;
        else if (ftype==STRING_CONST_DATA_TOK_NUM)
            *rtype = 2;
    }
    else if (stype==STRING_CONST_DATA_TOK_NUM) /// HARPL
    {
        *rclass = 3;
        if (ftype==CHAR_CONST_DATA_TOK_NUM)
            *rtype = 2;
        else if (ftype==INT_CONST_DATA_TOK_NUM)
            *rtype = 0;
        else if (ftype==FLOAT_CONST_DATA_TOK_NUM)
            *rtype = 1;
    }

}
