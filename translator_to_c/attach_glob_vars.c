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
 * Näbelli yglan edilen ülňiler başga ýasalan kodlarda yglan edilen global ülňidiginiň jogabyny gaýtarýar.
**/
int is_unknown_var_glob_def()
{
    int i;
    for(i=0; i<UNKNOWN_USED_VARS_NUM; ++i)
    {
        token t; init_token(&t);
        t.inf_file_num = UNKNOWN_USED_VARS[i].file_num;
        token_type ty;
        strncpy(ty.value, UNKNOWN_USED_VARS[i].ident, strlen(UNKNOWN_USED_VARS[i].ident)+1);
        add_potentional_token_type(&t, ty);
        if (!is_ident_used(&t,1))
        {
            inf_tok.inf_char = UNKNOWN_USED_VARS[i].start_char;
            inf_tok.inf_char_num = UNKNOWN_USED_VARS[i].start_char_position;
            inf_tok.inf_line_num = UNKNOWN_USED_VARS[i].line;
            inf_tok.inf_file_num = UNKNOWN_USED_VARS[i].file_num;
            return 0;
        }
        init_token(&t);
    }

    // Ülňi hiç yglan edilmändir.
    return 1;
}


/**
 * Ýasalan kodly faýllarda, näbelli ulanylan ülňileriň yglan edilen ýasalan kodlaryna salgylanma ýasalýar.
 * (Bu şerti C programma talap edýär: #include "glob_var_def.h").
 *
**/
int identificate_unknown_vars_as_glob_vars()
{
    // Näbelli ülňileriň ulanylan faýly
    file_item *f;
    file_incs *fi;
    //char var_def_f[MAX_FILE_LEN];

    int i;

    // ÜLŇILER:
    //      näbelli ülňiniň çagyrylan faýlynyň inkludlanýan faýl atlarynyň sanawynyň adresi (1-nji ülňi)
    //      näbelli ülňiniň çagyrylan faýlynyň, faýllar sanawyndan maglumatlarynyň adresi (2-nji ülňi)
    //      näbelli ülňiniň yglan edilen faýlynyň .h faýlynyň ady (3-nji ülňi)
    // ALGORITMLER
    // Her näbelli yglan edilen ülňi boýunça:
    //  1)Näbelli ülňiniň çagyrylan faýlynyň ady boýunça faýlyň maglumatlarynyň adresi alynýar. (2-nji ülňä <-)
    //  2)Eger içine inklud etmeli faýllaryň sanawynda ülňiniň çagyrylan faýlynyň ady ýok bolsa (GLOBAL INKLUD ÜLŇI)
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

    //  P.S add_includes_to_source diýen funksiýa, soň inklud etmeli faýllara, içinde inklud etmeli faýllaryň atlaryny goşýar.


    for (i=0; i<UNKNOWN_USED_VARS_NUM; ++i)
    {
        // 1) Näbelli ülňiniň ulanylan faýlynyň maglumatlary
        f = &FILES[UNKNOWN_USED_VARS[i].file_num];

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
        char var_def_f[MAX_FILE_LEN] = {0};
        strncpy(var_def_f, "\"", strlen("\"")+1);

        // 4)
        strncat(var_def_f, get_header_source_by_source(get_used_glob_file_name(UNKNOWN_USED_VARS[i].ident)), strlen(get_header_source_by_source(get_used_glob_file_name(UNKNOWN_USED_VARS[i].ident))));

        // 5)
        strncat(var_def_f, "\"", strlen("\""));
        //printf("%s\n", var_def_f);

        // 6)
        includes_file_add_include(fi, var_def_f);

    }

    return 1;
}
