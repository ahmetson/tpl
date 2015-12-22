/** Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
    Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
*/
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
 * cmp (class num: 7)
 * logic (class num: 8)
 * ctrl_sttmnt (class num: 9)
 * block (class num: 10)
 * loop_sttmnt (class num: 11)
 * triangle_block (class num: 12)
 * utype (class num: 13)
 * utype_con (class num: 14)
**/

extern const int TOK_CLASS_UNDEFINED;   // Tokeniň hiç hili tipi ýok
extern const int TOK_CLASS_UNKNOWN;     // Tokeniň klasy näbelli
extern const int TOK_CLASS_DEF_TYPE;
extern const int TOK_CLASS_GLOB;
extern const int TOK_CLASS_IDENT;
extern const int TOK_CLASS_ASSIGN;
extern const int TOK_CLASS_CONST_DATA;
extern const int TOK_CLASS_ARIF;
extern const int TOK_CLASS_CMP;
extern const int TOK_CLASS_LOGIC;
extern const int TOK_CLASS_CTRL_STTMNT;
extern const int TOK_CLASS_BLOCK;
extern const int TOK_CLASS_LOOP_STTMNT;
extern const int TOK_CLASS_TRIANGLE_BLOCK;
extern const int TOK_CLASS_UTYPE;
extern const int TOK_CLASS_UTYPE_CON;   // Hasaba alynanok

// Debag üçin
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
int is_token_cmp                (token *tok, char *tok_val);
int is_token_logic              (token *tok, char *tok_val);
int is_token_ctrl_sttmnt        (token *tok, char *tok_val);
int is_token_block              (token *tok, char *tok_val);
int is_token_loop_sttmnt        (token *tok, char *tok_val);
int is_token_triangle_block     (token *tok, char *tok_val);
int is_token_utype              (token *tok, char *tok_val);
int is_token_utype_con          (token *tok, char *tok_val);

#endif
