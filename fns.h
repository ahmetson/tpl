#ifndef FNS_H
#define FNS_H
#include <stdio.h>

// Concatinate string with char and put them to @to
int strstrchcat(char *to, char *from, char c);

// Checks first occurance of substring in string started from offset
int strstr_by_offset(const char *string, const char*sub, unsigned int offset);


// SISTEMA BILEN ISHLEYAN ULNILER
// Taze papka yasayar
// Eger ikinji argument dogry bolsa,
// 	Onki papkany pozyar.
int sys_mkdir(char *folder_name, char rm_prev_dir);
// Onki papkany pozyar
int sys_rmdir(char *folder_name);

/*
 * Berlen sozun bolmaly uzynlygyndaky,
 * ichindaki hemme harplaryn deregine \0 bilen dolduryar.
 *
 * Eger-de bolmaly uzynlygy tanalmasy, yagny argument -1 bolsa,
 * ol strlen() funksiyasy arkaly tanalyar.
 *   Shonda dine sozde birinji \0 dushyancha hemme harplaryn deregine \0 goyular
**/
char *empty_string(char *f, int len);

char *remove_dirnames(char *f);
char *remove_ext(char *f, char *e);

void return_last_char(FILE *f);

void divide_string(char *source, char d, char ***out, int *items);

#endif
