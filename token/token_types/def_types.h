/*
 * Programma dilde belli bolan sada tipler we olara degishli maglumatlar
**/
#ifndef DEF_TYPES_H
#define DEF_TYPES_H

#include "../token_structs.h"

// Sada tiplerin in uzynyn bolup biljeginin uzynlygy
#define DEF_TYPES_LEN (sizeof(wchar_t)*7)

typedef struct{
	wchar_t tk_name[DEF_TYPES_LEN];
	wchar_t value[DEF_TYPES_LEN];
	wchar_t init_val[DEF_TYPES_LEN];
}def_type;

// Programmada belli bolan tiplerin sany
extern const int DEF_TYPES_NUM;

// tiplering atlarynyn spisogy
extern def_type def_type_list[];

int is_token_def_type           (token *tok, wchar_t *tok_val);


#endif
