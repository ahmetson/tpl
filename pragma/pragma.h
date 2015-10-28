/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
 *
 * Pragmalar, duzulinde eyerilyan kanunlar:
 * pragmanyn sony hokman san bilen gutarmaly we ichinde san bolmaly dal.
**/
#ifndef PRAGMA_H
#define PRAGMA_H

#define MAX_PRAGMA_LEN 3

typedef struct {
	char name[MAX_PRAGMA_LEN];
	int is_compl;
}pragma;

typedef struct{
	char name[MAX_PRAGMA_LEN];
	void (*act)(pragma *prag);
}act_pragma_item;

// Pragmalaryn sany
extern int PRAGMAS_NUM;

// Pragmalar name harpdan bashlayar
extern char PRAGMA_START_CHAR;
// Pragmalar name harpdan gutaryar
extern char PRAGMA_END_CHAR;

// Pragma saygaryan parser boluminin kody
extern char PRAGMA_MODE;

// Programmanyn tanayan we ishleyan pragmalarynyn sanawy
extern char *pragmas[];

// Parserde pragma bilen işleýän bölüm
int work_with_pragma(pragma *prag, pragma *prev_pragma, char *mode, char c, int c_pos, int line);

// Pragmany ulanmana tayynlayar.
void init_pragma(pragma *prag);

// Pragmany tanayan funksiya
int recognise_pragma(pragma *prag);

void act_pragma(pragma *prag);				// Pragma uchin kompilyator bir zat eder
void act_pragma_main_file(pragma *prag);	// Esasy fayl barada kompilyator bir zat eder
#endif
