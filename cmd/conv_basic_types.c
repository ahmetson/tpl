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


wchar_t *conv_c_codes[ CMD_CONV_TYPES ][ 3 ] = {
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
    if ( return_cmd_item_type( sci, &sclass, &stype ) )
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

    if ( is_return_string( f, s ) )
    {
        /// Temporary variable to hold converted string in C code
        wchar_t *var_name = NULL;
        int len = 0;
        wcsadd_on_heap( &var_name, &len, C_CODE_CONV_TMP_STR );
        wchar_t var_num[10] = { 0 };
        wsprintfW(var_num, L"%d", C_CODE_CONV_TMP_NUM );
        wcsadd_on_heap( &var_name, &len, var_num );

        /// Variable initializing
        wchar_t *var_init = NULL;
        int init_len = 0;
        wcsadd_on_heap( &var_init, &init_len, L" wchar_t * " );
        wcsadd_on_heap( &var_init, &init_len, var_name );
        wcsadd_on_heap( &var_init, &init_len, L" = NULL;\n " );

        wcsadd_on_heap( &CMD_C_CODE_PRE, &CMD_C_CODE_PRE_LEN, var_init );

        free( var_init );

        /// Variable finishizing
        wchar_t *var_finish = NULL;
        int finish_len = 0;

        wcsadd_on_heap( &var_finish, &finish_len, L" if ( " );
        wcsadd_on_heap( &var_finish, &finish_len, var_name );
        wcsadd_on_heap( &var_finish, &finish_len, L" != NULL)\n\t free( " );
        wcsadd_on_heap( &var_finish, &finish_len, var_name );
        wcsadd_on_heap( &var_finish, &finish_len, L" );\n " );

        wcsadd_on_heap( &CMD_C_CODE_AFTER, &CMD_C_CODE_AFTER_LEN, var_finish );

        free( var_finish );

        /// Add code: ( varname = [data] )
        wcsadd_on_heap( l, llen, L" ( " );
        wcsadd_on_heap( l, llen, var_name );
        wcsadd_on_heap( l, llen, L" = " );
        wcsadd_on_heap( l, llen, conv_c_codes[f][s] );
        command_item *e2 = get_cmd_item(cmd->items,1);
        write_cmd_item_c_code( e2, l, llen );
        wchar_t *c = L")";
        wcsadd_on_heap( l, llen, c );
        wcsadd_on_heap( l, llen, c );

        free( var_name );

    }
    else
    {
        wcsadd_on_heap( l, llen, conv_c_codes[f][s] );

        command_item *e2 = get_cmd_item(cmd->items,1);
        write_cmd_item_c_code( e2, l, llen );

        wchar_t *c = L")";
        wcsadd_on_heap( l, llen, c );
    }
}


/** Determines whether data type is string or not */
int is_return_string( int from, int to )
{
    if ( from<3 && to==2 )
        return 1;
    return 0;
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
    if ( return_cmd_item_type( sci, &sclass, &stype ) );
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
