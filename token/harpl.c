/**
 * HARPL tokeniň öz aýratynlyklary bar. Sebäbi başga tokenlere garanyňda, bu tokenler maksimum tükeniksiz uzynlygy bolup bilýär.
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../main/glob.h"
#include "../main/tpl_esc_keys.h"
#include "harpl.h"

char HARPL_OPENER = '"';

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
