/** Funksiýalar bilen işleýän maglumatlar
**/
#include <stdlib.h>
#include <string.h>

#include "../cmds.h"
#include "../main/inf.h"
#include "fn_call.h"
#include "../translator_to_c/includes.h"
#include "../main/glob.h"
#include "../error.h"
#include "../paren/types.h"
#include "../fns.h"

int FN_CALL_TYPE_NUM = 0;

/**
 * Ülňi yglan etmek komandany saýgarýar
**/
int is_cmd_fn_call(command *cmd)
{
    if (cmd->items_num>CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type] || !cmd->items_num)
	{
		// Komandany savgaryp bolmady
		return 0;
	}

    if (cmd->items_num)
    {
       command_item *fci = get_cmd_item(cmd->items,0);
        if (fci->type==PAREN_ITEM && fci->paren.type!=PAREN_TYPE_DEF_TYPE)
            fn_call_cmd_mod(cmd, 0);
        else
            return 0;
    }

    if (cmd->items_num==2)
    {
        command_item *sci = get_cmd_item(cmd->items,1);
        if (sci->type==TOKEN_ITEM && sci->tok.potentional_types[0].type_class==TOK_CLASS_IDENT)
            fn_call_cmd_mod(cmd, 1);
        else
        {
            return 0;
        }
    }
    else if (cmd->items_num>2)
        return 0;
    return 1;
}


// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
void fn_call_cmd_mod(command *cmd, int item_num)
{
    if (item_num==0)
    {
        cmd->cmd_class = CMD_CLASS_FN_CALL;
        cmd->cmd_type  = FN_CALL_TYPE_NUM;
        cmd->is_compl  = 0;
        cmd->parenthesis = 1;
    }
    else if (item_num==1)
    {
        cmd->is_compl  = 1;
    }
}

// Funksiça çagyryş komandasynyn semantikasy
int semantic_cmd_fn_call(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    func *f = NULL;
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    // IKINJI BIRLIK BARLANÝAR
    // IDENTIFIKATOR BOLMALY, FUNKSIÝALARYŇ SANAWYNDA DUŞMALY
    // CONTINUE1
    //debug_cmd ( cmd );
    command_item *sci = get_cmd_item(cmd->items,1);

    if (!is_fn_exist(sci->tok.potentional_types[0].value))
    {
        CUR_PART = 7;
        print_err(CODE7_UNKNOWN_FNS_CALLED, &sci->tok);
        return 0;
    }
    else
        f = fn_get_by_name(sci->tok.potentional_types[0].value);

    // BIRINJI IDENTIFIKATOR BARLANÝAR
    //      Skobka bolmaly. Içinde islendik möçberde
    //          identifikator bolmaly, ýöne bir ýerde öň yglan edilmeli. tipi hökman harpl bolmaly
    //          harpl bolmaly
    //          funksiýa çagyrylyş bolmaly
    //          Ýa-da skobkanyň içi boş hem bolmaly.
    command_item *fci = get_cmd_item(cmd->items,0);
    if ( !check_fn_args( f->args_num, f->args, &fci->paren ) )
    {
        print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &sci->tok);
    }
    CUR_PART = prev_part;

    // ŞERT #1: Funksiýanyň prototipiniň yglan edilen faýly,
    //          funksiýanyň çagyrylan kodyna inklud etmeli faýllaryň sanawyna goşulmaly
    file_incs *fi;
    // 1) Eger içine inklud etmeli faýla öň faýl goşulmadyk bolsa
    //     a) Içine inklud etmeli faýl taýynlanýar
    if ((sci->tok.inf_file_num+1)>INCLUDES_NUM)
    {
        // 1.a)
        fi = includes_add_new();
    }
    else
    {
        // 1.b)
        fi = &INCLUDES[sci->tok.inf_file_num];
    }

    // 2)
    includes_file_add_include(fi, f->c_lib);

    return 1;
}

int fn_call_cmd_return_type(command *cmd, int *return_class, int *ret_type)
{
    *return_class = TOK_CLASS_UNDEFINED;
    return 0;
}


