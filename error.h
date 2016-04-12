#ifndef ERROR_H
#define ERROR_H

/*
 * Kompilyatoryn bolekleri
 *
 * 0 - kompilyatoryn oz yalnyshlyklary
 * 1 - inisializasiya (global ulnileri, sanawlary ylgan etmek) we fayllar
 * 2 - harplary saygarmak
 * 3 - token yasamak
 * 4 - komandalary yasamak
 * 5 - algoritmleri yasamak (komandalardan)
 * 6 - algoritmin golayndaky global sanawlar
 * 7 - semantika (algoritm boyuncha gechip, funksiyalaryn we ulnilerin yglan edilishlerini barlamak)
 * 8 - C translyatory
**/
typedef struct {
    int tpl_part;       // Ýalňyşlyk TPL'iň niresinde peýda boldy?
    int err_code;       // Ýalňyşlygyň kody
    wchar_t file_num;
    wchar_t line_num;
    wchar_t start_wchar_t;
    wchar_t wchar_t_num;
    wchar_t code_line;      // Koddaky ýalňyşlygyň duşan ýerini görkezmelimi
    wchar_t *msg;          // Ulanyja düşnükli ýalňyşlygyň teksti
}error_item;

extern wchar_t *parts_names[];

/* Yalnyshlykly tekstlerin nomerleri */
extern int CODE0_PRAGMA_ACT_NOT_IDENTIFIED;
extern int CODE0_MAIN_FILE_ALREADY_CHOSED;
extern int CODE0_NOT_FOUND_MAIN_FILE;
extern int CODE0_IDENT_NOT_DEFINED;
extern int CODE0_IDENT_CANT_ADD;
extern int CODE0_INCLUDES_CANT_ADD;
extern int CODE0_CANT_OPEN_FILE;
extern int CODE0_UNSUPPORT_INCLUDE_FILE_CMD;

extern int CODE1_FILE_NOT_FOUND;
extern int CODE1_FILE_CANT_OPEN;
extern int CODE1_FILE_NAME_RESERVED;

extern int CODE2_UNKNOWN_TOKENS_CHAR;
extern int CODE2_PRAGMA_NOT_END;
extern int CODE2_PRAGMA_NOT_IDENT;
extern int CODE2_REMAIN_TOKEN;
extern int CODE2_TOKEN_TOO_BIG;
extern int CODE2_PREV_TOKEN_INCORRECT;
extern int CODE2_STRING_IS_WRONG;
extern int CODE2_REQUIRED_END_BLOCK;
extern int CODE2_PRAGMA_NOT_SUPPORT_IN_TRIANGLE_BLOCK;
extern int CODE2_CMD_END_CHAR_NOT_SUPPORT_IN_TRIANGLE_BLOCK;
extern int CODE2_TRIANGLE_BLOCK_NOT_SUPPORT_CMD;
extern int CODE2_TRIANGLE_BLOCK_CANT_BE_EMPTY;
extern int CODE2_CANT_IDENT_CMD_IN_PAREN;

extern int CODE3_PREV_TOK_INCORRECT;

extern int CODE4_CMD_HASNT_FIRST_TOKEN;
extern int CODE4_CANT_ADD_TOKEN;
extern int CODE4_CANT_IDENT_CMD;
extern int CODE4_VARS_IDENT_USED;
extern int CODE4_TOK_TYPES_NOT_MATCH;
extern int CODE4_UNSUPPORT_CMD_INSIDE_FN;

extern int CODE7_TYPES_NOT_MATCH_RIGHT_DATA;
extern int CODE7_TYPES_NOT_MATCH_LEFT_DATA;
extern int CODE7_TYPES_NOT_MATCH_BOTH_IDENT;
extern int CODE7_FN_ARG_TYPES_NOT_MATCH;
extern int CODE7_FN_ARG_NUMES_NOT_MATCH;
extern int CODE7_UNKNOWN_FNS_CALLED;
extern int CODE7_UNKNOWN_DATA_USED;
extern int CODE7_A_TO_B_B_TO_A;
extern int CODE7_UNKNOWN_GLOB_VAR_CALLED;
extern int CODE7_GLOB_VAR_MUST_DEF;
extern int CODE7_LEFT_IDENT_NOT_DEFINED;
extern int CODE7_RIGHT_IDENT_NOT_DEFINED;
extern int CODE7_GLOB_VAR_NOT_MATCH_DATA_TYPE;
extern int CODE7_NEED_IF_OR_IFELSE_CMD;
extern int CODE7_GLOB_DEF_IN_BLOCK;
extern int CODE7_NEED_OPEN_BLOCK_CMD;
extern int CODE7_ARRAY_DOESNT_EXIST;
extern int CODE7_UNDEFINED_ARRAY_INCLUDE;
extern int CODE7_CONNECTING_TO_ARRAY_NOT_TO_ITEM;
extern int CODE7_OUT_OF_ARRAY_ITEMS;
extern int CODE7_GLOB_ARR_MUST_DEF;
extern int CODE7_GLOB_ARR_NOT_MATCH_DATA_TYPE ;
extern int CODE7_FN_RET_TYPE_NOT_MATCH;
extern int CODE7_FN_NOT_DEF;
extern int CODE7_MUST_BE_DEF_TYPE;
extern int CODE7_MUST_RETURN_TYPE;
//extern int CODE7_LEFT_IDENT_DEFINED;
//extern int CODE7_LEFT_IDENT_NOT_DEFINED;
//extern int CODE7_RIGHT_IDENT_NOT_DEFINED;

/* Yalnyshlyklaryn tekstleri */
extern error_item err_items[];
/*
 * Kompilyator ishlande yalnyshlyk cykan wagty,
 * Haty chap edyar
 *
 * @part - Kompilyatoryn bolumi
 * @num  - yalnyshlygyn nomeri
 * @file - Yalnyshlygyn chykan fayly
 * @line - Yalnyshlygyn chykan setiri
 * @ch   - Yalnyshlyk chykanda, parserin saklanan harpy
**/
void print_err(int num, token *tok);
error_item *get_err_inf(int num);

#endif
