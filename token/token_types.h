/*
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/
#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

#include "token_types/ident.h"

/** token TYPES

 * nabelli (class num: 0)
 * tip (class num:1)
 * glob (class num: 2)
 * ident (class num: 3)
 * assign (class num: 4)
 * const_data (class num: 5)
 * arif (class num: 6)
**/

extern const int TOK_CLASS_UNDEFINED;   // Tokeniň hiç hili tipi ýok
extern const int TOK_CLASS_UNKNOWN;     // Tokeniň klasy näbelli
extern const int TOK_CLASS_DEF_TYPE;
extern const int TOK_CLASS_GLOB;
extern const int TOK_CLASS_IDENT;
extern const int TOK_CLASS_ASSIGN;
extern const int TOK_CLASS_CONST_DATA;
extern const int TOK_CLASS_ARIF;

// Debug üçin
extern char *type_classes[];

// Funksiyalar token uchin strukturalary talap edyaler
#include "token_structs.h"

// tokenleri tanayan funksiyalar
int is_token_def_type           (token *tok, char *tok_val);
int is_token_def_glob           (token *tok, char *tok_val);
int is_token_ident              (token *tok, char *tok_val);
int is_token_var_left_assign    (token *tok, char *tok_val);
int is_token_int_const_data     (token *tok, char *tok_val);
int is_token_float_const_data   (token *tok, char *tok_val);
int is_token_char_const_data    (token *tok, char *tok_val);
int is_token_arif               (token *tok, char *tok_val);

#endif
