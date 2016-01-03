/** Koddaky skobkalar bilen işleýär
 *
**/
#ifndef PARENTHESIS_H
#define PARENTHESIS_H
#include <stdio.h>

extern char PARENTHESIS_OPEN;
extern char PARENTHESIS_CLOSE;
extern char PARENTHESIS_ELEM_SEPARATOR;

#ifndef PARENTHESIS_TYPES
#define PARENTHESIS_TYPES 4
#endif

// Bashga komandalar ya tokenler bolup bilyan skobka
typedef struct {
	int elems;
	int elems_num;
	int type;
}parenthesis;

#include "cmds.h"
#include "tokens.h"
typedef struct {
    parenthesis paren;
    token   tok;
    command cmd;
    char type;
}parenthesis_elem;


int (*PAREN_RETURN_TYPE[PARENTHESIS_TYPES])(parenthesis *paren, int *ret_class, int *ret_type);

/** Eger ýaý bir birlik saklaýan bolsa, onda birligiň tipini gaýtarýar:
    @p        - ýaýyň özi,
    @itemType - ýaýyň içindäki birligiň tipi
    @rClass   - birligiň tipiniň klasy
    @rType    - birligiň tipi

    \return - ýaýyň içindäki birligiň tipini ötürdip boldumy ýa ýoklugynyň statusyny gaýtarýar */
int get_paren_item_type(parenthesis *p, int *item_type, int *rClass, int *rType);

// Parserde skobka duşan wagty, içindäki elementler bilen işleýän bölüm
parenthesis parse_paren(FILE *f);

parenthesis      parenthesis_new();
parenthesis_elem parenthesis_elem_new();
void paren_add_element(parenthesis *paren, parenthesis_elem elem);
int paren_elem_add_token(parenthesis_elem *elem, token tok);
int param_elem_add_paren(parenthesis_elem *elem, parenthesis paren);
int recognize_paren(parenthesis *paren);

parenthesis get_empty_paren();

parenthesis_elem *get_paren_elems(int paren_num);

void paren_get_c_code(parenthesis *p, char **l, int *llen);

int is_paren_not_compl_item_exist(parenthesis *p, char rec);

#endif
