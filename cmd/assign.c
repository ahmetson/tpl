/**
 * Baglanma komandasy
 *
 * Baglanmanyn iki sany gornushi bar:
 * 	   1) Chepindaki ulna baglanma
 *     2) Sagyndaky ulna baglanma
**/
#include <stdlib.h>
#include <string.h>

#include "../main/glob.h"
#include "../error.h"
#include "def_var.h"
#include "assign.h"
#include "../semantic.h"
#include "../cmds.h"
#include "../main/inf.h"
#include "../fns.h"
#include "../fns/fn_helpers.h"
#include "../translator_to_c.h"

/**
 * Chepindaki ulna baglanma
 * Uch sany birliklerden durup bilyar.
 *
 * a) Birinji birlige uchunji birlikdaki maglumat gechirilyar:
 *    Birlik shu gornushlerde bolup bilyar:
 *     1) (komanda) ulni yglan etme               - taze ulni yglan edilende
 *                                                maglumat baglap bolyar.
 *     2) (token)   identifikator, ulninin ady    - on yglan edilen ulna
 *                                                maglumaty baglap bolyar.
 *
 * b) Ikinji birlik Baglanma token gornushinin chepine baglayan
 *    hilisinin tokeni bolmaly.
 *
 * c) Uchunji birlik birinji birlige baglanyan maglumat bolmaly.
 *    Birlik shu gornushlerden bolup bilyar:
 *    1) (token)  identifikator, ulninin ady    - on yglan edilen ulni taze
 *                                                ulninin maglumaty baglap bolyar.
**/

wchar_t *assign_right = L" _tpl_assign_right = ";
wchar_t *assign_left = L" _tpl_assign_left = ";


/** Berlen komandanyň 'assign' komandasymy ýa däldigini barlýar.
 *  Eger barlanýan komanda şowly tanalsa, onda komandanyň maglumatlary şuňa görä üýtgedilýär.
**/
int is_cmd_assign(command *cmd)
{

	if (cmd->items_num>CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type] || !cmd->items_num)
	{
		return 0;
	}
	int next_item_class=-1, next_item_type=-1;
	command_item *fci = get_cmd_item(cmd->items,0);
	// Birinji token ya ulni yglan etmek bolmaly ya identifikator.
	if (fci->type==TOKEN_ITEM &&
		fci->tok.type_class==TOK_CLASS_IDENT)
	{
	    assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
	}
	else if (fci->type==CMD_ITEM &&
		fci->cmd.cmd_class==CMD_CLASS_DEF_VAR)
	{
	    assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;

	}
	else if (fci->type==CMD_ITEM &&
            fci->cmd.cmd_class==CMD_CLASS_CALL_GLOB_VAR)
    {
        assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;
    }
    else if (fci->type==CMD_ITEM &&
            fci->cmd.cmd_class==CMD_CLASS_ARR && fci->cmd.cmd_type==CMD_CLASS_ARR_CON)
    {
        assign_cmd_mod(cmd, 0);
		next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;
    }
	/*else if (fci->type==TOKEN_ITEM &&
          fci->tok.type_class==TOK_CLASS_CONST_DATA)
	{

        //assign_cmd_mod(cmd, 0);
		//next_item_class=TOK_CLASS_ASSIGN;
		// Indiki token dine chepe tarap baglanma bolmaly
		//next_item_type= RIGHT_ASSIGN_TOK_NUM;
    }*/
    else
    {
		return 0;
	}

	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>1)
	{
	    command_item *sci = get_cmd_item(cmd->items,1);
		if (next_item_class>0 && sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_ASSIGN)
		{
			// Token dine chepe tarap baglanma bolmaly
			if (next_item_type>0 && !sci->tok.potentional_types[0].type_num==next_item_type)
			{
				return 0;
			}
			assign_cmd_mod(cmd, 1);
		}
		else
		{
			return 0;
		}
	}

	// Uchunji token identifikator bolmaly.
	if (cmd->items_num>2)
	{
	    command_item *tci = get_cmd_item(cmd->items,2); // tci - third command item
	    int ret_class = -1, ret_type = -1;
	    if((tci->type==TOKEN_ITEM &&
		   return_tok_type(&tci->tok, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED) ||
		   (tci->type==CMD_ITEM && CMD_RETURN_TYPE[tci->cmd.cmd_class][tci->cmd.cmd_type](&tci->cmd,&ret_class, &ret_type) &&
                ret_class!=TOK_CLASS_UNDEFINED) ||
           (tci->type==PAREN_ITEM &&
                PAREN_RETURN_TYPE[tci->paren.type](&tci->paren, &ret_class, &ret_type) && ret_class!=TOK_CLASS_UNDEFINED))
		{
		    assign_cmd_mod(cmd, 2);
		}
		else
		{
            return 0;
		}
	}

	return 1;
}


