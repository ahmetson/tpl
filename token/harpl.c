/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../main/glob.h"
#include "../main/tpl_esc_keys.h"
#include "../main/inf.h"
#include "harpl.h"
#include "../parser.h"
#include "../error.h"

char HARPL_OPENER = '"';

/**
 * Parserde harplar bilen işleýän bölüm
**/
token parse_string(FILE *s)
{
    token string_tok;

    string_prepare(&string_tok);

    while ((CUR_CHAR=fgetc(s))!=EOF)
    {
        update_inf();
        add_char_to_last_string(CUR_CHAR);

        if (!is_token_string_const_data(&string_tok))
        {
            // Nadogry diyip hat chykarmaly
            printf("SALAMM");
            print_err(CODE2_STRING_IS_WRONG, &string_tok);
        }
        else if (string_tok.potentional_types[0].is_compl==1)
        {
            //debug_token(&string_tok);
            return string_tok;
        }
    }
    //if (!work_with_token(&tok, &prev_tok_string))
    //    print_err();

    return string_tok;
    // TODO : eger string token gutarylmadyk bolsa yalnyshlyk chykarylyar.
}

/**
 * Harplar global ülňide bolýany üçin, başda taýynlamaly bolýar
**/
void string_prepare(token *string_tok)
{
    increment_string_tokens();

    // Taze token yasalvar
    init_token(string_tok);
    inf_add_to_token(string_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);

    string_tok->is_compl = 0;
    string_tok->type_class = TOK_CLASS_CONST_DATA;

    token_type tok_type;
    tok_type.is_compl = 0;
    tok_type.need_value = 1;
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.type_num = STRING_CONST_DATA_TOK_NUM;
    tok_type.parenthesis = 1;
    tok_type.string_value = &GLOB_STRINGS[GLOB_STRINGS_NUM-1];

    add_potentional_token_type(string_tok, tok_type);
    // Token inisializasiya edilip bolan son,
    // token tipi we klasy, gutarylany gutarylmadyk diyip goyulyar.
    // tokenin maglumaty diyip bolsa, soz maglumata in sonky soz sanynyn adresi baglanyar

    // add_char_to_last_string(c) atly funksiya arkaly gosha dyrnak tokenin ichine goshulyar
    add_char_to_last_string(HARPL_OPENER);

    // eger-de token sozlem tokeni yaly tanalmasa,
    if (!is_token_string_const_data(string_tok))
    {
        // Nadogry diyip hat chykarmaly
        print_err(CODE2_STRING_IS_WRONG, string_tok);
    }
}

/**
 * Ýasaljak programma-da ulanylýan sözleriň sanawy ýene bire köpeldilýär.
**/
int increment_string_tokens()
{
    GLOB_STRINGS_NUM++;
    size_t size = 0;

    size = sizeof(*GLOB_STRINGS)*GLOB_STRINGS_NUM;
    GLOB_STRINGS = realloc(GLOB_STRINGS, size);

    if (GLOB_STRINGS==NULL)
    {
        return 0;
    }
    GLOB_STRINGS[GLOB_STRINGS_NUM-1] = NULL;
    return 1;
}


/**
 * Ýasaljak programma-da ulanylýan söz tokenleriň iň soňky tanalanyna harpy goşýar
**/
int add_char_to_last_string(char c)
{
    int last = GLOB_STRINGS_NUM-1;
    long size = 2;
    if (GLOB_STRINGS[last]!=NULL)
    {
        size += strlen(GLOB_STRINGS[last]);
    }
    GLOB_STRINGS[last] = realloc(GLOB_STRINGS[last], size);
    if (GLOB_STRINGS[last]==NULL)
        printf("Soz uchin yer yasap bolmady\n");

    GLOB_STRINGS[last][size-1] = '\0';
    GLOB_STRINGS[last][size-2] = c;
    return 1;
}


/**
 * Ýasaljak programma üçin goşulan iň soňky sözi çalyşýar
**/
int change_last_string(char *new_val)
{
    int last = GLOB_STRINGS_NUM-1;
    long size = strlen(new_val);

    char *tmp = realloc(GLOB_STRINGS[last], size);
    if (tmp!=NULL)
    {
        GLOB_STRINGS[last] = tmp;
    }

    strncpy(GLOB_STRINGS[last], new_val, strlen(new_val)+1);

    return 1;
}


// Tokeniň sözmidigini barlaýar
int is_token_string_const_data(token *tok)
{
    int i;
    char *tok_val; tok_val = GLOB_STRINGS[GLOB_STRINGS_NUM-1];
    if (!(tok->potentional_types[0].type_class==TOK_CLASS_CONST_DATA &&
          tok->potentional_types[0].type_num==STRING_CONST_DATA_TOK_NUM))
        return 0;

    int len = strlen(tok_val);

    // Barlamak nämä gerek, diňe Goşa dyrnak bar.
    if (len==1)
        return 1;

    if (!is_valid_string_const_data(tok_val, 1))
    {
        return 0;
    }
    if (is_string_const_data_compl(tok_val, 0))
    {
        tok->potentional_types[0].is_compl = 1;
        tok->is_compl = 1;
    }
    return 1;
}

/// adaty sozler uchin after_quote_pos - 1.
int is_valid_string_const_data(char *tok_val, int after_quote_pos)
{
    int i, len = strlen(tok_val);
    char escape_quote = 0;
    for (i=after_quote_pos; i<len; ++i)
    {
        // ESC belgini açardan soň, azyndan iki sany harp bolmaly
        // a) ESC belginiň nyşany, b) sözlemi gutaryjy
        if (tok_val[i]==TPL_ESC_KEY_OPENER && (i+1)<len)
        {
            if (!is_tpl_ESC_key(tok_val[i+1], 0))
            {
                return 0;
            }
            else if (tok_val[i+1]=='"' && i+1==len-1)
            {
                escape_quote = 1;
            }
        }
    }

    return 1;
}


int is_string_const_data_compl(char *tok_val, char onstack)
{
    if (strlen(tok_val)<2)
        return 0;
    if (tok_val[strlen(tok_val)-1]=='"' && tok_val[strlen(tok_val)-2]!='=')
    {
        tpl_ESC_key_to_c_ESC_key(tok_val);
        if (!onstack)
            change_last_string(tok_val);
        return 1;
    }
    return 0;
}
