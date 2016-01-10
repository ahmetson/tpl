/** Logiki operatorlar */
#ifndef TOKEN_TOKEN_TYPES_LOGIC_H
#define TOKEN_TOKEN_TYPES_LOGIC_H


#include "../token_structs.h"

#ifndef TOK_CLASS_LOGIC_TYPES_NUM
#define TOK_CLASS_LOGIC_TYPES_NUM 3
#endif // TOK_CLASS_LOGIC_TYPES_NUM

/** WE                  &   */
extern int TOK_CLASS_LOGIC_AND_TYPE;

/** ÝA                  ?   */
extern int TOK_CLASS_LOGIC_OR_TYPE;

/** DÄL BOLSA           ! */
extern int TOK_CLASS_LOGIC_NOT_TYPE;


/// Tokenleriň başlap bilýän harplary
extern wchar_t LOGIC_AND_CHAR;
extern wchar_t LOGIC_OR_CHAR;
extern wchar_t LOGIC_NOT_CHAR;



// Arifmetiki operatorlar üçin ulanylýan harplar.
// Ikinji element bolsa, C dilinde, arifmetiki operatorlar üçin ulanylýan harp ekwiwalentleri
extern wchar_t *TOK_CLASS_LOGIC_CHARS[TOK_CLASS_LOGIC_TYPES_NUM][2];

// Debaglamak üçin
extern wchar_t *LOGIC_TOK_NUM_WORDS[TOK_CLASS_LOGIC_TYPES_NUM];

void tok_logic_c_code(token *tok, wchar_t **l, int *llen);

#endif // TOKEN_TOKEN_TYPES_LOGIC_H