/** Çagyrylan funksiýanyň kabul edýän argumentleriniň, funksiýanyň garaşýan argument tiplerini gabat gelýänini barlaýar

    \argn  - funksiýanyň garaşýan argumentleriniň sany

    \args  - funksiýanyň garaşýan argumentleri

    \paren - funksiýa çagyrylynda berlen maglumatlar
**/
int check_fn_args(int argn, int args_num, parenthesis *paren)
{
    func_arg *args = get_fn_args(args_num);
    parenthesis_elem *paren_elems = get_paren_elems(paren->elems);
    // funksiýa çagyrylanda berlen maglumatlaryň barlananynyň nomeri
    int checked_arg_pos = -1, j, ret_class = -1, ret_type = -1;

    if (argn==FN_ARGS_MULTIPLE)
    {
        for(j=0; j<paren->elems_num; ++j)
        {
            if (paren_elems[j].type==TOKEN_ITEM)
            {
                //token_type *tt = &paren_elems[j].tok.potentional_types[0];
                return_tok_type(&paren_elems[j].tok, &ret_class, &ret_type);
            }
            else if (paren_elems[j].type==CMD_ITEM)
                CMD_RETURN_TYPE[paren_elems[j].cmd.cmd_class][paren_elems[j].cmd.cmd_type](&paren_elems[j].cmd, &ret_class, &ret_type);
            else if (paren_elems[j].type==PAREN_ITEM)
                PAREN_RETURN_TYPE[paren_elems[j].paren.type-1](&paren_elems[j].paren, &ret_class, &ret_type);

            if(ret_class==args[0].type_class && ret_type==args[0].type_num)
            {
                checked_arg_pos = j;
            }
            else
            {
                CUR_PART = 7;
                if(paren_elems[j].type==TOKEN_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &paren_elems[j].tok);
                else if(paren_elems[j].type==CMD_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_last_token(&paren_elems[j].cmd));
                else if(paren_elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_parens_last_token(&paren_elems[j].paren));
            }
        }
    }
    else
    {
        for(j=0; j<argn; j++)
        {
            set_def_type_alias_const_data( &args[j].type_class, &args[j].type_num );

            if (paren_elems[j].type==TOKEN_ITEM)
            {
                //token_type *tt = &paren_elems[j].tok.potentional_types[0];
                return_tok_type(&paren_elems[j].tok, &ret_class, &ret_type);
            }
            else if(paren_elems[j].type==CMD_ITEM)
                CMD_RETURN_TYPE[paren_elems[j].cmd.cmd_class][paren_elems[j].cmd.cmd_type](&paren_elems[j].cmd, &ret_class, &ret_type);
            else if(paren_elems[j].type==PAREN_ITEM)
                PAREN_RETURN_TYPE[paren_elems[j].paren.type-1](&paren_elems[j].paren, &ret_class, &ret_type);

            if (ret_class==args[j].type_class && ret_type==args[j].type_num)
                checked_arg_pos = j;
            else
            {
                CUR_PART = 7;
                if(paren_elems[j].type==TOKEN_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, &paren_elems[j].tok);
                else if(paren_elems[j].type==CMD_ITEM)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_last_token(&paren_elems[j].cmd));
                else if(paren_elems[j].type==TOKEN_ITEM && ret_class==TOK_CLASS_UNKNOWN)
                    print_err(CODE7_FN_ARG_TYPES_NOT_MATCH, (token *)inf_get_parens_last_token(&paren_elems[j].paren));
            }
        }
    }

    // Hemme argumentler barlanan bolmaly
    if ((checked_arg_pos+1)!=paren->elems_num)
    {
        CUR_PART = 7;
        print_err(CODE7_FN_ARG_NUMES_NOT_MATCH, &inf_tok);
    }
    return 1;
}




/** Faýla degişli kody C koda ýazýar
**/
void cmd_fn_call_c_code(command *cmd, wchar_t **line, int *llen)
{
    wchar_t *paren_o = L"(",
            *paren_c = L")";
    // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
    // Eger birinji ülňi identifikator bolsa, özi alynýar.
    command_item *sci = get_cmd_item(cmd->items,1);
    func *func_params = fn_get_by_name( sci->tok.potentional_types[0].value );

    command_item *fci = get_cmd_item(cmd->items,0);
    /** Inside of C code, the HARPL type arguments of functions
        should be moved to memory. Because they could be changed inside of functions */

    int return_class = func_params->return_class,
        return_type  = func_params->return_type;
    set_def_type_alias_const_data( &return_class, &return_type );

    if ( return_class==TOK_CLASS_CONST_DATA && return_type==STRING_CONST_DATA_TOK_NUM )
    {
        /// Temporary variable to hold converted string in C code
        wchar_t *var_name = NULL;
        int len = 0;
        wcsadd_on_heap( &var_name, &len, C_CODE_FN_CALLS_STR );
        wchar_t var_num[10] = { 0 };
        wsprintfW(var_num, L"%d", C_CODE_FN_CALLS_NUM );
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
        wcsadd_on_heap( line, llen, L" ( " );
        wcsadd_on_heap( line, llen, var_name );
        wcsadd_on_heap( line, llen, L" = " );
        free( var_name );
    }

    wcsadd_on_heap( line, llen, func_params->c_name );

    wcsadd_on_heap( line, llen, paren_o );

    /** Ýasaljak koddaky funksiýa çagyrylmagynda, argumentleriň C dilindäki analogyny edýär.

    \funcParams - argumentleri ýerleşdirmegiň analogy
    \paren      - C dilindäki analogyny ýasamaly argumentleriň saklaýjysy

    \return     - taýyn C dilindäki analogy
    **/

    func_params->make_args_string( &fci->paren, line, llen );

    // Funksiýany ýapýar
    wcsadd_on_heap( line, llen, paren_c );

    if ( return_class==TOK_CLASS_CONST_DATA && return_type==STRING_CONST_DATA_TOK_NUM )
    {
        wchar_t *c = L")";
        wcsadd_on_heap( line, llen, c );
    }

    ++C_CODE_FN_CALLS_NUM;
}


