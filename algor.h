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
	char file_name[MAX_FILE_LEN];		// Ülňi haýsy faýla degişli
	int  file_num;		                // Ülňi haýsy faýla degişli
	int  line;
	int  start_char_position;
	char start_char;
	char ident[MAX_TOKEN_LEN];		    // Ülňiniň ady
	int  tok_class;						// Ülňiniň tipi
	int  tok_type;
	int  ns;
}var_def_item;


// Algoritmde komandany goshyar
int algor_add_cmd(command cmd);

int add_user_var_def_item(command cmd);

// Wagtlayyn fayllaryn rasshireniyasyny
extern char loc_head_file[MAX_FILE_LEN];
extern char loc_source_file[MAX_FILE_LEN];

//void write_loc_def_var();

// Hazirki parsing edilen faylda global ulni yglan edileninin jogabyny gaytaryar
int is_glob_def_var_in_cur();

/*
 * Ulninin ady boyuncha onun on yglan edilenmidigini barlayar.
**/
int is_ident_used(char *ident, int ns);
#endif
