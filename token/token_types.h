/*
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/
#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

/* token TYPES

 * tip (class num:1)
 * glob (class num: 2)
 * ident (class num: 3)
**/

// token types number, doks/token_goshmak_tertipleri
extern const int TOKEN_TYPES_NUM;

// The longest type 'ident' has 6 characters.
#define CONST_TOKEN_TYPE_MAX_LEN 6
extern const int TOKEN_TYPE_MAX_LEN;

extern const int TOK_CLASS_DEF_TYPE;
extern const int TOK_CLASS_GLOB;
extern const int TOK_CLASS_IDENT;
extern const int TOK_CLASS_ASSIGN;
extern const int TOK_CLASS_CONST_DATA;

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
#endif
