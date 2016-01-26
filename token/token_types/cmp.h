/** Deňeşdirme operatorlarynyň tokenleri
**/
#ifndef TOKEN_TOKEN_TYPES_CMD_H
#define TOKEN_TOKEN_TYPES_CMD_H

#include "../token_structs.h"

#ifndef TOK_CLASS_CMP_TYPES_NUM
#define TOK_CLASS_CMP_TYPES_NUM 5
#endif // TOK_CLASS_CMP_TYPES_NUM

extern int  TOK_CLASS_CMP_EQ_TYPE,      /** DEŇDIR           =   */
            TOK_CLASS_CMP_LT_TYPE,      /** KIÇIDIR          <   */
            TOK_CLASS_CMP_GT_TYPE,      /** ULUDYR           > */
            TOK_CLASS_CMP_EQLT_TYPE,    /** KIÇIDIR ÝA DEŇDIR <=    */
            TOK_CLASS_CMP_EQGT_TYPE;    /** ULUDYR ÝA DEŇDIR  >=    */


// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
extern wchar_t *TOK_CLASS_CMP_CHARS[TOK_CLASS_CMP_TYPES_NUM][3];


void tok_cmp_c_code(token *tok, wchar_t **l, int *llen);

int is_token_cmp                (token *tok, wchar_t *tok_val);


#endif // TOKEN_TOKEN_TYPES_CMD_H
