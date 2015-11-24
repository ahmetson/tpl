/** Programma:  TPL (tepe'el diýip okalýar) ( Turkmen|turkic|testable programming language )

   Awtory:     Medet Atabayev[milayter@milayt.com]
   Wersýesi:  0.0.1
 **/

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

/** ŞERT #1: Ülňiler global we lokal görnüşe öwrülmeli.
    Ülňiler ulanmazyldan öň, yglan edilmeli.

    *1) Global yglan edilip garaşylan ülňileriň sanawy bolýar. Sanawyň tipi: token.
    *1) Global yglan edilen ülňileriň sanawy bolýar. Sanawyň tipi: token.
    *2) Global ülňileri yglan edilen faýllaryň sanawy bolýar.

    *3) Pragma global ülňileriň yglab edilen faýlyny okaýan komanda goşulýar.
        a) Eger faýl öň goşulan faýllaryň arasynda bar bolsa (is_glob_defs_file_exist())
            I) Pragma gutarýar.
           ýogsa
            II) glob_defs_file_add() funksiýasy arkaly çagyrylan faýllaryň sanawyna goşulýar.
    *4)  Faýlyň içinde diňe lokal yglan edilen ülňileriň komandalry bolup bilýär.

    5) Eger komanda parsing edilen soň global ülňiniň yglan edilmesi diýip tanalsa
        a) Eger global yglan edilen ülňileriň arasynda şeýle atly ülňi bar bolsa (is_glob_var_def_exist())
            I) "Eýýäm yglan ediljek global ülňi, başga ýerde yglan edilipdir" diýip hat görkezilýär.
        b) Ýogsa
            I) Global ülňileriň sanawyna goşulýar. (glob_var_def_add())

    6) Täze komanda ýasalýar: Global ülňi çagyrylmagy (Nirede identifikator ulanyp bolýan bolsa, şol ýerde global ülňini çagyrsaň bolýar).
        Täze komandanyň tipini gaýtarýan funksiýa, is_glob_var_dec_exist() funksiýasy arkaly, ýok bolsa soň is_glob_var_def_exist()
        atly funksiýasy arkaly öň yglan edildimi ýa ýokdugy barlanýar.
        Eger yglan edilmedik bolsa, onda
            ýalňyşlyk görkezilýär.

    7) Parserler işläp gutaran soň, semantika barlanýan ýerde:
        Eger global ülňiniň maglumatlary yglan edilen sanaw boş bolmasa
            global ülňiniň maglumatlaryny global yglan edilen ülňileriň arasynda barlygy görülýär.
**/

void tpl(int argc, const char **args)
{
	// Kodly faýl bolmasa, TPL işlänok
	source_codes_exist(argc);

    //printf("Fayllaryn sany: %d, <%s>\n", argc, args[1]);
	// Inisializasiýa
	init();

	// HER BERLEN KODLY FAÝLYŇ ALGORITMLERI ÝASALÝAR
	work_with_sources(argc, args);

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
    if (GLOBAL_VAR_DECS_NUMS)
    {
        work_with_glob_decs();
    }

	// Ýasalan programmanyň baş faýly tanalmaly
	if (!strlen(MAIN_FILE_NAME))
	{
		print_err(CODE0_NOT_FOUND_MAIN_FILE, &inf_tok);
	}

    // Eger ýasaljak programma birnäçe ýasalan kodlarda algoritmlerden düzülen bolsa,
    // Hemme ýasaljak kodlardaky algoritmler ýerine ýetiriler ýaly,
    // Baş ýasalan koddaky algoritmleriň soňlarynda, galan ýasalan kodlaryň
    // Algoritmlerini saklaýan funksiýalar (ýasalan kodly faýlyň adyna gabat gelýär)
    // çagyrylmaly
    if (MAIN_FILE_INCLUDES_NUM)
    {
        if (!add_addtn_file_fns())
            print_err(CODE0_INCLUDES_CANT_ADD, &inf_tok);
    }

    /// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
    work_with_called_glob_vars();


    /// ŞERT BÖLÜMI: TRANSLATOR TO Ç
    /// ŞERT: Eger ýasalan kodlara #inklude preprosessor komandasyny ulanmaly bolsa, #inklud preprosessorynyň maglumatlary ýasalan
    ///       kodlara salynýar.
    /** ŞERT #3: Eger haýsy bolsada bir faýlda ulanylýan birlikleriň biri global yglan edilen ülňileri çagyrýan bolsa
                Onda faýlda ülňiniň yglan edilen faýlyna inklud etmeli

        TPL funksiýada, parsing işläp bolan soň,
    */
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



