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
extern int  TOK_CLASS_ARIF_PLUS_TYPE ,      /** GOŞMAK  +   */
            TOK_CLASS_ARIF_MINUS_TYPE,      /** AÝYRMAK -   */
            TOK_CLASS_ARIF_MULTI_TYPE,      /** KÖPELTMEK * */
            TOK_CLASS_ARIF_DIV_TYPE;      /** BÖLMEK :    */

/// Arifmetiki operator üçin ulanylýan harplar
wchar_t *TOK_CLASS_ARIF_CHARS[TOK_CLASS_ARIF_TYPES_NUM][3];

int is_token_arif               (token *tok, wchar_t *tok_val);

void tok_arif_c_code(token *tok, wchar_t **l, int *llen);


#endif // TOKEN_TOKEN_TYPES_ARIF_H
