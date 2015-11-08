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
#include "../fns/3rdparty/std.h"


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
    return 0;
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

    // C translator, ýasaljak kodlarda inklud edilmeli faýl atlary
    INCLUDES = NULL;
    INCLUDES_NUM = 0;

    init_token(&inf_tok);

    GLOB_SOURCE_CODES = NULL;

    GLOB_PARENTHS     = NULL;
    GLOB_PARENTHS_NUM = 0;

    USER_VAR_DEFS_NUM = 0;
    USER_VAR_DEFS = NULL;

    FUNCS = NULL;
    FUNCS_NUM = 0;
    FUNC_ARGS = NULL;
    FUNC_ARGS_NUM = 0;

    UNKNOWN_CALLED_FNS_NUM = 0;
    UNKNOWN_CALLED_FNS = NULL;

    UNKNOWN_TOKENS_NUM = UNKNOWN_PARENS_NUM = UNKNOWN_CMDS_NUM = 0;
    UNKNOWN_TOKENS = NULL;
    UNKNOWN_PARENS = NULL;
    UNKNOWN_CMDS = NULL;
    // Standard lib'däki faňksiýeler goşulýar
    add_std_funs();

    // A->B, B->A yagday bolmaz yaly
    //COMPARE_IDENTS = NULL;
    //COMPARE_IDENTS_NUM = 0;

	// C dilinin kody uchin papkany tayynlayar
	sys_mkdir(C_SOURCE_FOLDER, 1);

	return 1;
}


