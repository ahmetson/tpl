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
 * Işe başlamazdan öňürti, hemme ulanyljak ülňiler taýynlanmaly.
 * Şu funksiýa arkaly
**/
int init()
{
	// TPL ulgam bölüminden başlanýar
	CUR_PART = 0;

	// Häzirki  parsing edilýän: faýlyň ady, setiri, harpy
	strncpy(CUR_FILE_NAME, "", strlen("")+1);
	CUR_LINE = 0;
	CUR_CHAR = -1;

	cmd_first_items_classes[0].type		 = TOKEN_ITEM;
	cmd_first_items_classes[0].item_class= DEF_TYPE_TYPE_CLASS;
	cmd_first_items_classes[0].item_type = -1;
	cmd_first_items_classes[1].type=       TOKEN_ITEM;
	cmd_first_items_classes[1].item_class= GLOB_TYPE_CLASS;
	cmd_first_items_classes[1].item_type = -1;
	cmd_first_items_classes[2].type=       CMD_ITEM;
	cmd_first_items_classes[2].item_class= ASSIGN_CLASS_NUM;
	cmd_first_items_classes[2].item_type = LEFT_ASSIGN_TOK_NUM;
	cmd_first_items_classes[3].type=       TOKEN_ITEM;
	cmd_first_items_classes[3].item_class= IDENT_TYPE_CLASS;
	cmd_first_items_classes[3].item_type = -1;
	init_cmd(&cmd, 0);

	// Algoritmler uchin
	CUR_ALGOR_SIZE = 0;
	//printf("%d is sizeof cmd, %d is size of cmd type\n", CUR_ALGOR_SIZE), sizeof(command);
	CUR_ALGOR_ITEMS_NUM = 0;
	//CUR_ALGOR = malloc(CUR_ALGOR_SIZE);

	// Butin programma boyuncha ichki komandalaryn sanawy.
	GLOB_SUBCMDS_NUM = 0;

	// Global yglan edilen ulniler
	GLOB_VAR_DEFS_SIZE = GLOB_VAR_DEFS_NUM = 0;

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

