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
extern char *parts_names[];


/* Yalnyshlykly tekstlerin nomerleri */
extern int CODE0_PRAGMA_ACT_NOT_IDENTIFIED;
extern int CODE0_MAIN_FILE_ALREADY_CHOSED;
extern int CODE0_NOT_FOUND_MAIN_FILE;

extern int CODE1_FILE_NOT_FOUND;
extern int CODE1_FILE_CANT_OPEN;

extern int CODE2_UNKNOWN_TOKENS_CHAR;
extern int CODE2_PRAGMA_NOT_END;
extern int CODE2_PRAGMA_NOT_IDENT;
extern int CODE2_REMAIN_TOKEN;
extern int CODE2_TOKEN_TOO_BIG;

extern int CODE3_PREV_TOK_INCORRECT;

extern int CODE4_CMD_HASNT_FIRST_TOKEN;
extern int CODE4_TOO_MANY_TOKENS;
extern int CODE4_CANT_IDENT_CMD;


/* Yalnyshlyklaryn tekstleri */
extern char *err_texts[][10];
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
void print_err(int num);

#endif
