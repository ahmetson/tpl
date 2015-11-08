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
#include "semantic/compare_token_types.h"
#include "semantic.h"
#include "translator_to_c/includes.h"


void tpl(int argc, const char **args)
{
	// Kodly faýl bolmasa, TPL işlänok
	source_codes_exist(argc);

    //printf("Fayllaryn sany: %d, <%s>\n", argc, args[1]);
	// Inisializasiýa
	init();


	// HER BERLEN KODLY FAÝLYŇ ALGORITMLERI ÝASALÝAR
	work_with_sources(argc, args);

    // Semantika barlanyar.
    // Sag tarapy konstanta maglumat bolan we chepi ulni bolan
    if (GLOB_RIGHT_DATA_CMDS_NUM)
    {
        compare_token_types_right_data();
    }
    if (UNKNOWN_CALLED_FNS_NUM)
    {
        work_with_unknown_fns();
    }

    if (UNKNOWN_TOKENS_NUM)
    {
        work_with_unknown_tokens();
    }
    if (UNKNOWN_CMDS_NUM)
    {
        work_with_unknown_cmds();
    }
    if (UNKNOWN_PARENS_NUM)
    {
        work_with_unknown_parens();
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
		print_err(CODE0_NOT_FOUND_MAIN_FILE, &inf_tok);
	}
	//printf("PARSERLER ISHLAP GUTARDY\n");

    // Eger ýasaljak programma birnäçe ýasalan kodlarda algoritmlerden düzülen bolsa,
    // Hemme ýasaljak kodlardaky algoritmler ýerine ýetiriler ýaly,
    // Baş ýasalan koddaky algoritmleriň soňlarynda, galan ýasalan kodlaryň
    // Algoritmlerini saklaýan funksiýalar (ýasalan kodly faýlyň adyna gabat gelýär)
    // çagyrylmaly
    if (MAIN_FILE_INCLUDES_NUM-1)
    {
        if (!add_addtn_file_fns())
            print_err(CODE0_INCLUDES_CANT_ADD, &inf_tok);
    }

    // Eger bir ýasaljak kodda näbelli ülňiler bar bolsa,
    // Ülňiler başga ýasaljak kodlarda yglan edilip bilinýär.
    if(UNKNOWN_USED_VARS_NUM)                           // Ýasaljak programmada näbelli ulanylýan ülňiler bar.
    {
        //printf("NACHE SANY: %s\n", UNKNOWN_USED_VARS[0].ident);
        //debug_GLOB_VAR_DEFS();

        if (!is_unknown_var_glob_def())                 // Näbelli ülňileriň arasynda başga
            print_err(CODE0_IDENT_NOT_DEFINED, &inf_tok);         // ýasaljak kodlarda yglan edilen global ülňi bar

        if (!identificate_unknown_vars_as_glob_vars())  // Ulanylan global ülňiler tanalar ýaly
            print_err(CODE0_IDENT_CANT_ADD, &inf_tok);            // Ýasaljak koda, ulanylan ülňileriň yglan edilen faýllary ýazylmaly
                                                        // Muny C dili talap edýär (#include "glob.h").
    }
    //printf("NABELLI ULNILER TANALDY (BAR BOLAN BOLSALAR)\n");

    if (GLOB_BOTH_IDENT_CMDS_NUM)
    {
        compare_token_types_both_ident();
    }


    /*
    // A->B, B->A yagdayy barlamak
    if (COMPARE_IDENTS_NUM)
    {
        check_a_to_b_b_to_b();
    }*/

    // ŞERT BÖLÜMI: TRANSLATOR TO Ç
    // ŞERT: Eger ýasalan kodlara #inklude preprosessor komandasyny ulanmaly bolsa, #inklud preprosessorynyň maglumatlary ýasalan
    //       kodlara salynýar.
    if (INCLUDES_NUM)
    {
        //printf("%d\n", INCLUDES_NUM);
        translator_to_c_add_includes();
    }
    //printf("NABELLI IDENTIFIKATORYN TIPLERI HEM BARLANDY\n");


	// TPL PROGRAMMADAN ÇYKMALY
	free_globs();
	printf("OK!\n");
}



