/** Deňeşdirme operatorlarynyň tokenleri
**/
#ifndef TOKEN_TOKEN_TYPES_CMD_H
#define TOKEN_TOKEN_TYPES_CMD_H

#include "../token_structs.h"

#ifndef TOK_CLASS_CMP_TYPES_NUM
#define TOK_CLASS_CMP_TYPES_NUM 5
#endif // TOK_CLASS_CMP_TYPES_NUM


/** DEŇDIR           =   */
extern int TOK_CLASS_CMP_EQ_TYPE;

/** KIÇIDIR          <   */
extern int TOK_CLASS_CMP_LT_TYPE;

/** ULUDYR           > */
extern int TOK_CLASS_CMP_GT_TYPE;

/** KIÇIDIR ÝA DEŇDIR <=    */
extern int TOK_CLASS_CMP_EQLT_TYPE;

/** ULUDYR ÝA DEŇDIR  >=    */
extern int TOK_CLASS_CMP_EQGT_TYPE;

/// Tokenleriň başlap bilýän harplary
extern wchar_t CMP_EQ_CHAR;
extern wchar_t CMP_LT_CHAR;
extern wchar_t CMP_GT_CHAR;

/// Debug'lamak üçin
wchar_t *CMP_TOK_NUM_WORDS[TOK_CLASS_CMP_TYPES_NUM];

// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
extern wchar_t *TOK_CLASS_CMP_CHARS[TOK_CLASS_CMP_TYPES_NUM][2];


void tok_cmp_c_code(token *tok, wchar_t **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_CMD_H
