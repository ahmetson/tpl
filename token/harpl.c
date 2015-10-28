/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../main/glob.h"
#include "../main/tpl_esc_keys.h"
#include "harpl.h"
#include "../parser.h"
#include "../error.h"

char HARPL_OPENER = '"';

/**
 * Parserde harplar bilen işleýän bölüm
**/
int work_with_string(token *string_tok, char *mode, char c)
{
    // add_char_to_last_string(c) atly funksiya arkaly gosha dyrnak tokenin ichine goshulyar
    add_char_to_last_string(c);

    if (!is_token_string_const_data(string_tok))
    {
        // Nadogry diyip hat chykarmaly
        print_err(CODE2_STRING_IS_WRONG, string_tok);
    }
    else if (string_tok->potentional_types[0].is_compl==1)
    {
        // Adaty moda gechilyar.
        move_to_cmd(string_tok);
        init_token(string_tok);

        *mode = PARSER_DEFAULT_MODE;
    }
    return 1;
}

/**
 * Harplar global ülňide bolýany üçin, başda taýynlamaly bolýar
**/
int prepare_to_work_with_string(token *string_tok, char *mode, char c, int c_pos, int line)
{
    increment_string_tokens();

    // Taze token yasalvar
    init_token(string_tok);
    inf_add_to_token(string_tok, c, c_pos, line);

    string_tok->is_compl = 0;
    string_tok->type_class = TOK_CLASS_CONST_DATA;

    token_type tok_type;
    tok_type.is_compl = 0;
    tok_type.need_value = 1;
    tok_type.type_class = TOK_CLASS_CONST_DATA;
    tok_type.type_num = STRING_CONST_DATA_TOK_NUM;
    tok_type.string_value = &GLOB_STRINGS[GLOB_STRINGS_NUM-1];

    add_potentional_token_type(string_tok, tok_type);
    // Token inisializasiya edilip bolan son,
    // token tipi we klasy, gutarylany gutarylmadyk diyip goyulyar.
    // tokenin maglumaty diyip bolsa, soz maglumata in sonky soz sanynyn adresi baglanyar

    // add_char_to_last_string(c) atly funksiya arkaly gosha dyrnak tokenin ichine goshulyar
    add_char_to_last_string(HARPL_OPENER);
    add_char_to_last_string(c);

    // eger-de token sozlem tokeni yaly tanalmasa,
    if (!is_token_string_const_data(string_tok))
    {
        // Nadogry diyip hat chykarmaly
        print_err(CODE2_STRING_IS_WRONG, string_tok);
    }
    else if (string_tok->potentional_types[0].is_compl==1)
    {
        // Komanda salmaly
        move_to_cmd(string_tok);
        empty_token(string_tok);

        *mode = PARSER_DEFAULT_MODE;
    }
    else
    {
        // Soz moduna bashlasanam bolyar
        *mode = STRING_MODE;
    }
    return 1;
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

    if (GLOB_STRINGS==NULL) return 0;
    GLOB_STRINGS[GLOB_STRINGS_NUM-1] = NULL;
    return 1;
}


/**
 * Ýasaljak programma-da ulanylýan söz tokenleriň iň soňky tanalanyna harpy goşýar
**/
int add_char_to_last_string(char c)
{
    int last = GLOB_STRINGS_NUM-1;
    size_t size = 2;
    if (GLOB_STRINGS[last]!=NULL)
        size += strlen(GLOB_STRINGS[last]);

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
    size_t size = strlen(new_val);

    char **tmp = realloc(GLOB_STRINGS, size);
    if (tmp!=NULL)
    {
        GLOB_STRINGS = tmp;
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

    int len = strlen(tok_val), complete=0;

    // Barlamak nämä gerek, diňe Goşa dyrnak bar.
    if (len==1)
        return 1;

    char ESC_key = 0;
    int  ESC_key_len = 0;
    char DOT_key = 0;
    for (i=1; i<len; ++i)
    {
        // ESC belgini açardan soň, azyndan iki sany harp bolmaly
        // a) ESC belginiň nyşany, b) sözlemi gutaryjy
        if (tok_val[i]==TPL_ESC_KEY_OPENER && (i+1)<len)
        {
            ESC_key=1;
            if (!is_tpl_ESC_key(tok_val[i+1], 0))
            {
                // Tokeniň içinde ESC belgi bolmasa, token maksimum 3 belgiden ybarat bolmaly.
                //delete_potentional_token_type(tok, TOK_CLASS_CONST_DATA, CHAR_CONST_DATA_TOK_NUM);
                return 0;
            }
            else if (tok_val[i+1]=='"' && i+1==len-1)
            {
                DOT_key = 1;
            }
        }
    }

    if (tok_val[strlen(tok_val)-1]=='"' && !DOT_key)
    {
        tpl_ESC_key_to_c_ESC_key(tok_val);
        change_last_string(tok_val);

        tok->potentional_types[0].is_compl = 1;
    }

    return 1;
}
