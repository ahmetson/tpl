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

int  TEST = 0;
char CHAR_UNDERSCORE = '_';

// Global, komandanyň soňyny aňladýar
char CMD_END = '.';

// Hazirki yasalyp duran komanda
command         cmd;
token           inf_tok;

// Parsing edilip duran faýlyň maglumatlary
int             CUR_PART;
char            CUR_FILE_NAME[];
int             CUR_FILE_NUM;               // Programma-da häzirki faýl näçinji orunda gelýär.
char            CUR_CHAR;
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
char ***GLOB_SOURCE_CODES;


/// Global harpl ülňileriniň sanawy. Olaryň uzynlyklary näbelli bolup durýar.
char                  **GLOB_STRINGS;
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

/** Funksiýalaryň argumentleri **/
func_arg             **FUNC_ARGS;
int                    FUNC_ARGS_NUM; // Goşulan argumentleriň sanawy

/// Goşmaly faýllaryň sanawy
file_incs             *INCLUDES;
int                    INCLUDES_NUM;

/// Ýasaljak programma boýunça global yglan edilen ülňileriň sanawy
glob_ident          *GLOBAL_VAR_DEFS;
int                  GLOBAL_VAR_DEFS_NUMS;

/// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
glob_ident          *GLOBAL_VAR_DECS;
int                  GLOBAL_VAR_DECS_NUMS;

/// Ýasaljak kodly faýlda yglan edilen ülňileriň maglumatlarynyň sanawy
glob_ident          *LOCAL_VAR_DEFS;
int                  LOCAL_VAR_DEFS_NUMS;

/// Global yglan etmeli faýllaryň sanawy
char               **GLOB_DECS_FILES;
int                  GLOB_DECS_FILES_NUM;

/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
called_var         *GLOBAL_CALLED_VARS;
int                 GLOBAL_CALLED_VARS_NUM;

/** Bütin TPL boýunça ulanylýan ülňileriň kompýuteriň ýadynda eýelän ýerleri boşadylýar */
void free_globs(void)
{
    int i;

    if (GLOBAL_CALLED_VARS_NUM)
    {
        for(i=0; i<GLOBAL_CALLED_VARS_NUM; ++i)
        {
            if(GLOBAL_CALLED_VARS[i].num)
            {
                free(GLOBAL_CALLED_VARS[i].ident);
            }
        }
        free(GLOBAL_CALLED_VARS);
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

    // Komandanyň içindäki bolup biljek komandalaryň birlikleri üçin ýerler boşadylýar
	if (GLOB_SUBCMDS_NUM)
	{
		for (i=0; i<GLOB_SUBCMDS_NUM; ++i)
		{
			free(GLOB_SUBCMD_ITEMS_LIST[i]);
		}
		free(GLOB_SUBCMD_ITEMS_LIST);
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
        for(i=0; i<FUNC_ARGS_NUM; ++i)
        {
            free(FUNC_ARGS[i]);
        }
        free(FUNC_ARGS);
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
    if (GLOBAL_VAR_DEFS_NUMS)
        free(GLOBAL_VAR_DEFS);

    /// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
    if (GLOBAL_VAR_DECS_NUMS)
        free(GLOBAL_VAR_DECS);

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
        free(cmd.items);
        cmd.items = NULL;
    }

    if (LOCAL_VAR_DEFS_NUMS)
    {
        LOCAL_VAR_DEFS_NUMS=0;
        free(LOCAL_VAR_DEFS);
        LOCAL_VAR_DEFS = NULL;
    }
}