/**
 * Tokene görä komandanyň maglumatlaryny üýtgedýär.
 *
 * @cmd      - üýtgedilmeli komanda
 * @tokNum   - tokene görä üýtgedilmeli.
**/
void assign_cmd_mod(command *cmd, int tok_num)
{
	// Birinji birlik
	if (tok_num==0)
	{
	    command_item *fci = get_cmd_item(cmd->items, 0);
		cmd->cmd_class = CMD_CLASS_ASSIGN;
		cmd->is_compl = 0;

		// Eger birlik global ulni yglan etme bolsa
		// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
		if (!(fci->type==CMD_ITEM && is_glob_def_var_cmd(&fci->cmd)) )
		{
			cmd->ns = LOCAL;
		}
	}
	else if (tok_num==1)
	{
	    command_item *sci = get_cmd_item(cmd->items, 1);
		cmd->cmd_type = sci->tok.potentional_types[0].type_num-1;
		cmd->cmd_class = CMD_CLASS_ASSIGN;

		cmd->is_compl = 0;
	}
	else if (tok_num==2)
	{
		cmd->cmd_class = CMD_CLASS_ASSIGN;
		cmd->is_compl = 1;

        // TODO, eger birinji birlik maglumat bolup ikinjisi identifikator bolsa.
	}
}

/** Baglama komandasynyň semantikasy **/
int semantic_cmd_assign(command *cmd)
{
    int prev_part = CUR_PART;
    CUR_PART = 7;
    command_item *sci = get_cmd_item(cmd->items,1);
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN
    if (sci->tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM)
    {
        command_item *fci = get_cmd_item(cmd->items,0);
        if (fci->type==TOKEN_ITEM && fci->tok.type_class==TOK_CLASS_IDENT)
        {
            token *item = &fci->tok; // Gysgaltmak uchin ulanlyar
            if ( is_inside_fn() && (
                            !is_tmp_fn_var_ident_used( item->potentional_types[0].value) &&
                            !is_tmp_fn_var_arg_ident_used( item->potentional_types[0].value )
                                    ) )
                print_err(CODE7_LEFT_IDENT_NOT_DEFINED, item);
            else if ( is_inside_fn()==0 && !is_local_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_LEFT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (fci->type==CMD_ITEM)
        {
            check_semantics(&fci->cmd);
        }

        command_item *tci = get_cmd_item(cmd->items,2);
        // IKINJI BIRLIGIN BARLANMASY
        //      eyyam yglan edilen ulnin identifikatory bolsa - identifikator hokman on bir yerde yglan edilmeli
        if (tci->type==TOKEN_ITEM && tci->tok.type_class==TOK_CLASS_IDENT)
        {
            token *item = &tci->tok; // Gysgaltmak uchin ulanlyar

            if ( is_inside_fn() && (
                 !is_tmp_fn_var_ident_used( item->potentional_types[0].value ) &&
                 !is_tmp_fn_var_arg_ident_used( item->potentional_types[0].value )
                                    ) )
                print_err(CODE7_RIGHT_IDENT_NOT_DEFINED, item);
            else if ( !is_inside_fn() && !is_local_var_def_exist(item->potentional_types[0].value))
            {
                print_err(CODE7_RIGHT_IDENT_NOT_DEFINED, item);
            }
        }
        else if (tci->type==CMD_ITEM)
        {
            check_semantics(&tci->cmd);
        }


        /// Ikinji masagaraçylyk: ikisiniňem tipleri biri birine gabat gelmeli
        command_item *ci = tci;
        int class1 = -1, type1 = -1, class2 = -1, type2 = -1;

	    if( return_cmd_item_type( ci, &class2, &type2  ) && class2!=TOK_CLASS_UNDEFINED )
		{
		    command_item *f = fci;
            if( !return_cmd_item_type( f, &class1, &type1 ) )
            {
                if ( f->type==CMD_ITEM &&  f->cmd.cmd_class==CMD_CLASS_DEF_VAR)
                {
                    command_item *ident_ci = get_cmd_item( f->cmd.items, f->cmd.items_num-1 );
                    return_tok_type(&ident_ci->tok, &class1, &type1);
                }
                if (class1==TOK_CLASS_DEF_TYPE)
                    set_def_type_alias_const_data(&class1, &type1);

                if (!(class1==class2 && type1==type2))
                {
                     if(f->type==TOKEN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, &f->tok);
                     if(f->type==CMD_ITEM)
                       print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_last_token(&f->cmd));
                     if(f->type==PAREN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_BOTH_IDENT, (token *)inf_get_parens_last_token(&f->paren));
                }
            }
            else
            {
                if (!(class1==class2 && type1==type2))
                {
                    if(f->type==TOKEN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, &f->tok);
                    if(f->type==CMD_ITEM)
                       print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_last_token(&f->cmd));
                    if(f->type==PAREN_ITEM)
                        print_err(CODE7_TYPES_NOT_MATCH_LEFT_DATA, (token *)inf_get_parens_last_token(&f->paren));
                }
            }
		}
		else
        {
            if(ci->type==TOKEN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, &ci->tok);
            if(ci->type==CMD_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_last_token(&ci->cmd));
            if(ci->type==PAREN_ITEM)
                print_err(CODE7_TYPES_NOT_MATCH_RIGHT_DATA, (token *)inf_get_parens_last_token(&ci->paren));
        }
    }
    // KOMANDANYN CHEPE BAGLANMA GORNUSHI UCHIN

    CUR_PART = prev_part;
    return 1;
}




