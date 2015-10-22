/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler,
 *
 * Hem-de olary programmadan çykylanda boşadýan funksiýalar
**/
#include <stdio.h>
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


// Ýasaljak faýl boýunça yglan edilen ülňileriň sanawy.
int             USER_VAR_DEFS_NUM;
var_def_item   *USER_VAR_DEFS;


// Hemme kodly faýllardaky global ulanylan ülňileriň sanawy
long            UNKNOWN_USED_VARS_SIZE;
int             UNKNOWN_USED_VARS_NUM;
var_def_item   *UNKNOWN_USED_VARS;


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


/**
 * Iki sany maglumat birliklerden we ikinji birligi konstanta maglumat bolan komandany
 * barlamak üçin, şeýle komandalaryň sanawy.
**/
right_data_cmd_item    *GLOB_RIGHT_DATA_CMDS_LIST;
unsigned int            GLOB_RIGHT_DATA_CMDS_NUM;


/**
 * Ýasaljak programmadaky faýllaryň sanawy
**/
file_item *FILES;


/**
 * Bütin TPL boýunça ulanylýan ülňileriň kompýuteriň ýadynda eýelän ýerleri boşadylýar
**/
void free_globs(void)
{

    if (GLOB_RIGHT_DATA_CMDS_NUM)
    {
        free(GLOB_RIGHT_DATA_CMDS_LIST);
    }

	// Global yglan edilen ülňiler boşadylýar.
	if (USER_VAR_DEFS_NUM)
        free(USER_VAR_DEFS);

    // Ulanylan global ülňileriň sanawy boşadylýar
    if (UNKNOWN_USED_VARS_NUM)
        free(UNKNOWN_USED_VARS);

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

	// Komandanyň içindäki birlikler ýatda emeli usulda goýulýar.
	// Indi, olaryň alýan ýerleri boşadylýar.
	if (cmd.items_num)
        free(cmd.items);

}
