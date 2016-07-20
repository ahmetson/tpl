#include "block_inside.h"
#include "../token/token_types/block_inside.h"
#include "../token/helpers.h"
#include "../main/glob.h"
#include "../fns/fn_helpers.h"
#include "../error.h"
#include "../translator_to_c.h"
#include "../fns.h"
#include "../main/inf.h"


int     is_cmd_block_inside( command *cmd )
{
    if ( cmd->items_num!=2 )
        return 0;

    int type_class = -1, type_num = -1;
    command_item *fci = get_cmd_item( cmd->items, 0 );
    if ( !(fci->type==TOKEN_ITEM && get_token_type_class( &fci->tok )==TOK_CLASS_VOID)  &&
        ( (fci->type==TOKEN_ITEM && !return_tok_type(&fci->tok, &type_class, &type_num )) ||
        (fci->type==CMD_ITEM &&   !CMD_RETURN_TYPE[fci->cmd.cmd_class][fci->cmd.cmd_type](&fci->cmd, &type_class, &type_num )) ||
        (fci->type==PAREN_ITEM && !PAREN_RETURN_TYPE[fci->paren.type](&fci->paren, &type_class, &type_num ) ) ) )
        return 0;

    fci = get_cmd_item( cmd->items, 1 );
    if ( fci->type==TOKEN_ITEM && get_token_type_class( &fci->tok )==TOK_CLASS_BLOCK_INSIDE &&
        get_token_type( &fci->tok )==TOK_CLASS_BLOCK_INSIDE_FN_RETURN  )
        cmd_block_inside_return_mod( cmd );
    else
        return 0;

    return 1;
}


void cmd_block_inside_return_mod( command *cmd )
{
    cmd->cmd_class = CMD_CLASS_BLOCK_INSIDE;
    cmd->cmd_type = TOK_CLASS_BLOCK_INSIDE_FN_RETURN;
    cmd->is_compl = 1;
    cmd->ns = LOCAL;
    cmd->parenthesis = 0;
    cmd->is_compl = 1;
}

int semantic_cmd_block_inside( command *cmd )
{
    if ( !is_inside_fn() )
    {
        CUR_PART = 7;
        print_err(CODE7_RETURN_CMD_CAN_BE_USED_INSIDE_FUNCTION, (token *)inf_get_last_token(cmd));
    }

    int type_class = -1, type_num = -1,
        return_class = -1, return_num = -1;

    return_class = TMP_FUNC.return_class;
    return_num = TMP_FUNC.return_type;
    set_def_type_alias_const_data( &return_class, &return_num );

    command_item *fci = get_cmd_item( cmd->items, 0 );
    if ( fci->type==TOKEN_ITEM && get_token_type_class( &fci->tok )==TOK_CLASS_VOID )
    {
        if ( return_class==TOK_CLASS_VOID )
            return 1;
        else
        {
            CUR_PART = 7;
            print_err( CODE7_RETURN_CMD_TYPE_NOT_MATCH, (token *)inf_get_last_token( cmd ) );
        }
    }

    else if ( ( fci->type==TOKEN_ITEM && return_tok_type(&fci->tok, &type_class, &type_num ) ) || (
        (fci->type==CMD_ITEM &&   CMD_RETURN_TYPE[fci->cmd.cmd_class][fci->cmd.cmd_type](&fci->cmd, &type_class, &type_num )) ||
        (fci->type==PAREN_ITEM && PAREN_RETURN_TYPE[fci->paren.type](&fci->paren, &type_class, &type_num ) )  ) )
    {
        if ( !( return_class==type_class && return_num==type_num ) )
        {
            CUR_PART = 7;
            print_err( CODE7_RETURN_CMD_TYPE_NOT_MATCH, (token *)inf_get_last_token( cmd ) );
        }
    }
    else
    {
        CUR_PART = 7;
        print_err( CODE7_RETURN_CMD_TYPE_NOT_MATCH, (token *)inf_get_last_token( cmd ) );
    }


    return 1;
}
/** Semantika:

    Eger funksiyanyn icinde bolmasa
        'Yza' komandy diňe funksiýanyň içinde ulanyp bolýar.

    Funksiýanyň gaýtarýan maglumatynyň tipi alynýar.
    Yza komandanyň gaýtarýan maglumatynyň tipi alynýar.

    Eger funksiýanyň gaýtarýan maglumatynyň tipi we yza komandanyň gaýtarýan maglumatynyň tipi gabat gelmese
        Yza komandasy nädogry tipde maglumaty yza gaýtarýar diýen ýalňyşlyk görkezilýär.

*/

