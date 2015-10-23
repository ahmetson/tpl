/* Programma:  TPL ( Turkmen|turkic|testable programming language )

   Awtory:     Medet Atabayev[milayter@milayt.com],
   Wersiýesi:  0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tpl.h"
#include "main/init.h"
#include "work_with_source.h"
#include "dev_debug.h"
#include "error.h"


char *tpl(int argc, const char **args)
{
	// Kodly faýl bolmasa, TPL işlänok
	source_codes_exist(argc);


	// Inisializasiýa
	init();


	// HER BERLEN KODLY FAÝLYŇ ALGORITMLERI ÝASALÝAR
	work_with_sources(argc, args);

    // Semantika barlanyar.
    // work_with_semantics();
    // Sag tarapy konstanta maglumat bolan we chepi ulni bolan
    if (GLOB_RIGHT_DATA_CMDS_NUM)
    {
        compare_token_types_right_data();
    }
    if (GLOB_BOTH_IDENT_CMDS_NUM)
    {
        compare_token_types_both_ident();
    }
    /*if (GLOB_LEFT_DATA_CMDS_NUM)
    {
        compare_token_types_left_data();
    }*/


    // Iki tarapy hem ulni bolan

    // Son, Chep tarapy konstanta maglumat  we sagy ulni bolan

	// Ýasalan programmanyň baş faýly tanalmaly
	if (!strlen(MAIN_FILE_NAME))
	{
		print_err(CODE0_NOT_FOUND_MAIN_FILE);
	}


    // Eger bir ýasaljak kodda näbelli ülňiler bar bolsa,
    // Ülňiler başga ýasaljak kodlarda yglan edilip bilinýär.
    if(UNKNOWN_USED_VARS_NUM)                           // Ýasaljak programmada näbelli ulanylýan ülňiler bar.
    {
        if (!is_unknown_var_glob_def())                 // Näbelli ülňileriň arasynda başga
            print_err(CODE0_IDENT_NOT_DEFINED);         // ýasaljak kodlarda yglan edilen global ülňi bar

        if (!identificate_unknown_vars_as_glob_vars())  // Ulanylan global ülňiler tanalar ýaly
            print_err(CODE0_IDENT_CANT_ADD);            // Ýasaljak koda, ulanylan ülňileriň yglan edilen faýllary ýazylmaly
                                                        // Muny C dili talap edýär (#include "glob.h").
    }

    // Eger ýasaljak programma birnäçe ýasalan kodlarda algoritmlerden düzülen bolsa,
    // Hemme ýasaljak kodlardaky algoritmler ýerine ýetiriler ýaly,
    // Baş ýasalan koddaky algoritmleriň soňlarynda, galan ýasalan kodlaryň
    // Algoritmlerini saklaýan funksiýalar (ýasalan kodly faýlyň adyna gabat gelýär)
    // çagyrylmaly
    if (MAIN_FILE_INCLUDES_NUM-1)
    {
        if (!add_addtn_file_fns())
            print_err(CODE0_INCLUDES_CANT_ADD);
    }

	// TPL PROGRAMMADAN ÇYKMALY
	free_globs();
	printf("OK!\n");
}



