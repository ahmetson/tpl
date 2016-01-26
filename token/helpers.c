/** Token bilen işleýän kömekçi funksiýalar*/
#include "helpers.h"
#include "../tokens.h"
#include "token_types.h"
#include "../main/files.h"
#include "../error.h"

int empty_tok_return_type(token *tok, int *tok_class, int *tok_num)
{
    *tok_class = TOK_CLASS_UNDEFINED;
    return 0;
}

void empty_tok_c_code(token *tok, wchar_t **l, int *llen){}

token get_empty_tok()
{
    token t;
    init_token(&t);
    return t;
}

void check_tok_len( wchar_t *tokval, token *tok )
{
    if ( wcslen( tokval )+1 >= MAX_TOKEN_LEN)
        print_err(CODE2_TOKEN_TOO_BIG, tok);
}

/** Tokeniň içinde ulanyp boljak maglumat bardygyny barlaýar*/
int is_token_empty(token *tok)
{
	return !tok->potentional_types_num;
}

/** Tokeniň bolup biljek tipiniň klasyny gaýtarýar*/
int get_potentional_token_type_class( token *tok, int potentional_type_num )
{
    return tok->potentional_types[ potentional_type_num ].type_class;
}


/** Tokeniň bolup biljek tipiniň nomerini gaýtarýar*/
int get_potentional_token_type( token *tok, int potentional_type_num )
{
    return tok->potentional_types[ potentional_type_num ].type_num;
}


/** Tokeniň degişli bolan klasyny gaýtarýar*/
int get_token_type_class( token *tok )
{
    return tok->potentional_types[ 0 ].type_class;
}

/** Tokeniň degişli bolan tipini gaýtarýar*/
int get_token_type( token *tok )
{
    return tok->potentional_types[ 0 ].type_num;
}

wchar_t *get_token_value ( token *tok )
{
    return (wchar_t *)tok->potentional_types[0].value;
}

/** Tokeniň gutarandygynyň statusyny gaýtarýar*/
int is_complete_token( token *tok )
{
    return tok->potentional_types[0].is_compl;
}
