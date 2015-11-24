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
#define PRAGMA_MAIN_FILE "b1"
#define PRAGMA_INCLUDE_GLOB_DEC '@'
#define PRAGMA_ADDR_QUOTE '"'
#define PRAGMA_INCLUDE_GLOB_DEC_FILE "@\""
#define PRAGMAS_NUM 2
#define MAX_PRAGMA_LEN 3
#endif // PRAGMA_MAIN_FILE

typedef struct {
	char name[MAX_PRAGMA_LEN+MAX_FILE_LEN];
	int is_compl;
}pragma;

typedef struct{
	char name[MAX_PRAGMA_LEN+MAX_FILE_LEN];
	void (*act)(pragma *prag);
}act_pragma_item;

// Pragmalar name harpdan bashlayar
extern char PRAGMA_START_CHAR;
// Pragmalar name harpdan gutaryar
extern char PRAGMA_END_CHAR;

// Pragma saygaryan parser boluminin kody
extern char PRAGMA_MODE;

// Programmanyn tanayan we ishleyan pragmalarynyn sanawy
extern char *pragmas[];

// Parserde pragma bilen işleýän bölüm
void parse_pragma(FILE *s);

// Pragmany ulanmana tayynlayar.
void init_pragma(pragma *prag);

// Pragmany tanayan funksiya
int recognise_pragma(pragma *prag);

void act_pragma(pragma *prag);				        // Pragma uchin kompilyator bir zat eder
void act_pragma_main_file(pragma *prag);	        // Esasy fayl barada kompilyator bir zat eder
void act_pragma_include_glob_decl_file(pragma *p);	// Global yglan edilen ülňiler babatda maglumatlar
#endif
