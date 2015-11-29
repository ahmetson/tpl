/** Deňeşdirme operatorlarynyň tokenleri
**/
#include <string.h>
#include <stdlib.h>
#include "cmp.h"

/** DEŇDIR           =   */
int TOK_CLASS_CMP_EQ_TYPE = 0;

/** KIÇIDIR          <   */
int TOK_CLASS_CMP_LT_TYPE = 1;

/** ULUDYR           > */
int TOK_CLASS_CMP_GT_TYPE = 2;

/** KIÇIDIR ÝA DEŇDIR <=    */
int TOK_CLASS_CMP_EQLT_TYPE = 3;

/** ULUDYR ÝA DEŇDIR  >=    */
int TOK_CLASS_CMP_EQGT_TYPE = 4;

/// Tokenleriň başlap bilýän harplary
char CMP_EQ_CHAR = '=';
char CMP_LT_CHAR = '<';
char CMP_GT_CHAR = '>';



// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
char *TOK_CLASS_CMP_CHARS[TOK_CLASS_CMP_TYPES_NUM][2] = {
    {"=",  "=="},
    {"<",  "<"},
    {">",  ">"},
    {"<=", "<="},
    {">=", ">="}
};

// Debaglamak üçin
char *CMP_TOK_NUM_WORDS[TOK_CLASS_CMP_TYPES_NUM] = {
    "deň",
    "kiçi <",
    "uly >",
    "kiçi ýa deň <=",
    "uly ýa deň >="
};

void tok_cmp_c_code(token *tok, char **l, int *llen)
{

    *llen += strlen(TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *l = realloc(*l, *llen+1);
        strncpy(*l, TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_CMP_CHARS[tok->potentional_types[0].type_num][1]));
    }
}

