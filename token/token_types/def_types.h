/*
 * Programma dilde belli bolan sada tipler we olara degishli maglumatlar
**/
#ifndef DEF_TYPES_H
#define DEF_TYPES_H

#include "../token_structs.h"

// Sada tiplerin in uzynyn bolup biljeginin uzynlygy
#define DEF_TYPES_LEN 7

typedef struct{
	const char tk_name[DEF_TYPES_LEN];
	const char value[DEF_TYPES_LEN];
	const char init_val[DEF_TYPES_LEN];
}def_type;

// Programmada belli bolan tiplerin sany
extern const int DEF_TYPES_NUM;

// tiplering atlarynyn spisogy
extern def_type def_type_list[];


#endif
