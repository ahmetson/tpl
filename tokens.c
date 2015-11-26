/*
 * Tokenler bilen işleýän bölüm
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TPL.H global ülňileriň yglan edilmelerini saklaýar
#include "tpl.h"
#include "tokens.h"
#include "token/token_structs.h"
#include "token/token_types.h"
#include "main/inf.h"
#include "cmd/def_var.h"
#include "fns.h"
#include "dev_debug.h"
#include "error.h"


// Sozüň tokendigini bilmek üçin, şu sanawdan geçäýmeli
is_token_item tok_types[] = {
	   {is_token_def_type},
	   {is_token_def_glob},
	   {is_token_ident},
	   {is_token_var_left_assign},
	   {is_token_float_const_data},
	   {is_token_int_const_data},
	   {is_token_char_const_data},
	   {is_token_arif}
};


// Tokeniň klasy we tipi boýunça tokeniň gaýtarmaly maglumatynyň tipini berýän funksiýa çagyrýan sanaw
int (*TOK_RETURN_TYPE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, int *tok_class, int *tok_num) =
{
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // Nabelli
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // tip (class num:1)
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // glob
    {get_tok_type_ident_ident_val_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // ident
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}, // assign
    {get_tok_type_const_data_int_val_type, get_tok_type_const_data_float_val_type, get_tok_type_const_data_char_val_type, get_tok_type_const_data_string_val_type},  // const_data
    {empty_tok_return_type, empty_tok_return_type, empty_tok_return_type, empty_tok_return_type}  // arif
};


// Tokeniň klasy we tipi boýunça tokeniň gaýtarmaly maglumatynyň tipini berýän funksiýa çagyrýan sanaw
void (*TOK_GET_C_CODE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, char **l, int *llen) =
{
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // Nabelli
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // tip (class num:1)
    {empty_tok_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // glob
    {tok_ident_c_code, empty_tok_c_code, empty_tok_c_code, empty_tok_c_code}, // ident
    {tok_assign_c_code,tok_assign_c_code, tok_assign_c_code, tok_assign_c_code}, // assign
    {tok_int_c_code,   tok_float_c_code, tok_char_c_code, tok_string_c_code},  // const_data
    {tok_arif_c_code,  tok_arif_c_code,  tok_arif_c_code,  tok_arif_c_code}  // arif
};


// 'Default' maglumatlar goýulýar.
// Token ýasalan soň, hökman şu funksiýany çagyryp 'default' maglumatlar goýulmaly
void init_token(token *tok)
{
	tok->ns = 0;							// Global ýa Lokal

	int i; token_type tok_type;

	if (tok->potentional_types_num>=1 && tok->potentional_types_num<=TOKEN_TYPES_NUM)
	{
	    for(i=0; i<(tok->potentional_types_num); i++)
		{
		    //tok->potentional_types[i].string_value = NULL;
		    tok->potentional_types[i] = tok_type;
			//
		}
    }

    tok->potentional_types_num = 0;								// Number of recognized types for token
	//char source_file[MAXLEN];									// source file of token
	tok->type_class = 0;
	tok->value[0] = '\0';
	tok->is_compl = 0;
	tok->parenthesis = 0;

    tok->inf_char =
    tok->inf_char_num =
    tok->inf_file_num =
    tok->inf_line_num = -1;
}


void empty_token(token *tok)
{

	init_token(tok);

}


/**
 * Sözüň token bolup biljegini barlaýar.
 * Eger token bolup bilýän bolsa, @tok ülňä ýasalan tokeni baglaýar
**/
int recognize_token(token *tok, char *val)
{
	int i;

	for (i=0; i<TOKEN_TYPES_NUM; ++i) tok_types[i].is_token(tok, val);

	// Token not recognized
	if (tok->potentional_types_num==0)
	{
		//printf("Token not recognized\t(Inputted text:%s)\n", val);
		return 0;
	}

	//printf("Token Recognized\t(Inputted text:%s)\n", val);
	inf_add_to_token(tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
	return 1;
}


/**
 * Tokenin bolup biljek tipini tokene goşýar.
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


/**
 * Tokenin bolup biljek tipini tokene goşýar.
 * Tokeniň bolup biljek tipleri ýene köpelýär
 * TODO: Eger tokenin eyyam goshuljak gornushinin klasynda gornushi bar bolsa,
 * we taze gornush gutarylan bolsa, onda onki gornush tazelenyar.
 *
**/
int delete_potentional_token_type(token *tok, int type_class, int type_num)
{
    int i, j, moveto, potentional_types_num = tok->potentional_types_num;
    for (i=0; i<tok->potentional_types_num; ++i)
    {
        if (tok->potentional_types[i].type_class==type_class &&
            tok->potentional_types[i].type_num  ==type_num)
        {
            potentional_types_num--;
            // Eger pozulan tipden soň, ýene token tipleri bar bolsa,
            // olary öňe süýşirmeli
            if (i<tok->potentional_types_num-1)
            {
                moveto = 1;
            }
        }
        if (moveto)
        {
            for(j=i; j<tok->potentional_types_num; ++j)
            {
                if (j+1<tok->potentional_types_num)
                    tok->potentional_types[j] = tok->potentional_types[j+1];
            }
            moveto = 0;
        }
    }

    tok->potentional_types_num = potentional_types_num;

	return 1;
}

/**
 * Tokeniň içinden, komanda-da gerek bolmajak maglumatlary pozýar.
**/
int finishize_token(token *tok)
{
    // Hiç zat pozmak gerek däl
	if (!tok->potentional_types_num) return 0;

	// dine bir bolup biljek tip goyulyar,
	// in sonky goylan fayl, onki goyulanlary yapyar
	int i;
	for (i=tok->potentional_types_num-1;i>=0; --i)
	{
		if (tok->potentional_types[i].is_compl==0)
			continue;
		else
			tok->potentional_types[0] = tok->potentional_types[i];
	}
	tok->potentional_types_num = 1;
	tok->is_compl = tok->potentional_types[0].is_compl;

	return 1;
}


/**
 * Tokeniň içinde ulanyp boljak maglumat bardygyny barlaýar
**/
int is_token_empty(token *tok)
{
	return !tok->potentional_types_num;
}

/**
 * Token gutaran soň, komanda geçirilmeli.
**/
int work_with_token(token *tok, command *cmd)
{
// 1. Onki token bar
    if (!is_token_empty(tok) && tok->is_compl)
    {
        cmd_add_item(cmd, TOKEN_ITEM, get_empty_paren(), get_empty_cmd(), *tok);
        return 1;
    }
    return 0;
}


token parse_token(FILE *s)
{
    // Adaty parseriň modynda, tokenleri saýgarmak üçin.
	char prev_tok_string[CONST_MAX_TOKEN_LEN] = {0};
	char new_tok_string [CONST_MAX_TOKEN_LEN] = {0};
	// Bular bolsa, tokenler saýgarylanda, ýatda saklamak üçin
	token tok;      init_token(&tok);
	token new_tok;  init_token(&new_tok);

    return_last_char(s);

    while((CUR_CHAR=fgetc(s))!=EOF)
	{
	    // Maglumatlar üçin
	    update_inf();

        if (strlen(prev_tok_string)+1 >= CONST_MAX_TOKEN_LEN)
                print_err(CODE2_TOKEN_TOO_BIG, &tok);

        // Häzirki harp öňki tokeniň yzy bolsa
        if ( strstrchcat(new_tok_string, prev_tok_string, CUR_CHAR) &&
            !recognize_token(&new_tok, new_tok_string))
        {
            /** DÜZETMELI #1: Eger programmirleme diliniň goldamaýan harpy bolsa, programma onuň bilen näme etmelidigini bilenok.
                              Şonuň üçinem harp tanaýan parserler boýunça barlap ömürlik aýlaw edip durýar.

                ++ Islendik parser çagyrylmazdan öň:
                1) Eger is_valid_char() diýen funksiýa arkaly, harpyň başga parserleriň başydygy barlanmagy şowly bolsa
                    a) "Nätanyş harp duşdy, token beýle harplardan başlanok" diýip ýalňyş ugradylýar.
            **/
            return_last_char(s);
            // Tokenden komanda goşulanda gerek bolmajak maglumatlar pozulýar
            //debug_token(&tok);
            finishize_token(&tok);

            return tok;
        }
        else
        {
            empty_string(prev_tok_string, CONST_MAX_TOKEN_LEN);
            strncpy(prev_tok_string, new_tok_string, strlen(new_tok_string)+1);

            empty_string(new_tok_string, CONST_MAX_TOKEN_LEN);
            tok = new_tok;
            init_token(&new_tok);
                // Tokene maglumatlary goýmaly
            inf_add_to_token(&new_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);

        }
    }
    //if (!work_with_token(&tok, &prev_tok_string))
    //    print_err();
    finishize_token(&tok);

    return tok;
}

int empty_tok_return_type(token *tok, int *tok_class, int *tok_num)
{
    *tok_class = TOK_CLASS_UNDEFINED;
    return 0;
}


void empty_tok_c_code(token *tok, char **l, int *llen)
{
    return;
}


token get_empty_tok()
{
    token t;
    init_token(&t);
    return t;
}
