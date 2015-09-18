/* Programma: TPL (Turkmen|turkic|testable programming language)

   Aftory:     Medet Atabayev[milayter@milayt.com],
   Versiyesi:  0.0.1
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tpl.h"
#include "init_and_work_with_files.h"
#include "tokens.h"
#include "cmd.h"
#include "algor.h"
#include "translator_to_c.h"
#include "fns.h"
#include "dev_debug.h"
#include "error.h"

// GLOBAL yglan edilen ulniler, butin programma boyuncha ulanylyar
	// komandanyn sony diymegi anladyar
char cmd_end = '.';

	// Programmanyn bash fayly
char main_file_name[];

	// Hazirki parsing edilyan faylyn maglumatlary
char cur_parse_file_name[];	
char cur_parse_char;
unsigned int  cur_parse_line_num;

	// Hazir TPL-in haysy bolumi kod bilen ishleyar
int CUR_PART;

	// Hazirki yasalyp duran komanda
command cmd;

	// Hazirki faylyn algoritmi
long cur_file_algor_size;		// Yatdaky algoritmin gowrumi
int cur_file_algor_cmds;		// Algoritmdaki komandalaryn sany
command *cur_file_algor;

	// Hazirki faylda yglan edilen ulnilerin sanawy
long loc_def_vars_size;
int loc_def_vars_num;
local_def_var *loc_def_vars; 

	// Butin programma bouyuncha yglan edilen global ulnilerin sanawy,
	// Yglan edilen faylynyn ady bilen yazylyar.
long glob_def_vars_size;
int glob_def_vars_cmds;
global_def_var *glob_def_vars;



char *tpl(int argc, const char **args)
{
	// eger kodly fayl berilmese, programma bashlanok
	has_argument(argc);

	// Inisializasiya
	init();
	
	int i;
	// Her fayl boyuncha parseri chagyrmaly
	for (i=1; i<argc; ++i)
	{
		//printf("Parsing etmeli: '%s'\n", args[i]);
		
		// Hazirki parsing edilyan fayl
		strncpy(cur_parse_file_name, args[i], strlen(args[i])+1);

		read_source_file(args[i]);

	}
	
	// Fayllar parsing edip bolanyndan son
	
	// Yasalmaly programmanyn bash fayly bolmaly
	if (!strlen(main_file_name))
	{
		print_err(CODE0_NOT_FOUND_MAIN_FILE);
	}
	//debug_glob_def_vars(glob_def_vars);
	free_globs();
	
	printf("OK!\n");
}

void free_globs(void)
{
	// Global ulnileri yatdan boshadyas
	if (glob_def_vars_size)
		free(glob_def_vars);
	
	free_locals();
}

// Lokal, dine hazirki fayla degishli maglumatlar uchin berlen yat boshadylyar
void free_locals(void)
{
	// Lokal maglumatlar pozulyar
	if (cur_file_algor_size)
	{
		cur_file_algor_size = 0;
		free(cur_file_algor);
	}
	// Lokal yglan edilen ulniler pozulyar
	if (loc_def_vars_num)
	{
		init_loc_def_vars();
		free(loc_def_vars);
	}	
}
