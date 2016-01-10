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


/** TPL programma hökman kodly faýllar berilmeli. Bu funksiýa bolsa kodly faýllaryň barmy ýa ýokdygyny barlaýar
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

/** Işe başlamazdan öňürti, hemme ulanyljak ülňiler taýynlanmaly.
**/
int init()
{
    wchar_t *empty = L"";
	// TPL ulgam bölüminden başlanýar
	CUR_PART = 0;

	// Häzirki  parsing edilýän: faýlyň ady, setiri, harpy
	wcsncpy(CUR_FILE_NAME, empty, wcslen(empty)+1);
	CUR_LINE = 0;
	CUR_CHAR = -1;
	CUR_CHAR_POS = 0;

    MAIN_CMD_ITEMS_LIST = NULL;
    TMP_CMD_ITEMS_LIST = NULL;
	init_cmd(&cmd, 0);
	cmd.items = -1;

	// Algoritmler uchin
	CUR_ALGOR_SIZE = 0;
	//printf(L"%d is sizeof cmd, %d is size of cmd type\n", CUR_ALGOR_SIZE), sizeof(command);
	CUR_ALGOR_ITEMS_NUM = 0;
	//CUR_ALGOR = malloc(CUR_ALGOR_SIZE);

	// Butin programma boyuncha ichki komandalaryn sanawy.
	GLOB_SUBCMDS_NUM = 0;

    // Ýasaljak programma-da ulanylýan harpl tokenleriň sanawy
    GLOB_STRINGS_NUM = 0;
    GLOB_STRINGS     = NULL;

	// Fayllar
	CUR_FILE_NUM = 0;

    // Baş faýla salmaly faýllar bilen funksiýalar
    MAIN_FILE_INCLUDES_NUM = 0;
    MAIN_FILE_INCLUDES = NULL;

    // C translator, ýasaljak kodlarda inklud edilmeli faýl atlary
    INCLUDES = NULL;
    INCLUDES_NUM = 0;

    init_token(&inf_tok);

    GLOB_PARENTHS     = NULL;
    GLOB_PARENTHS_NUM = 0;


    FUNCS = NULL;
    FUNCS_NUM = 0;
    FUNC_ARGS = NULL;

    TMP_FUNC_NUM = -1;
    free_tmp_fn();
    free_local_fns();


    /** Beýan edilýän faýlda yglan edilen funksiýalaryň maglumatlary **/
    DEC_FUNCS = NULL;
    DEC_FUNCS_NUM = 0;
    DEC_FUNC_ARGS = NULL;

    // Standard lib'däki faňksiýeler goşulýar
    add_std_funs();

    GLOBAL_VAR_DEFS = NULL;
    GLOBAL_ARR_DEFS = NULL;
    GLOBAL_VAR_DEFS_NUMS = 0;
    GLOBAL_ARR_DEFS_NUMS = 0;
    GLOBAL_ARR_DEFS_ITEMS = NULL;
    GLOBAL_ARR_DECS_ITEMS = NULL;

    /// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
    GLOBAL_VAR_DECS = NULL;
    GLOBAL_ARR_DECS = NULL;
    GLOBAL_VAR_DECS_NUMS = 0;
    GLOBAL_ARR_DECS_NUMS = 0;

    /// Global yglan etmeli faýllaryň sanawy
    GLOB_DECS_FILES = NULL;
    GLOB_DECS_FILES_NUM = 0;

    LOCAL_VAR_DEFS = NULL;
    LOCAL_ARR_DEFS = NULL;
    LOCAL_VAR_DEFS_NUMS = 0;
    LOCAL_ARR_DEFS_NUMS = 0;
    LOCAL_ARR_DEFS_ITEMS = NULL;

    GLOBAL_CALLED_VARS = NULL;
    GLOBAL_CALLED_ARRS = NULL;
    GLOBAL_CALLED_VARS_NUM = 0;
    GLOBAL_CALLED_ARRS_NUM = 0;

    GLOB_BLOCK_INCLUDES = 0;

    /// Bloklaryň içindeliginiň sanawy
    GLOB_BLOCKS = NULL;
    GLOB_BLOCKS_NUM = 0;

	// C dilinin kody uchin papkany tayynlayar
	sys_mkdir(C_SOURCE_FOLDER, 1);

	return 1;
}


