/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
 *
 * Pragmalar, duzulinde eyerilyan kanunlar:
 * pragmanyn sony hokman san bilen gutarmaly we ichinde san bolmaly dal.
**/
#ifndef PRAGMA_H
#define PRAGMA_H
#include "../main/glob.h"
#ifndef PRAGMA_MAIN_FILE
#define PRAGMA_MAIN_FILE L"b1"
#define PRAGMA_INCLUDE_GLOB_DEC L'@'
#define PRAGMA_ADDR_QUOTE L'"'
#define PRAGMA_INCLUDE_GLOB_DEC_FILE L"@\""
#define PRAGMA_TYPES_NUM 2
#define MAX_PRAGMA_LEN 3
#endif // PRAGMA_MAIN_FILE

typedef struct {
	wchar_t name[MAX_PRAGMA_LEN+MAX_FILE_LEN];
	int is_compl;
}pragma;

typedef struct{
	wchar_t name[MAX_PRAGMA_LEN+MAX_FILE_LEN];
	void (*act)(pragma *prag);
}act_pragma_item;

// Pragmalar name harpdan bashlayar
extern wchar_t PRAGMA_START_CHAR;
// Pragmalar name harpdan gutaryar
extern wchar_t PRAGMA_END_CHAR;

// Pragma saygaryan parser boluminin kody
extern wchar_t PRAGMA_MODE;

// Programmanyn tanayan we ishleyan pragmalarynyn sanawy
extern wchar_t *pragmas[];

// Parserde pragma bilen işleýän bölüm
void parse_pragma(FILE *s);

// Pragmany ulanmana tayynlayar.
void init_pragma(pragma *prag);

// Pragmany tanayan funksiya
int recognise_pragma(pragma *prag);

void act_pragma(pragma *prag);				        // Pragma uchin kompilyator bir zat eder
void act_pragma_main_file(pragma *prag);	        // Esasy fayl barada kompilyator bir zat eder
void act_pragma_include_glob_decl_file(pragma *p);	// Global yglan edilen ülňiler babatda maglumatlar

int parser_mode_pragma( FILE *s );
#endif