/** Faýla degişli kody C koda ýazýar
**/
void cmd_assign_c_code(command *cmd, wchar_t **l, int *llen)
{
    // Çepe baglanma:
    command_item *first =  get_cmd_item(cmd->items,0);
    command_item *second = get_cmd_item(cmd->items,1);
    command_item *third =  get_cmd_item(cmd->items,2);

    int cmd_class, cmd_type;

    if ( second->tok.potentional_types[0].type_num==LEFT_ASSIGN_TOK_NUM )
    {
        return_cmd_item_type( first, &cmd_class, &cmd_type );

        if ( cmd_class==TOK_CLASS_CONST_DATA && cmd_type==STRING_CONST_DATA_TOK_NUM )
        {
            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, assign_right );
            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, L" ( " );
            write_cmd_item_c_code( third, &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN );
            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, L" );\n " );

            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, assign_left );
            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, L" ( " );
            write_cmd_item_c_code( first, &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN );
            wcsadd_on_heap( &CMD_ASSIGN_C_CODE_PRE, &CMD_ASSIGN_C_CODE_PRE_LEN, L" );\n " );
            wchar_t *add_right_to_left =
L" if ( ( _tpl_assign_left==NULL || _tpl_assign_right==NULL ) || wcslen( _tpl_assign_left )!=wcslen( _tpl_assign_right ) )\
    _tpl_assign_left = realloc( _tpl_assign_left,  ( wcslen( _tpl_assign_right )+1 )*sizeof( wchar_t ) ); \
   wcsncpy( _tpl_assign_left, _tpl_assign_right, wcslen( _tpl_assign_right )+1 ); ";
            wcsadd_on_heap( l, llen, add_right_to_left );

        }
        else
        {
            // Eger birinji birlik ülňi yglan etmek bolsa, komandanyň içinden tokeniň ady alynýar
            // Eger birinji ülňi identifikator bolsa, özi alynýar.
            if ( first->type==CMD_ITEM )
            {
                command *c = &first->cmd;   /// Ýöne komandana çatylaňda çalt bolar ýaly, gysgaltdyldy
                CMD_GET_C_CODE[ c->cmd_class ][ c->cmd_type ]( c, l, llen );
            }
            else if (first->type==TOKEN_ITEM)
            {
                token *t = &first->tok;
                TOK_GET_C_CODE[t->potentional_types[0].type_class][t->potentional_types[0].type_num](t, l, llen);
            }

            // baglanma ülňiniň c dili üçin warianty goýulýar
            wchar_t *assign_c = L" = ";
            wcsncat_on_heap( l, llen, assign_c );

            // üçünji ülňi maglumat.
            // Üçünji birlik identifikator bolsa, özüni geçirmeli.
            if ( third->type==TOKEN_ITEM && third->tok.potentional_types[0].type_class==TOK_CLASS_IDENT )
            {
                wchar_t *rvalue = third->tok.potentional_types[0].value;
                wcsncat_on_heap( l, llen, rvalue );
            }
            // Üçünji birlik, konstanta maglumat bolsa, onda gerekli funksiýa arkaly maglumat içine salynýar.
            else if ( third->type==TOKEN_ITEM && third->tok.potentional_types[0].type_class==TOK_CLASS_CONST_DATA )
            {
                //wchar_t *rvalue = get_const_data_string(&third->tok);
                //wcsncat_on_heap( l, llen, rvalue );
                write_tok_c_code( &third->tok, l, llen );
            }
            else if ( third->type==CMD_ITEM )
            {
                write_cmd_c_code( &third->cmd, l, llen);
            }
            else if ( third->type==PAREN_ITEM )
            {
                paren_get_c_code(&third->paren, l, llen);
            }
        }


    }
}

