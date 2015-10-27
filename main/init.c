/**
 * TPL programmany ulanmazdan öň, ähli zatlary şol sanda geljekki algoritm üçin RAM'kada ýeri taýynlap goýýan bölüm
**/
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "glob.h"
#include "main_file.h"
#include "../error.h"
#include "../fns.h"
#include "../translator_to_c.h"


/**
 * TPL programma hökman kodly faýllar berilmeli. Bu funksiýa bolsa kodly faýllaryň barmy ýa ýokdygyny barlaýar
 *
 * @argn - TPL programma berlen argumentleriň sany
**/
int source_codes_exist(int argn)
{
	if (argn>1)
		return 1;

	CUR_PART = 1;
	print_err(CODE1_FILE_NOT_FOUND, &inf_tok);
}

/**
 * Işe başlamazdan öňürti, hemme ulanyljak ülňiler taýynlanmaly.
**/
int init()
{
	// TPL ulgam bölüminden başlanýar
	CUR_PART = 0;

	// Häzirki  parsing edilýän: faýlyň ady, setiri, harpy
	strncpy(CUR_FILE_NAME, "", strlen("")+1);
	CUR_LINE = 0;
	CUR_CHAR = -1;
	CUR_CHAR_POS = 0;

	cmd_first_items_classes[0].type		 = TOKEN_ITEM;
	cmd_first_items_classes[0].item_class= TOK_CLASS_DEF_TYPE;
	cmd_first_items_classes[0].item_type = -1;
	cmd_first_items_classes[1].type=       TOKEN_ITEM;
	cmd_first_items_classes[1].item_class= TOK_CLASS_GLOB;
	cmd_first_items_classes[1].item_type = -1;
	cmd_first_items_classes[2].type=       CMD_ITEM;
	cmd_first_items_classes[2].item_class= CMD_CLASS_ASSIGN;
	cmd_first_items_classes[2].item_type = LEFT_ASSIGN_TOK_NUM;
	cmd_first_items_classes[3].type=       TOKEN_ITEM;
	cmd_first_items_classes[3].item_class= TOK_CLASS_IDENT;
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
	USER_VAR_DEFS_NUM  = 0;
	UNKNOWN_USED_VARS_SIZE = UNKNOWN_USED_VARS_NUM = 0;

    // Semantika üçin sag tarapy konstanta maglumat we çep tarapy konstanta maglumat bolmadyk komandalary
    // barlamak üçin
    GLOB_RIGHT_DATA_CMDS_NUM = 0;

    // Ýasaljak programma-da ulanylýan harpl tokenleriň sanawy
    GLOB_STRINGS_NUM = 0;
    GLOB_STRINGS     = NULL;

	// Fayllar
	CUR_FILE_NUM = 0;

    // Baş faýla salmaly faýllar bilen funksiýalar
    MAIN_FILE_INCLUDES_NUM = 1;
    MAIN_FILE_INCLUDES = malloc(sizeof(*MAIN_FILE_INCLUDES));

    init_token(&inf_tok);

    GLOB_SOURCE_CODES = NULL;

	// C dilinin kody uchin papkany tayynlayar
	sys_mkdir(C_SOURCE_FOLDER, 1);

	return 1;
}