/** C kody ýazylanda:

    Eger yza komanda hiç hili maglumaty gaýtarmaýan bolsa
        Birinji maglumat ýazylanok.

    Gaýtarylmaly maglumatyň tipi alynýar.
    Eger tipi HARPL bolsa
        @" wchar_t *_tpl_str1 = ( [return_data] );
           wchar_t *_tpl_str2 = malloc( ( wcslen( _tpl_str1 )+1 )*sizeof( wchar_t ) );
           wçsnçpy( _tpl_str2, _tpl_str1, ( wcslen( _tpl_str1 )+1 ) );
           return _tpl_str2 ; ";
           ----- diýip Ç koda ýazylýar.
    Ýogsa
        @" return ( [return_data] );"
           ----- diýip Ç koda ýazylýar.

 */
void cmd_block_inside_fn_return_c_code(command *cmd, wchar_t **l, int *llen)
{
    wchar_t *o = L" ( ",
            *c = L" ) ";

    command_item *data_item = get_cmd_item( cmd->items, 0 );

    if ( data_item->type==TOKEN_ITEM && get_token_type_class( &data_item->tok )==TOK_CLASS_CONST_DATA
         && get_token_type( &data_item->tok )==STRING_CONST_DATA_TOK_NUM )
    {
        wchar_t *pre = L"  wchar_t *_tpl_str1 =  ";
        wcsadd_on_heap( l, llen, pre );
        wcsadd_on_heap( l, llen, o );
        write_cmd_item_c_code( data_item, l, llen );
        wcsadd_on_heap( l, llen, c );
        write_terminator( l, llen );
        wchar_t *after =
         L"wchar_t *_tpl_str2 = malloc( ( wcslen( _tpl_str1 )+1 )*sizeof( wchar_t ) );\n\
           wcsncpy( _tpl_str2, _tpl_str1, ( wcslen( _tpl_str1 )+1 ) );\n\
           return _tpl_str2 ; ";
        wcsadd_on_heap( l, llen, after );
    }
    else
    {
        wcsadd_on_heap( l, llen, TOK_CLASS_BLOCK_INSIDE_CHARS[ TOK_CLASS_BLOCK_INSIDE_FN_RETURN ][ 1 ] );

        if ( data_item->type==TOKEN_ITEM && get_token_type_class( &data_item->tok )==TOK_CLASS_VOID )
            return;

        wcsadd_on_heap( l, llen, o );
        write_cmd_item_c_code( data_item, l, llen );
        wcsadd_on_heap( l, llen, c );
    }
}



int cmd_block_inside_return_type(command *cmd, int *type_class, int *type_num)
{
    if (!cmd->is_compl)
        return 0;

    command_item *fci = get_cmd_item(cmd->items, 0);

    if ((fci->type==TOKEN_ITEM && return_tok_type(&fci->tok, type_class, type_num ) ) || (
        (fci->type==CMD_ITEM &&   CMD_RETURN_TYPE[fci->cmd.cmd_class][fci->cmd.cmd_type](&fci->cmd, type_class, type_num )) ||
        (fci->type==PAREN_ITEM && PAREN_RETURN_TYPE[fci->paren.type](&fci->paren, type_class, type_num ) ) ) )
    {
        set_def_type_alias_const_data(type_class, type_num);
        return 1;
    }

    *type_class = -1;
    return 0;
}


