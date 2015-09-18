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

// token types number
#define CONST_TOKEN_TYPES_NUM 3
extern const int TOKEN_TYPES_NUM;

// The longest type 'ident' has 6 characters.
#define CONST_TOKEN_TYPE_MAX_LEN 6
extern const int TOKEN_TYPE_MAX_LEN;


extern const int DEF_TYPE_TYPE_CLASS;
extern const int GLOB_TYPE_CLASS;
extern const int IDENT_TYPE_CLASS;

// Debug uchin
extern char *type_classes[];

// Funksiyalar token uchin strukturalary talap edyaler
#include "token_structs.h"

// tokenleri tanayan funksiyalar
int is_token_def_type(token *tok, char *token_val);
int is_token_def_glob(token *tok, char *token_val);
int is_token_ident(token *tok, char *token_val);

#endif