int cmd_fn_call_return_type(command *cmd, int *return_class, int *return_type)
{
    if (cmd->items_num<2)
    {
        *return_class = TOK_CLASS_UNKNOWN;
        *return_type  = 0;
        return 1;
    }
    command_item *sci = get_cmd_item(cmd->items,1);
    wchar_t *n = sci->tok.potentional_types[0].value;
    if(!is_fn_exist(n))
    {
        *return_class = TOK_CLASS_UNDEFINED;
        return 0;
    }
    func *f = fn_get_by_name(n);
    *return_class = f->return_class;
    *return_type  = f->return_type;

    return 1;
}

void trans_to_c_move_fn_arg_to_malloc( token *tok )
{
    /// Temporary variable to hold argument in C code
    wchar_t *var_name = NULL;
    int len = 0;
    wcsadd_on_heap( &var_name, &len, C_CODE_FN_HARPL_ARG_STR );
    wchar_t var_num[10] = { 0 };
    wsprintfW(var_num, L"%d", C_CODE_FN_HARPL_ARG_NUM );
    wcsadd_on_heap( &var_name, &len, var_num );

    /// Variable initializing
    wchar_t *var_init = NULL;
    int init_len = 0;
    /** Move argument string to allocated memory */
    wcsadd_on_heap( &var_init, &init_len, L" wchar_t * " );
    wcsadd_on_heap( &var_init, &init_len, var_name );
    wcsadd_on_heap( &var_init, &init_len, L" = malloc( ( wcslen( " );
    tok_string_c_code( tok, &var_init, &init_len );
    //debug_token( tok );
    //wcsadd_on_heap( &var_init, &init_len, val );
    wcsadd_on_heap( &var_init, &init_len, L" )+1 )*sizeof( wchar_t ) );\n " );
    wcsadd_on_heap( &var_init, &init_len, L" wcscpy( " );
    wcsadd_on_heap( &var_init, &init_len, var_name );
    wcsadd_on_heap( &var_init, &init_len, L" , " );
    tok_string_c_code( tok, &var_init, &init_len );
    //wcsadd_on_heap( &var_init, &init_len, val );
    wcsadd_on_heap( &var_init, &init_len, L" );\n " );

    wcsadd_on_heap( &CMD_C_CODE_PRE, &CMD_C_CODE_PRE_LEN, var_init );

    free( var_init );

    /** add it to remove at the end of block variable's list */
    if ( is_inside_loc_fn() )
    {
        add_string_to_free_list( var_name, &C_CODE_DEF_FN_FREE_VARS_NUM[ CUR_LOC_FN ], &C_CODE_DEF_FN_FREE_VARS[ CUR_LOC_FN ] );
    }
    else
    {
        add_string_to_free_list( var_name, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );
    }

    free( var_name );

    ++C_CODE_FN_HARPL_ARG_NUM;
}

void trans_to_c_write_last_fn_arg( wchar_t **mem, int *memlen, wchar_t *add )
{
    /// Temporary variable to hold argument in C code
    wcsadd_on_heap( mem, memlen, C_CODE_FN_HARPL_ARG_STR );
    wchar_t var_num[10] = { 0 };
    wsprintfW(var_num, L"%d", C_CODE_FN_HARPL_ARG_NUM-1 );  // Last argument
    wcsadd_on_heap( mem, memlen, var_num );
}

