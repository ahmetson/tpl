/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler,
 *
 * Hem-de olary programmadan çykylanda boşadýan funksiýalar
**/
#include <stdlib.h>

#include "glob.h"
#include "main_file.h"


// Global, komandanyň soňyny aňladýar
char CMD_END = '.';

// Hazirki yasalyp duran komanda
command         cmd;

// Parsing edilip duran faýlyň maglumatlary
int             CUR_PART;
char            CUR_FILE_NAME[];
int             CUR_FILE_NUM;               // Programma-da häzirki faýl näçinji orunda gelýär.
char            CUR_CHAR;
unsigned int    CUR_LINE;

// Parsing edilip duran faýldan emele geljek komandalar
long            CUR_ALGOR_SIZE;		        // Algoritmiň göwrümi
int             CUR_ALGOR_ITEMS_NUM;	    // Algoritmdaki komandalaryň sany
command        *CUR_ALGOR;                  // Algoritmiň poýnteri
// Parsing edilip duran faýlda yglan edilen ülňiler
long            LOCAL_VAR_DEFS_SIZE;
int             LOCAL_VAR_DEFS_NUM;
local_def_var  *LOCAL_VAR_DEFS;


// Hemme kodly faýllardaky global yglan edilen ülňileriň sanawy.
// Yglan edilen faýlynyň ady hem sanawa girizilýär.
long            GLOB_VAR_DEFS_SIZE;
int             GLOB_VAR_DEFS_NUM;
global_def_var *GLOB_VAR_DEFS;


// Hemme kodly faýllardaky global ulanylan ülňileriň sanawy
long            UNKNOWN_USED_VARS_SIZE;
int             UNKNOWN_USED_VARS_NUM;
global_def_var *UNKNOWN_USED_VARS;


/*
 * Komandanyn ichindaki komandalaryn birlikleri kuchada yerleshmeli
 * Programmadan chykylanda, olary kuchadan boshatmaly
 * Shonun uchin olaryn hemmesinin adresleri yatda saklanylyar.
 *
 * Global programma boyuncha yer boshadyan funksiya bu sanawdaky kuchadaky
 * Yerleri hem boshadyar
**/
command_item  **GLOB_SUBCMD_ITEMS_LIST;
unsigned int    GLOB_SUBCMDS_NUM;


/*
 * Ýasaljak programmadaky faýllaryň sanawy
**/
file_item *FILES;


/***
 * Bütin TPL boýunça ulanylýan ülňileriň kompýuteriň ýadynda eýelän ýerleri boşadylýar
**/
void free_globs(void)
{
	// Global yglan edilen ülňiler boşadylýar.
	if (GLOB_VAR_DEFS_SIZE) free(GLOB_VAR_DEFS);

    // Ulanylan global ülňileriň sanawy boşadylýar
    if (UNKNOWN_USED_VARS_NUM) free(UNKNOWN_USED_VARS);

    // Komandanyň içindäki bolup biljek komandalaryň birlikleri üçin ýerler boşadylýar
	if (GLOB_SUBCMDS_NUM)
	{
		int i;
		for (i=0; i<GLOB_SUBCMDS_NUM; ++i)
		{
			// Her komandanyň birlikleri üçin aýratyn ýer eýelenýär
			free(GLOB_SUBCMD_ITEMS_LIST[i]);
		}

		free(GLOB_SUBCMD_ITEMS_LIST);
	}

	// Baş faýla salmaly faýllar bilen funksiýalaryň sanawy
	free(MAIN_FILE_INCLUDES);

    // Faýllaryň sanawy hem arassalanýar
    if (CUR_FILE_NUM)
        free(FILES);

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
	}
	// Parsing edilýän faýlda yglan edilen ülňileriň sanawy boşadylýar
	if (LOCAL_VAR_DEFS_NUM && !(LOCAL_VAR_DEFS_SIZE = LOCAL_VAR_DEFS_NUM = 0)) free(LOCAL_VAR_DEFS);

	// Komandanyň içindäki birlikler ýatda emeli usulda goýulýar.
	// Indi, olaryň alýan ýerleri boşadylýar.
	if (cmd.items_num)                                                      free(cmd.items);


}
