/** Logiki operatorlar
*/
#include <stdlib.h>
#include <string.h>
#include "logic.h"


/** WE                  &   */
int TOK_CLASS_LOGIC_AND_TYPE = 0;

/** ÝA                  ?   */
int TOK_CLASS_LOGIC_OR_TYPE = 1;

/** DÄL BOLSA           ! */
int TOK_CLASS_LOGIC_NOT_TYPE = 2;


/// Tokenleriň başlap bilýän harplary
char LOGIC_AND_CHAR = '&';
char LOGIC_OR_CHAR = '?';
char LOGIC_NOT_CHAR = '!';



// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
char *TOK_CLASS_LOGIC_CHARS[TOK_CLASS_LOGIC_TYPES_NUM][2] = {
    {"&",  "&&"},
    {"?",  "||"},
    {"!",  "!"}
};

// Debaglamak üçin
char *LOGIC_TOK_NUM_WORDS[TOK_CLASS_LOGIC_TYPES_NUM] = {
    "we &",
    "ýa ?",
    "däl bolsa !"
};


void tok_logic_c_code(token *tok, char **l, int *llen)
{

    *llen += strlen(TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1]);
    if (!(*llen-strlen(TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1])))
    {
        *l = realloc(*l, *llen+1);
        strncpy(*l, TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1])+1);
    }
    else
    {
        *l = realloc(*l, *llen);
        strncat(*l, TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1], strlen(TOK_CLASS_LOGIC_CHARS[tok->potentional_types[0].type_num][1]));
    }
}

