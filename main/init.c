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


/** Funksiýa bolsa kodly faýllaryň barmy ýa ýokdygyny barlaýar*/
void source_codes_exist( int argn )
{
	if ( !argn )
    {
        CUR_PART = 1;
        print_err(CODE1_FILE_NOT_FOUND, &inf_tok);
    }
}


/** Işe başlamazdan öňürti, hemme ulanyljak ülňiler taýynlanmaly.*/
int init()
{
    atexit(free_globs);

    wchar_t *empty = L"";
	/// Maglumat üçin ulanylýan informasiýa
	CUR_PART =
	CUR_LINE =
	CUR_CHAR_POS =
    CUR_ALGOR_SIZE =
	CUR_ALGOR_ITEMS_NUM =
	CUR_FILE_NUM = 0;
	CUR_CHAR = -1;
	wcsncpys( CUR_FILE_NAME, empty );
	/// Maglumatlary çap edýän funksiýa
	init_token(&inf_tok);

    MAIN_CMD_ITEMS_LIST = NULL;
    TMP_CMD_ITEMS_LIST = NULL;

    /// Global komandany saklamak üçin ulanylýan ülňi
    init_cmd(&cmd, 0);
	cmd.items = -1;

	/// Içki komandalaryň sanawy
	GLOB_SUBCMDS_NUM = 0;
	/// Içki sözlemleriň sanawy
    GLOB_STRINGS_NUM = 0;
    GLOB_STRINGS     = NULL;
    /// Içki ýaýlaryň sanawy
    GLOB_PARENTHS     = NULL;
    GLOB_PARENTHS_NUM = 0;

	/// Baş faýla galan kodlaryň maglumatlaryny çagyrar ýaly faýllar bilen funksiýalarynyň sanawy
    MAIN_FILE_INCLUDES_NUM = 0;
    MAIN_FILE_INCLUDES = NULL;

    /// Hemme ýasaljak kodlara salmaly faýl atlary
    INCLUDES = NULL;
    INCLUDES_NUM = 0;

    /** Program boýunça ähli ulanyp bolýan funksiýalaryň sanawy */
    FUNCS = NULL;
    FUNCS_NUM = 0;
    FUNC_ARGS = NULL;

    TMP_FUNC_NUM = -1;
    free_tmp_fn();
    free_local_fns();


    /** Beýan edilýän faýlda yglan edilen funksiýalaryň maglumatlary */
    DEC_FUNCS = NULL;
    DEC_FUNCS_NUM = 0;
    DEC_FUNC_ARGS = NULL;

    /// Global yglan etmeli faýllaryň sanawy
    GLOB_DECS_FILES = NULL;
    GLOB_DECS_FILES_NUM = 0;


    /// Ýasaljak programda yglan edilen Ülňileriň, sanawlaryň maglumatlary
    GLOB_VAR_DEFS = NULL;
    GLOB_ARR_DEFS = NULL;
    GLOB_VAR_DEFS_NUMS = 0;
    GLOB_ARR_DEFS_NUMS = 0;
    GLOB_ARR_DEFS_ITEMS = NULL;
    GLOB_ARR_DECS_ITEMS = NULL;

    GLOB_VAR_DECS = NULL;
    GLOB_ARR_DECS = NULL;
    GLOB_VAR_DECS_NUMS = 0;
    GLOB_ARR_DECS_NUMS = 0;

    LOCAL_VAR_DEFS = NULL;
    LOCAL_ARR_DEFS = NULL;
    LOCAL_VAR_DEFS_NUMS = 0;
    LOCAL_ARR_DEFS_NUMS = 0;
    LOCAL_ARR_DEFS_ITEMS = NULL;

    GLOB_CALLED_VARS = NULL;
    GLOB_CALLED_ARRS = NULL;
    GLOB_CALLED_VARS_NUM = 0;
    GLOB_CALLED_ARRS_NUM = 0;

    /// Bloklaryň içindeliginiň sanawy
    GLOB_BLOCKS = NULL;
    GLOB_BLOCK_INCLUDES =
        GLOB_BLOCKS_NUM = 0;

	/// C dilinin kody uchin papkany tayynlayar
	sys_mkdir(C_SOURCE_FOLDER, 1);

    /// Standard lib'däki funksiýalar goşulýar
    add_std_funs();

	return 1;
}


