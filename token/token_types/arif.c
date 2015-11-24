/** Arifmetiki operatorlar:
 *  +, -, *, :
 **/
#include <stdlib.h>
#include <string.h>
#include "arif.h"


/** GOŞMAK  +   */
int TOK_CLASS_ARIF_PLUS_TYPE = 0;
char ARIF_PLUS_CHAR = '+';

/** AÝYRMAK -   */
int TOK_CLASS_ARIF_MINUS_TYPE = 1;
char ARIF_MINUS_CHAR = '-';

/** KÖPELTMEK * */
int TOK_CLASS_ARIF_MULTI_TYPE = 2;
char ARIF_MULTI_CHAR = '*';

/** BÖLMEK :    */
int TOK_CLASS_ARIF_DIV_TYPE = 3;
char ARIF_DIV_CHAR = ':';


// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
char *TOK_CLASS_ARIF_CHARS[TOK_CLASS_ARIF_TYPES_NUM][2] = {
    {"+", "+"},
    {"-", "-"},
    {"*", "*"},
    {":", "/"}
};

// Debaglamak üçin
char *ARIF_TOK_NUM_WORDS[TOK_CLASS_ARIF_TYPES_NUM] = {
    "Goşmak",
    "Aýyrmak",
    "Köpeltmek",
    "Bölmek"
};


void tok_arif_c_code(token *tok, char **l, int *llen)
{

    *llen += strlen(TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *l = realloc(*l, *llen+1);
        strncpy(*l, TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_ARIF_CHARS[tok->potentional_types[0].type_num][1]));
    }
}
