/** Arifmetiki operatorlar:
 *  +, -, *, :
 **/
#ifndef TOKEN_TOKEN_TYPES_ARIF_H
#define TOKEN_TOKEN_TYPES_ARIF_H

#include "../token_structs.h"

#ifndef TOK_CLASS_ARIF_TYPES_NUM
#define TOK_CLASS_ARIF_TYPES_NUM 4
#endif // TOK_CLASS_ARIF_TYPES_NUM

/** GOŞMAK  +   */
extern int TOK_CLASS_ARIF_PLUS_TYPE;
extern wchar_t ARIF_PLUS_CHAR;

/** AÝYRMAK -   */
extern int TOK_CLASS_ARIF_MINUS_TYPE;
extern wchar_t ARIF_MINUS_CHAR;

/** KÖPELTMEK * */
extern int TOK_CLASS_ARIF_MULTI_TYPE;
extern wchar_t ARIF_MULTI_CHAR;

/** BÖLMEK :    */
extern int TOK_CLASS_ARIF_DIV_TYPE;
extern wchar_t ARIF_DIV_CHAR;

/// Arifmetiki operator üçin ulanylýan harplar
wchar_t *TOK_CLASS_ARIF_CHARS[TOK_CLASS_ARIF_TYPES_NUM][2];

wchar_t *ARIF_TOK_NUM_WORDS[TOK_CLASS_ARIF_TYPES_NUM];


void tok_arif_c_code(token *tok, wchar_t **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_ARIF_H
