/*
 * 1. Kodly fayllary harplar boyuncha parsing edyan bolum
**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tpl.h"
#include "cmd.h"
#include "parser.h"
#include "algor.h"
#include "error.h"

/*
 * inisializasiya edilyar
**/
int init()
{
	// Hazirki fayl
	strncpy(cur_parse_file_name, "", strlen("")+1);
	// Hazirki setir
	cur_parse_line_num = 0;
	// Hazirki harp
	cur_parse_char = -1;
	
	init_cmd(&cmd);
	
	// Algoritmler uchin
	cur_file_algor_size = 0;
	//printf("%d is sizeof cmd, %d is size of cmd type\n", cur_file_algor_size), sizeof(command);
	cur_file_algor_cmds = 0;
	//cur_file_algor = malloc(cur_file_algor_size);

	// Global yglan edilen ulniler
	glob_def_vars_size = 0;
	glob_def_vars_cmds = 0;
	
	// Lokal ulniler uchin

	return 1;
}


/*
 * Programma argument gelendigini barlayar
**/
int has_argument(int argc)
{
	if (argc==1)
	{
		print_err(1 , CODE1_FILE_NOT_FOUND);
	}
	return 1;
}

/*
 * Her fayl boyuncha aylanylyp, parser chagyrylyar
**/
int read_source_file(const char *parse_file_name)
{
	int parser_answer;
	FILE *source = fopen(parse_file_name, "r");
		
	if (source==NULL)
	{
		print_err(1 , CODE1_FILE_CANT_OPEN);
	}
	parser_answer = parser(source);
	
	fclose(source);
}

