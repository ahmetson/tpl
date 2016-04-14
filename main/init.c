/**
 * TPL programmany ulanmazdan öň, ähli zatlary şol sanda geljekki algoritm üçin RAM'kada ýeri taýynlap goýýan bölüm
**/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

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
    GLOB_BLOCK_INCLUDES = GLOB_BLOCKS_NUM = 0;

    /// Prepare used paths
    prepare_paths();
	/// GCC compiler create
	prepare_gcc();

	/// C dilinin kody uchin papkany tayynlayar
	sys_mkdir( DIR_C_FOLDER, 1);

    /// Standard lib'däki funksiýalar goşulýar
    add_std_funs();

	return 1;
}

void    prepare_gcc()
{
    wchar_t archiver[ MAX_FILE_LEN ] = { 0 },
            archive[ MAX_FILE_LEN ] = { 0 };
    wcsncpys( archiver, DIR_TPL_BASE );
    wcsncat( archiver, L"7za.exe", wcslen( L"7za.exe" )+1 );
    wcsncpys( archive, DIR_TPL_BASE );
    wcsncat( archive, L"mingw.7z", wcslen( L"mingw.7z" ) );
    if ( !sys_is_dir_exists( DIR_GCC_FOLDER ) )
    {
        if( _waccess( archiver, F_OK ) != -1 && _waccess( archive, F_OK ) != -1  )
        {
            printf("%ls", L"C kompilýatory siziň kompýuteriňize ötürdilýär. Azajyk garaşyň...\n");
            wchar_t *sys_cmd = L"7za.exe x -y mingw.7z >nul 2>nul";		// >nul hides result, and
            if ( _wsystem(sys_cmd)<1 )
            {
                // Can't prepare GCC compiler
                CUR_PART = 0;
                print_err( CODE0_CANT_PREPARE_TPL, &inf_tok );
            }

        } else {
            // file doesn't exist

            CUR_PART = 0;
            print_err( CODE0_MISSING_REQUIRED_FILES, &inf_tok );
        }
    }
}

#include <stdio.h>  /* defines FILENAME_MAX */
#define WINDOWS
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _wgetcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

void prepare_paths()
{
    /// The directory where TPL called from.
    /// All additional programs (GCC) and directories (TEMPORARY FOLDER FOR C FILES) included in this folder
    get_tpl_main_dir();

    /// Folder, where stored temporary .C and .O files while making program
    wcsncpy( DIR_C_FOLDER, DIR_TPL_BASE, wcslen( DIR_TPL_BASE )+1 );
    wcsncat( DIR_C_FOLDER, L"tmp_c\\", wcslen( L"tmp_c\\" ) );

    /// Folder, where stored GCC program
    wcsncpy( DIR_GCC_FOLDER, DIR_TPL_BASE, wcslen( DIR_TPL_BASE )+1 );
    wcsncat( DIR_GCC_FOLDER, L"mingw\\", wcslen( L"mingw\\" ) );

    /// Path of GCC compiler
    wcsncpy( FILE_GCC_EXE, DIR_GCC_FOLDER, wcslen( DIR_GCC_FOLDER )+1 );
    wcsncat( FILE_GCC_EXE, L"bin\\gcc.exe", wcslen( L"bin\\gcc.exe" ) );

    /// Additional required arguments for commands, when using GCC compiler

    wchar_t add1[ MAX_FILE_LEN ] = {0},
            add2[ MAX_FILE_LEN ],
            add3[ MAX_FILE_LEN ];
    wcsncpy( add1, L" -I\"", wcslen( L" -I\"" )+1 );
    wcscat( add1, DIR_GCC_FOLDER );
    wcscat( add1, L"include\" " );
    wcsncpy( add2, L" -I\"", wcslen( L" -I\"" )+1 );
    wcscat( add2, DIR_GCC_FOLDER );
    wcscat( add2, L"x86_64-w64-mingw32\\include\" " );
    wcsncpy( add3, L" -I\"", wcslen( L" -I\"" )+1 );
    wcscat( add3, DIR_GCC_FOLDER );
    wcscat( add3, L"lib\\gcc\\x86_64-w64-mingw32\\4.9.2\\include\" " );

    wcsncpy( GCC_C_TO_O_ADDS, add1, wcslen( add1 )+1 );
    wcscat( GCC_C_TO_O_ADDS, add2 );
    wcscat( GCC_C_TO_O_ADDS, add3 );

    // Arguments used in GCC linker
    wchar_t add21[ MAX_FILE_LEN ] = {0},
            add22[ MAX_FILE_LEN ],
            add23[ MAX_FILE_LEN ];
    wcsncpy( add21, L" -L\"", wcslen( L" -I\"" )+1 );
    wcscat( add21, DIR_GCC_FOLDER );
    wcscat( add21, L"lib\" " );
    wcsncpy( add22, L" -L\"", wcslen( L" -I\"" )+1 );
    wcscat( add22, DIR_GCC_FOLDER );
    wcscat( add22, L"x86_64-w64-mingw32\\lib\" " );
    wcsncpy( add23, L"  -static-libgcc", wcslen( L"  -static-libgcc" )+1 );

    wcsncpy( GCC_O_TO_EXE_ADDS, add21, wcslen( add21 )+1 );
    wcscat( GCC_O_TO_EXE_ADDS, add22 );
    wcscat( GCC_O_TO_EXE_ADDS, add23 );

    /// Save full path of directory,
    /// Where TPL called from. ( The created .EXE must be saved in this directory )
    GetCurrentDir( CUR_DIR, sizeof( CUR_DIR ) );

    CUR_DIR[ wcslen( CUR_DIR )-1] = '\\'; /* not really required */
    CUR_DIR[ wcslen( CUR_DIR )] = '\0'; /* not really required */
}

void get_tpl_main_dir()
{
    wchar_t Buffer[ MAX_FILE_LEN ];

    GetModuleFileNameW( NULL, Buffer, MAX_FILE_LEN );

    // Find last dir delimeter
    int i;
    for ( i=wcslen( Buffer )-1; i>=0; --i )
    {
        if ( Buffer[ i ]==L'\\' )
        {
            break;
        }
    }
    // Put as folder name
    int j;
    for ( j=0; j<i+1; ++j )
    {
        DIR_TPL_BASE[ j ] = Buffer[ j ];
    }
    DIR_TPL_BASE[ wcslen( Buffer )-1 ] = L'\0';
}
