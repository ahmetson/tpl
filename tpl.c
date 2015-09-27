/* Programma:  TPL (Turkmen|turkic|testable programming language)

   Awtory:     Medet Atabayev[milayter@milayt.com],
   Wersiýesi:  0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tpl.h"
#include "init_and_work_with_files.h"
#include "tokens.h"
#include "cmds.h"
#include "algor.h"
#include "translator_to_c.h"
#include "fns.h"
#include "dev_debug.h"
#include "error.h"

// Global, komandanyň soňyny aňladýar
char CMD_END = '.';
// Ýasaljak programmanyň baş faýlynyň ady
char MAIN_FILE_NAME[];
// Parsing edilip duran faýlyň maglumatlary
int             CUR_PART;
char            CUR_FILE_NAME[];
char            CUR_CHAR;
unsigned int    CUR_LINE;
// Hazirki yasalyp duran komanda
command         cmd;
// Parsing edilip duran faýldan emele geljek komandalar
long            CUR_ALGOR_SIZE;		// Algoritmiň göwrümi
int             CUR_ALGOR_ITEMS_NUM;	// Algoritmdaki komandalaryň sany
command        *CUR_ALGOR;             // Algoritmiň poýnteri
// Parsing edilip duran faýlda yglan edilen ülňiler
long            LOCAL_VAR_DEFS_SIZE;
int             LOCAL_VAR_DEFS_NUM;
local_def_var  *LOCAL_VAR_DEFS;

// Hemme kodly faýllardaky global yglan edilen ülňileriň sanawy.
// Yglan edilen faýlynyň ady hem sanawa girizilýär.
long            GLOB_VAR_DEFS_SIZE;
int             GLOB_VAR_DEFS_NUM;
global_def_var *GLOB_VAR_DEFS;


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


char *tpl(int argc, const char **args)
{
	// eger kodly fayl berilmese, programma bashlanok
	has_argument(argc);

	// Inisializasiya
	init();

	int i;
	// Her faýl parserden geçmeli
	for (i=1; i<argc; ++i)
	{
		//printf("Parsing etmeli: '%s'\n", args[i]);
		// Häzirki parsing edilmeli faýlyň ady
		strncpy(CUR_FILE_NAME, args[i], strlen(args[i])+1);

		read_source_file(args[i]);
	}

    // Hemme faýllar parsing edilip bolundy.
    // Ýasaljak programmanyň hemme faýllaryna degişli boljak işler edilmeli

	// 1) Ýasalan programmanyň baş faýly tanalmaly
	if (!strlen(MAIN_FILE_NAME))
	{
		print_err(CODE0_NOT_FOUND_MAIN_FILE);
	}

	// PROGRAMMADAN ÇYKMALY
	free_globs();

	printf("OK!\n");
}


/*
 * Bütin TPL boýunça ulanylýan ülňileriň kompýuteriň ýadynda eýelän ýerleri boşadylýar
**/
void free_globs(void)
{
	// Global yglan edilen ülňiler boşadylýar.
	if (GLOB_VAR_DEFS_SIZE) free(GLOB_VAR_DEFS);

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

	// Diňe parsing edilýän wagty ulanylýan ülpileriň ýerleri boşadylýar
	free_locals();
}


/*
 * Diňe parsing edilip duran wagty ulanylýan ýerler boşadylýar.
**/
void free_locals(void)
{
	// Parsing edilýän faýlyň komandalarynyň sanawy boşadylýar
	if (CUR_ALGOR_SIZE &&     !(CUR_ALGOR_SIZE = CUR_ALGOR_ITEMS_NUM = 0))     free(CUR_ALGOR);

	// Parsing edilýän faýlda yglan edilen ülňileriň sanawy boşadylýar
	if (LOCAL_VAR_DEFS_NUM && !(LOCAL_VAR_DEFS_SIZE = LOCAL_VAR_DEFS_NUM = 0)) free(LOCAL_VAR_DEFS);

	// Komandanyň içindäki birlikler ýatda emeli usulda goýulýar.
	// Indi, olaryň alýan ýerleri boşadylýar.
	if (cmd.items_num)                                                      free(cmd.items);
}
