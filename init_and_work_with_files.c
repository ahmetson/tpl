/*
 * 1. Kodly fayllary harplar boyuncha parsing edyan bolum
**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tpl.h"
#include "cmds.h"
#include "parser.h"
#include "algor.h"
#include "translator_to_c.h"
#include "error.h"

/*
 * TPL-in ishlemezden on, ozuni duzmeleshdiryar we taynlayar
**/
int init()
{
	// TPL bashlanda, bitarap bolumden bashlayar
	CUR_PART = 0;
	
	// Hazirki fayl
	strncpy(cur_parse_file_name, "", strlen("")+1);
	// Hazirki setir
	cur_parse_line_num = 0;
	// Hazirki harp
	cur_parse_char = -1;
	
	init_cmd(&cmd, 0);
	
	// Algoritmler uchin
	cur_file_algor_size = 0;
	//printf("%d is sizeof cmd, %d is size of cmd type\n", cur_file_algor_size), sizeof(command);
	cur_file_algor_cmds = 0;
	//cur_file_algor = malloc(cur_file_algor_size);

	// Global yglan edilen ulniler
	glob_def_vars_size = glob_def_vars_cmds = 0;
	
	// Lokal ulniler uchin

	// C dilinin kody uchin papkany tayynlayar
	sys_mkdir(C_SOURCE_FOLDER, 1);

	return 1;
}


/*
 * TPL-e kodly fayllar berlenini barlayar argument gelendigini barlayar
**/
int has_argument(int argc)
{
	if (argc>1)
		return 1;
	
	CUR_PART = 1;
	print_err(CODE1_FILE_NOT_FOUND);
	
}

/*
 * Her fayl boyuncha aylanylyp, parser chagyrylyar
**/
int read_source_file(const char *parse_file_name)
{
	FILE *source = fopen(parse_file_name, "r");
		
	if (source==NULL)
	{
		CUR_PART = 1;
		//printf("Init we fayllar bilen ishleyan bolumde");
		print_err(CODE1_FILE_CANT_OPEN);
	}
	
	// 2-nji TPL-in bolumine gechildi
	parser(source);
	
	fclose(source);
}

