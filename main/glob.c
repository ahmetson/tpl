/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler,
 *
 * Hem-de olary programmadan çykylanda boşadýan funksiýalar
**/
#include <stdio.h>
#include <stdlib.h>

#include "glob.h"
#include "..\fns\fn.h"
#include "main_file.h"
#include "user_def_type.h"

int  TEST = 0;
wchar_t CHAR_UNDERSCORE = L'_';

// Global, komandanyň soňyny aňladýar
wchar_t CMD_END = L'.';
wchar_t TMP_CHAR = 0;

// Hazirki yasalyp duran komanda
command         cmd;
token           inf_tok;
command_item   *MAIN_CMD_ITEMS_LIST;

// Parsing edilip duran faýlyň maglumatlary
int             CUR_PART;
wchar_t            CUR_FILE_NAME[];
int             CUR_FILE_NUM;               // Programma-da häzirki faýl näçinji orunda gelýär.
wchar_t            CUR_CHAR;
int             CUR_CHAR_POS;
unsigned int    CUR_LINE;

// Parsing edilip duran faýldan emele geljek komandalar
long            CUR_ALGOR_SIZE;		        // Algoritmiň göwrümi
int             CUR_ALGOR_ITEMS_NUM;	    // Algoritmdaki komandalaryň sany
command        *CUR_ALGOR;                  // Algoritmiň poýnteri

/**
 * Komandanyn ichindaki komandalaryn birlikleri kuchada yerleshmeli
 * Programmadan chykylanda, olary kuchadan boshatmaly
 * Shonun uchin olaryn hemmesinin adresleri yatda saklanylyar.
 *
 * Global programma boyuncha yer boshadyan funksiya bu sanawdaky kuchadaky
 * Yerleri hem boshadyar
**/
command_item  **GLOB_SUBCMD_ITEMS_LIST;
unsigned int    GLOB_SUBCMDS_NUM;


/** Programmadaky kodlaryň setirleri. **/
wchar_t ***GLOB_SOURCE_CODES;


/// Global harpl ülňileriniň sanawy. Olaryň uzynlyklary näbelli bolup durýar.
wchar_t                  **GLOB_STRINGS;
unsigned long           GLOB_STRINGS_NUM;

/** Ýasaljak programmadaky faýllaryň sanawy **/
file_item              *FILES;

/** Kodlarda ulanylýan skobkalarda (parenthesis), näçe sany element boljagy näbelli.
    Şonuň üçin olar kuça-da (heap) ýerleşdirilýär. **/
parenthesis_elem     **GLOB_PARENTHS;
int                    GLOB_PARENTHS_NUM;

/** Funksiýalar TPL derejede mümkin. Şonuň üçin kuçada ýerleşdirilýär **/
func                  *FUNCS;
int                    FUNCS_NUM;
func_arg             **FUNC_ARGS;

/** Funksiýalar TPL derejede mümkin. Şonuň üçin kuçada ýerleşdirilýär **/
func                   TMP_FUNC;
int                    TMP_FUNC_NUM;
command               *TMP_FUNC_PARAMS;
command               *TMP_FUNC_ALGOR;
int                    TMP_FUNC_ALGOR_NUM;
glob_ident            *TMP_FUNC_VARS;
int                    TMP_FUNC_VARS_NUM;
array_item            *TMP_FUNC_ARRS;
int                    TMP_FUNC_ARRS_NUM;
array_inc_item       **TMP_FUNC_ARRS_ITEMS;

/** Diňe parsing edilip duran faýlyň döwründe işleýär.
    Diňe parsing edilen faýlyň içinde yglan edilen funksiýalaryň parametrleri **/
int                    LOC_FUNCS_NUM;
func                  *LOC_FUNCS;
command              **LOC_FUNCS_PARAMS;
command              **LOC_FUNCS_ALGOR;
glob_ident           **LOC_FUNCS_VARS;
int                   *LOC_FUNCS_VARS_NUM;
array_item           **LOC_FUNCS_ARRS;
int                   *LOC_FUNCS_ARRS_NUM;
int                   *LOC_FUNCS_ALGOR_NUM;
array_inc_item      ***LOC_FUNCS_ARRS_ITEMS;

/** Beýan edilýän faýlda yglan edilen funksiýalaryň maglumatlary **/
func                  *DEC_FUNCS;
int                    DEC_FUNCS_NUM;
func_arg             **DEC_FUNC_ARGS;

/// Goşmaly faýllaryň sanawy
file_incs             *INCLUDES;
int                    INCLUDES_NUM;

/// Ýasaljak programma boýunça global yglan edilen ülňileriň sanawy
glob_ident          *GLOB_VAR_DEFS;
int                  GLOB_VAR_DEFS_NUMS;

/// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
glob_ident          *GLOB_VAR_DECS;
int                  GLOB_VAR_DECS_NUMS;

/// Ýasaljak kodly faýlda yglan edilen ülňileriň maglumatlarynyň sanawy
glob_ident          *LOCAL_VAR_DEFS;
int                  LOCAL_VAR_DEFS_NUMS;

/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
called_var         *GLOB_CALLED_VARS;
int                 GLOB_CALLED_VARS_NUM;


/// Ýasaljak programma boýunça global yglan edilen birsyhly sanawlaryň sanawy
array_item          *GLOB_ARR_DEFS;
int                  GLOB_ARR_DEFS_NUMS;

/// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen birsyhly sanawlaryň maglumatlarynyň sanawy
array_inc_item     **GLOB_ARR_DEFS_ITEMS;

array_item          *GLOB_ARR_DECS;
int                  GLOB_ARR_DECS_NUMS;

array_inc_item     **GLOB_ARR_DECS_ITEMS;


/// Ýasaljak programma boýunça global yglan edilen birsyhly sanawlaryň sanawy
array_inc_item     **LOCAL_ARR_DEFS_ITEMS;

/// Ýasaljak programma boýunça global yglan edilen birsyhly sanawlaryň sanawy
array_item          *LOCAL_ARR_DEFS;
int                  LOCAL_ARR_DEFS_NUMS;

/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
called_var         *GLOB_CALLED_ARRS;
int                 GLOB_CALLED_ARRS_NUM;


/// Global yglan etmeli faýllaryň sanawy
wchar_t               **GLOB_DECS_FILES;
int                  GLOB_DECS_FILES_NUM;

/// Häzirki içine parsing edilip goşulýan birlikli komanda
command             *CUR_CMD;

command_item        *TMP_CMD_ITEMS_LIST;


/// Bloklaryň içindeligini görkezýän ülňi
int                  GLOB_BLOCK_INCLUDES;


/// Bloklaryň içindeliginiň sanawy
block_inc           *GLOB_BLOCKS;
int                  GLOB_BLOCKS_NUM;


/// Çap etmek üçin ulanylýar
char                **PRINTED_STRINGS = NULL;
int                 PRINTED_STRINGS_NUM = 0;

/// Paths
wchar_t DIR_TPL_BASE[ MAX_FILE_LEN ],
        DIR_C_FOLDER[ MAX_FILE_LEN ],
        DIR_GCC_FOLDER[ MAX_FILE_LEN ],
        FILE_GCC_EXE[ MAX_FILE_LEN ],
        GCC_C_TO_O_ADDS[ MAX_FILE_LEN*4 ],
        GCC_O_TO_EXE_ADDS[ MAX_FILE_LEN*3 ],
        CUR_DIR[ MAX_FILE_LEN ],
        FILE_OUT_EXE[ MAX_FILE_LEN ];

