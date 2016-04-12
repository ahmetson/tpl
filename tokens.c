/** Tokenler bilen işleýän bölüm*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/// TPL.H global ülňileriň yglan edilmelerini saklaýar
#include "tpl.h"
#include "tokens.h"
#include "token/helpers.h"
#include "parser.h"
#include "token/token_structs.h"
#include "token/token_types.h"
#include "main/inf.h"
#include "cmd/def_var.h"
#include "fns.h"
#include "dev_debug.h"
#include "error.h"


/// Parsing edilýän söz boýunça tokeni tanamak üçin şu sanawdan geçäýmeli
func_ptr check_token[] = {
	   is_token_def_type,
	   is_token_def_glob,
	   is_token_ident,
	   is_token_assign,
	   is_token_float_const_data,
	   is_token_int_const_data,
	   is_token_wchar_t_const_data,
	   is_token_arif,
	   is_token_cmp,
	   is_token_logic,
	   is_token_ctrl_sttmnt,
	   is_token_block,
	   is_token_loop_sttmnt,
	   is_token_triangle_block,
	   is_token_utype,
	   is_token_utype_con,
	   is_token_void,
	   is_token_comment
};


// Tokeniň klasy we tipi boýunça tokeniň gaýtarmaly maglumatynyň tipini berýän funksiýa çagyrýan sanaw
int (*TOK_RETURN_TYPE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, int *tok_class, int *tok_num) =
{
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // Nabelli
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // tip (class num:1)
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // glob
    {get_tok_type_ident_ident_val_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // ident
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // assign
    {get_const_data_type,   get_const_data_type,    get_const_data_type,    get_const_data_type}, // const_data
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // arif
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // cmp
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // logic
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // ctrl_sttmnt
    {empty_tok_return_type},                                                                      // block
    {empty_tok_return_type, empty_tok_return_type},                                               // loop_sttmnt
    {empty_tok_return_type, empty_tok_return_type},                                               // triangle_block
    {empty_tok_return_type, empty_tok_return_type},                                               // utype,
    {get_utype},                                                                                  // utype_con
    {empty_tok_return_type},                                                                      // void
    {empty_tok_return_type, empty_tok_return_type}                                                // comment
};


// Tokeniň klasy we tipi boýunça tokeniň gaýtarmaly maglumatynyň tipini berýän funksiýa çagyrýan sanaw
void (*TOK_GET_C_CODE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, wchar_t **l, int *llen) =
{
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // Nabelli
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // tip (class num:1)
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // glob
    {tok_ident_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // ident
    {tok_assign_c_code,tok_assign_c_code, tok_assign_c_code, tok_assign_c_code}, // assign
    {tok_int_c_code,   tok_float_c_code, tok_wchar_t_c_code, tok_string_c_code},  // const_data
    {tok_arif_c_code,  tok_arif_c_code,  tok_arif_c_code,  tok_arif_c_code},  // arif
    {tok_cmp_c_code,   tok_cmp_c_code,   tok_cmp_c_code,   tok_cmp_c_code, tok_cmp_c_code},   // cmp
    {tok_logic_c_code, tok_logic_c_code, tok_logic_c_code},                      // logic
    {tok_ctrl_sttmnt_c_code, tok_ctrl_sttmnt_c_code, tok_ctrl_sttmnt_c_code, tok_ctrl_sttmnt_c_code},  // ctrl_sttmnt
    {tok_block_c_code},                                                          // block
    {tok_loop_sttmnt_c_code, tok_loop_sttmnt_c_code},                            // loop_sttmnt
    {empty_tok_c_code, empty_tok_c_code},                                        // triangle_block
    {empty_tok_c_code, utype_item_separator_c_code},                             // utype
    {tok_utype_con_c_code},                                                      // utype_con
    {tok_void_con_c_code},                                                       // void
    {empty_tok_c_code, empty_tok_c_code}                                         // comment
};


// 'Default' maglumatlar goýulýar.
// Token ýasalan soň, hökman şu funksiýany çagyryp 'default' maglumatlar goýulmaly
void init_token(token *tok)
{
	tok->ns = 0;							// Global ýa Lokal

	int i; token_type tok_type;

    tok->potentional_types_num = 0;								// Number of recognized types for token
	//wchar_t source_file[MAXLEN];									// source file of token
	tok->type_class = 0;
	tok->value[0] = L'\0';
	tok->is_compl = 0;
	tok->parenthesis = 0;

    tok->inf_wchar_t =
    tok->inf_wchar_t_num =
    tok->inf_file_num =
    tok->inf_line_num = -1;
}


void empty_token(token *tok)
{
	init_token(tok);
}


/** Sözüň token bolup biljek tokenlere barlanýar.
  * Eger token bolup bilýän bolsa, @tok ülňä ýasalan tokeni baglaýar */
