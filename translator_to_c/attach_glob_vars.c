/**
 * Ýasalan programmada, dürli-dürli ýasalan kodlaryň arasynda
 * ulanylýan global ülňileri biri-biri bilen baglanyşdyrýan bölüm.
 * Bölüm hemme algoritmler yglan edilen soň çagyrylmaly.
**/
#include <stdlib.h>
#include <string.h>

#include "attach_glob_vars.h"
#include "files.h"
#include "includes.h"
#include "../main/glob.h"



/**
 * Ýasalan kodly faýllarda, näbelli ulanylan ülňileriň yglan edilen ýasalan kodlaryna salgylanma ýasalýar.
 * (Bu şerti C programma talap edýär: #include "glob_var_def.h").
 *
**/
int glob_vars_def_files_include_add(glob_ident *gi)
{
    // Näbelli ülňileriň ulanylan faýly
    file_item *f;
    file_incs *fi;
    //wchar_t var_def_f[MAX_FILE_LEN];

    /** ÜLŇILER:
    //      näbelli ülňiniň çagyrylan faýlynyň inkludlanýan faýl atlarynyň sanawynyň adresi (1-nji ülňi)
    //      näbelli ülňiniň çagyrylan faýlynyň, faýllar sanawyndan maglumatlarynyň adresi (2-nji ülňi)
    //      näbelli ülňiniň yglan edilen faýlynyň .h faýlynyň ady (3-nji ülňi)
    // ALGORITMLER
    // Her näbelli yglan edilen ülňi boýunça:
    //  1)Näbelli ülňiniň çagyrylan faýlynyň ady boýunça faýlyň maglumatlarynyň adresi alynýar. (2-nji ülňä <-)
    //  2)Eger içine inklud etmeli faýllaryň sanawynda ülňiniň çagyrylan faýlynyň ady ýok bolsa (GLOB INKLUD ÜLŇI)
    //      a) Ülňiniň çagyrylan faýl ady boýunça, täze faýly inkludlara goşulmaly.
    //         inkludlara täze faýly goşýan funksiýa, goşulan faýlyň adresini gaýtarýar (1-nji ülňi <-)
    //    Ýogsa
    //      b) get_fn_link diýen funksiýa arkaly inklud etmeli faýllaryň sanawyndan
    //         häzirki näbelli ülňiniň çagyrylan faýlynyň adresi alynýar.  (1-nji ülňi <-)

    //  ŞERT #1: IÇINE INKLUD ETMELI FAÝLA, IÇINDE ÇAGYRYLAN ÜLŇILERIŇ YGLAN EDILEN FAÝLLARYNYŇ .H FAÝLLARYNY GOŞMALY.

    //  3) Näbelli ülňiniň .h faýlynyň adresi üçin ülňä ilki goşa dyrnak salynýar. Sebäbi inklud edilýän faýl ulanyjy tarapyndan yglan edilen
    //     (3-nji ülňi <-)
    //  4) Näbelli ülňiniň .h faýlynyň adresi alynýar. (3-nji ülňi <-)
    //  5) Näbelli ülňiniň .h faýlynyň soňundan goşa dyrnak dakylýar (3-nji ülňi <-).
    //  6) add_includes(inklud etmeli elementiň adresi, inklud etmeli maglumat) atly funksiýa arkaly
    //     .h faýlyň adresi, inklud etmeli elementiň inkludleriniň sanawyna goşulýar. (Funksiýa ilki goşmaly
    //     faýlyň inklud edilýän faýlda öň goşulanandygyny barlaýar. Eger goşulmadyk bolsa, onda goşýar).

    //  P.S add_includes_to_source diýen funksiýa, soň inklud etmeli faýllara, içinde inklud etmeli faýllaryň atlaryny goşýar.*/


    f = &FILES[gi->inf_file_num];

    // 2)
    if ((f->num+1)>INCLUDES_NUM)
    {
        // 2.a)
        fi = includes_add_new();
    }
    else
    {
        // 2.b)
        fi = &INCLUDES[f->num];
    }

    // #1

    // 3)
    wchar_t var_def_f[MAX_FILE_LEN] = {0},
            *dquote = L"\"";
    wcsncpy(var_def_f, dquote, wcslen(dquote)+1);

    // 4)
    wcsncat(var_def_f, f->h_source, wcslen(f->h_source));

    // 5)
    wcsncat(var_def_f, dquote, wcslen(dquote));

    // 6)
    includes_file_add_include(fi, var_def_f);

    return 1;
}