/** Bütin TPL boýunça ulanylýan ülňileriň kompýuteriň ýadynda eýelän ýerleri boşadylýar */
void free_globs(void)
{
    int i;
    if (USER_DEF_TYPES_NUM)
    {
        for (i=0; i<USER_DEF_TYPES_NUM; ++i)
        {
            /// Eger tipiň sanaw birlikleri bar bolsa
            if (USER_DEF_TYPES_ARRS_NUMS[i])
            {
                if (USER_DEF_TYPES_ARRS[i]!=NULL)
                {
                    free(USER_DEF_TYPES_ARRS[i]);
                }
                if (USER_DEF_TYPES_ARRS_ITEMS[i]!=NULL)
                {
                    int j;
                    for (j=0; j<USER_DEF_TYPES_ARRS_NUMS[i]; ++j)
                        free(USER_DEF_TYPES_ARRS_ITEMS[i][j]);
                    free(USER_DEF_TYPES_ARRS_ITEMS[i]);
                }
            }
            if (USER_DEF_TYPE_ITEMS[i]!=NULL)
                free(USER_DEF_TYPE_ITEMS[i]);
        }
        free(USER_DEF_TYPES_ARRS_ITEMS);
        free(USER_DEF_TYPES_ARRS);
        free(USER_DEF_TYPE_ITEMS);
        free(USER_DEF_TYPES_ARRS_NUMS);
    }
    free_tmp_user_type();

    if (GLOB_CALLED_ARRS_NUM)
    {
        for(i=0; i<GLOB_CALLED_ARRS_NUM; ++i)
        {
            if (GLOB_CALLED_ARRS[i].num)
            {
                free(GLOB_CALLED_ARRS[i].ident);
            }
        }
        free(GLOB_CALLED_ARRS);
    }

    if (GLOB_CALLED_VARS_NUM)
    {
        for(i=0; i<GLOB_CALLED_VARS_NUM; ++i)
        {
            if(GLOB_CALLED_VARS[i].num)
            {
                free(GLOB_CALLED_VARS[i].ident);
            }
        }
        free(GLOB_CALLED_VARS);
    }

    // Komandanyň içindäki bolup biljek komandalaryň birlikleri üçin ýerler boşadylýar
	if (GLOB_SUBCMDS_NUM)
	{
		for (i=0; i<GLOB_SUBCMDS_NUM; ++i)
		{
            if (GLOB_SUBCMD_ITEMS_LIST[i]!=NULL)
			free(GLOB_SUBCMD_ITEMS_LIST[i]);
		}
		free(GLOB_SUBCMD_ITEMS_LIST);
	}

    if (GLOB_PARENTHS_NUM)
    {
        for (i=0; i<GLOB_PARENTHS_NUM; ++i)
		{
			// Her komandanyň birlikleri üçin aýratyn ýer eýelenýär
			if (GLOB_PARENTHS[i]!= NULL)
                free(GLOB_PARENTHS[i]);
		}

        free(GLOB_PARENTHS);
    }

    // Ýasaljak kodda ulanyljak harpl tokenleriň maglumatlarynyň sanawy
	if (GLOB_STRINGS_NUM)
    {
        for (i=0; i<GLOB_STRINGS_NUM; ++i)
		{
		    if (GLOB_STRINGS[i]!=NULL)
			free(GLOB_STRINGS[i]);
		}
		free(GLOB_STRINGS);
    }

	// Baş faýla salmaly faýllar bilen funksiýalaryň sanawy
	free(MAIN_FILE_INCLUDES);

    // Faýllaryň sanawy hem arassalanýar
    if (CUR_FILE_NUM)
    {
        for(i=0; i<CUR_FILE_NUM; ++i)
        {
            if (GLOB_SOURCE_CODES[i]!=NULL)
            {
                free(GLOB_SOURCE_CODES[i]);
            }
            free(GLOB_SOURCE_CODES);
        }
        free(FILES);
    }

    if(FUNCS_NUM)
    {
        free(FUNCS);
        for(i=0; i<FUNCS_NUM; ++i)
        {
            free(FUNC_ARGS[i]);
        }
        free(FUNC_ARGS);
    }

    if (DEC_FUNCS_NUM)
    {
        for (i=0; i<DEC_FUNCS_NUM; ++i)
        {
            free(DEC_FUNC_ARGS[i]);
        }
        free(DEC_FUNC_ARGS);
        free(DEC_FUNCS);
    }


    if (INCLUDES_NUM)
    {
        for(i=0; i<INCLUDES_NUM; ++i)
        {
            if (INCLUDES[i].num)
                free(INCLUDES[i].inc);
        }
        free(INCLUDES);
    }

    /// Ýasaljak programma boýunça global yglan edilen ülňileriň sanawy
    if (GLOB_VAR_DEFS_NUMS)
        free(GLOB_VAR_DEFS);
    if (GLOB_ARR_DEFS_NUMS)
    {
        free(GLOB_ARR_DEFS);
        int i;
        for (i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
        {
            if (GLOB_ARR_DEFS_ITEMS[i]!=NULL)
            {
                free(GLOB_ARR_DEFS_ITEMS[i]);
            }
        }
        free(GLOB_ARR_DEFS_ITEMS);
    }

    /// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
    if (GLOB_VAR_DECS_NUMS)
        free(GLOB_VAR_DECS);
    if (GLOB_ARR_DECS_NUMS)
    {
        free(GLOB_ARR_DECS);
        int i;
        for (i=0; i<GLOB_ARR_DECS_NUMS; ++i)
        {
            if (GLOB_ARR_DECS_ITEMS[i]!=NULL)
            {
                free(GLOB_ARR_DECS_ITEMS[i]);
            }
        }
        free(GLOB_ARR_DECS_ITEMS);
    }

    /// Global yglan etmeli faýllaryň sanawy
    if (GLOB_DECS_FILES_NUM)
    {
        for (i=0; i<GLOB_DECS_FILES_NUM; ++i)
        {
            if (GLOB_DECS_FILES[i]!=NULL)
            free(GLOB_DECS_FILES[i]);
        }
        free(GLOB_DECS_FILES);
    }
    /*if (COMPARE_IDENTS_NUM)
        free(COMPARE_IDENTS);*/

	// Diňe parsing edilýän wagty ulanylýan ülpileriň ýerleri boşadylýar
	free_locals();
}


/***
 * Diňe parsing edilip duran wagty ulanylýan ýerler boşadylýar.
**/
void free_locals(void)
{
	// Parsing edilýän faýlyň komandalarynyň sanawy boşadylýar
	if (CUR_ALGOR_SIZE)
    {
        CUR_ALGOR_SIZE = CUR_ALGOR_ITEMS_NUM = 0;
        free(CUR_ALGOR);
        CUR_ALGOR = NULL;
	}

	// Komandanyň içindäki birlikler ýatda emeli usulda goýulýar.
	// Indi, olaryň alýan ýerleri boşadylýar.
	if (cmd.items_num)
    {
        free(MAIN_CMD_ITEMS_LIST);
        MAIN_CMD_ITEMS_LIST = NULL;
    }

    if (LOCAL_VAR_DEFS_NUMS)
    {
        LOCAL_VAR_DEFS_NUMS=0;
        free(LOCAL_VAR_DEFS);
        LOCAL_VAR_DEFS = NULL;
    }
    if (LOCAL_ARR_DEFS_NUMS)
    {
        int i;
        for (i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
            if (LOCAL_ARR_DEFS_ITEMS[i]!=NULL)
            {
                free(LOCAL_ARR_DEFS_ITEMS[i]);
            }
        }
        LOCAL_ARR_DEFS_NUMS=0;
        free(LOCAL_ARR_DEFS);
        LOCAL_ARR_DEFS = NULL;

        free(LOCAL_ARR_DEFS_ITEMS);
        LOCAL_ARR_DEFS_ITEMS = NULL;
    }
    free_tmp_fn();
    free_local_fns();
}

void free_tmp_fn()
{
    if (TMP_FUNC_PARAMS!=NULL)
    {
        free(TMP_FUNC_PARAMS);
        TMP_FUNC_PARAMS = NULL;
    }
    if (TMP_FUNC_ALGOR_NUM)
    {
        free(TMP_FUNC_ALGOR);
        TMP_FUNC_ALGOR  = NULL;
        TMP_FUNC_ALGOR_NUM = 0;
    }
    if (TMP_FUNC_ARRS_NUM)
    {
        int i, j;
        for (i=0; i<TMP_FUNC_ARRS_NUM; ++i)
        {
            free(TMP_FUNC_ARRS_ITEMS[i]);
        }
        free(TMP_FUNC_ARRS_ITEMS);
        free(TMP_FUNC_ARRS);
        TMP_FUNC_ARRS  = NULL;
        TMP_FUNC_ARRS_NUM = 0;
        TMP_FUNC_ARRS_ITEMS = NULL;
    }
    if (TMP_FUNC_VARS_NUM)
    {
        free(TMP_FUNC_VARS);
        TMP_FUNC_VARS  = NULL;
        TMP_FUNC_VARS_NUM = 0;
    }
    --TMP_FUNC_NUM;
}


void free_local_fns()
{
    if (LOC_FUNCS_NUM)
    {
        int i;  // funksiýanyň nomeri
        for (i=0; i<LOC_FUNCS_NUM; ++i)
        {
            int j, z;   /// j - sanawyň nomeri
            for (j=0; j<LOC_FUNCS_ARRS_NUM[i]; ++j)
            {
                free(LOC_FUNCS_ARRS_ITEMS[i][j]);
            }
            free(LOC_FUNCS_ARRS_ITEMS[i]);
            free(LOC_FUNCS_ALGOR[i]);
            free(LOC_FUNCS_PARAMS[i]);
            free(LOC_FUNCS_ARRS[i]);
            free(LOC_FUNCS_VARS[i]);
        }
        free(LOC_FUNCS_ALGOR);
        LOC_FUNCS_ALGOR = NULL;
        free(LOC_FUNCS_ALGOR_NUM);
        LOC_FUNCS_ALGOR_NUM = NULL;
        free(LOC_FUNCS_ARRS);
        LOC_FUNCS_ARRS = NULL;
        free(LOC_FUNCS_ARRS_ITEMS);
        LOC_FUNCS_ARRS_ITEMS = NULL;
        free(LOC_FUNCS_ARRS_NUM);
        LOC_FUNCS_ARRS_NUM = NULL;
        free(LOC_FUNCS_VARS);
        LOC_FUNCS_VARS = NULL;
        free(LOC_FUNCS_VARS_NUM);
        LOC_FUNCS_VARS_NUM = NULL;
        free(LOC_FUNCS_PARAMS);
        LOC_FUNCS_PARAMS = NULL;
        free(LOC_FUNCS);
        LOC_FUNCS = NULL;
        LOC_FUNCS_NUM = 0;
    }
}

