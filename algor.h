/*
 * Algoritmli fayly yasayan funksiya
**/
#ifndef ALGOR_H
#define ALGOR_H

#define MAX_FILE_LEN 255
#define MAX_TOKEN_LEN 32

#include "cmds.h"
#include "tokens.h"

typedef struct{
	char file_name[MAX_FILE_LEN];		// Haysy faylda ulni yglan edilipdir
	char tok_name[MAX_TOKEN_LEN];		// Ulninin ady
	int tok_class;						// Ulninin gornushi
	int tok_type;
}global_def_var;

typedef struct{
	char tok_name[MAX_TOKEN_LEN];
	int tok_class;
	int tok_type;
}local_def_var;

// Algoritmde komandany goshyar
int algor_add_cmd(command cmd);

// Global ulni komanda goshulyar
int glob_vars_add_cmd(command cmd);


int loc_vars_add_cmd(command cmd);


// Wagtlayyn fayllaryn rasshireniyasyny
extern char loc_head_file[MAX_FILE_LEN];
extern char loc_source_file[MAX_FILE_LEN];

//void write_loc_def_var();

// Hazirki parsing edilen faylda global ulni yglan edileninin jogabyny gaytaryar
int is_glob_def_var_in_cur();
/*
 * Ulninin ady boyuncha onun on yglan edilenmidigini barlayar.
**/
int is_ident_used(char *ident);
#endif