int recognize_token(token *tok, wchar_t *val)
{
	int i;
	for (i=0; i<TOKEN_TYPES_NUM; ++i)
    {
        check_token[i](tok, val);
	}

	/// Token tanalmady
	if ( !tok->potentional_types_num )
	{
		return 0;
	}

	inf_add_to_token(tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
	return 1;
}


/** Tokenin bolup biljek tipini tokene goşýar.
 * Tokeniň bolup biljek tipleri ýene köpelýär
 * TODO: Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa,
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
**/
int add_potentional_token_type(token *tok, token_type tok_type)
{
	tok->potentional_types[tok->potentional_types_num++] = tok_type;
	return 1;
}


/**  Tokenin bolup biljek tipini tokene goşýar.
 * Tokeniň bolup biljek tipleri ýene köpelýär
 * TODO: Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa,
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
**/
int delete_potentional_token_type(token *tok, int type_class, int type_num)
{
    int i, j,                                               /// gaýtalamalaryň kounterleri
        potentional_class, potentional_type,
        potentional_types_num = tok->potentional_types_num;

    for (i=0; i<tok->potentional_types_num; ++i)
    {
        potentional_class = get_potentional_token_type_class( tok, i );
        potentional_type = get_potentional_token_type( tok, i );

        if (potentional_class==type_class && potentional_type  == type_num)
        {
            potentional_types_num--;
            /// Pozulan tipden soňky token tipleri öňe süýşürýär,
            move_potentional_types (  tok, i );
        }
    }

    /// Tipleri pozmak üçin, tokende ähli tipleriň möçberini bilmek zerur.
    /// Şonuň üçin pozulan tipleriň möçberi, hemme tipler pozulan soň goýulmaly
    tok->potentional_types_num = potentional_types_num;

	return 1;
}


/** Pozulan token tipiniň ýerine, indiki token tipleriniň maglumatlaryny süýşürýär*/
void move_potentional_types (  token *tok, int deleted_potentional_type_num )
{
    /// Iň soňky tip eken
    if ( deleted_potentional_type_num < tok->potentional_types_num-1 )
    {
        return;
    }

    int i;
    for( i=deleted_potentional_type_num; i<tok->potentional_types_num-1; ++i )
    {
        /// Soňky token tipi süýşürmeli
        tok->potentional_types[ i ] = tok->potentional_types[ i+1 ];
    }
}


/** Tokeniň içinden, komanda-da gerek bolmajak maglumatlary pozýar.
**/
int finishize_token(token *tok)
{
    /// Hiç zat pozmak gerek däl
	if ( !tok->potentional_types_num )
        return 0;

	// dine bir bolup biljek tip goyulyar,
	// in sonky goylan fayl, onki goyulanlary yapyar
	int i;
	for ( i=tok->potentional_types_num-1; i>=0 ; --i )
	{
		if ( tok->potentional_types[i].is_compl==0 )
			continue;

		tok->potentional_types[0] = tok->potentional_types[i];
	}
	tok->potentional_types_num = 1;
	tok->is_compl = 1;

	return 1;
}

/** Token gutaran soň, komanda geçirilmeli. */
void work_with_token(token *tok, command *cmd)
{
    if ( !is_token_empty(tok) && tok->is_compl )
    {
        if ( tok->type_class!=TOK_CLASS_COMMENT )
            add_to_cmd(cmd, TOKEN_ITEM, *tok, get_empty_paren(), get_empty_cmd());
    }
}


/** Faýldan tokeni tanaýan parser.*/
token parse_token( FILE *s )
{
    /// Adaty parseriň modynda, tokenleri saýgarmak üçin.
	wchar_t tok_string[ MAX_TOKEN_LEN ] = { 0 };
	/// Bular bolsa, tokenler saýgarylanda, ýatda saklamak üçin
	token   tok,
            new_tok;
    init_token( &tok );
	init_token( &new_tok );

    /// Ilkinji parsing edilen harpy salmaly
    prevent_from_parsing_file();

    while( 1 )
	{
	    if ( !process_char( s, CHECK_VALID_CHAR ) )
            break;

        /** Tokeniň uzynlygy MAX_TOKEN_LEN-den uly bolmaly däl*/
        check_tok_len( tok_string, &tok );

        /** Token - bu öňki token, we häzirki parsing edilen harpdan durýar*/
        str_add_char( tok_string, CUR_CHAR );

        /** Häzirki harp öňki tokeniň yzy bolsa*/
        if ( !recognize_token( &new_tok, tok_string ) )
        {
            /** Parser, tanalmadyk harpy başga parserlere görä barlamaly */
            prevent_from_parsing_file();
            break;
        }
        else
        {
            tok = new_tok;
            init_token(&new_tok);
        }
    }

    /** Tokenden komanda goşulanda gerek bolmajak maglumatlar pozulýar */
    finishize_token( &tok );

    return tok;
}


int return_tok_type(token *tok, int *ret_class, int *ret_type)
{
    if (tok->type_class==TOK_CLASS_UTYPE_CON)
    {
        *ret_class = TOK_CLASS_UTYPE_CON;
        *ret_type   = tok->potentional_types[0].type_num;
        return 1;
    }
    return TOK_RETURN_TYPE[tok->potentional_types[0].type_class][tok->potentional_types[0].type_num](tok, ret_class, ret_type);
}
