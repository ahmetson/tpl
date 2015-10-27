/**
 * Ýasalan programmada, dürli-dürli ýasalan kodlaryň arasynda
 * ulanylýan global ülňileri biri-biri bilen baglanyşdyrýan bölüm.
 * Bölüm hemme algoritmler yglan edilen soň çagyrylmaly.
**/
#include <stdlib.h>
#include <string.h>

#include "attach_glob_vars.h"
#include "files.h"
#include "../main/glob.h"


/**
 * Näbelli yglan edilen ülňiler başga ýasalan kodlarda yglan edilen global ülňidiginiň jogabyny gaýtarýar.
**/
int is_unknown_var_glob_def()
{
    int i;
    for(i=0; i<UNKNOWN_USED_VARS_NUM; ++i)
    {
        if (!is_ident_used(UNKNOWN_USED_VARS[i].ident, -1))
        {
            inf_tok.inf_char = UNKNOWN_USED_VARS[i].start_char;
            inf_tok.inf_char_num = UNKNOWN_USED_VARS[i].start_char_position;
            inf_tok.inf_line_num = UNKNOWN_USED_VARS[i].line;
            inf_tok.inf_file_num = UNKNOWN_USED_VARS[i].file_num;
            return 0;
        }
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
    typedef struct{
        char file_name[255];
        char (*included_files)[255];
        int included_files_num;
    }used_glob_var_file;
    int i, j, z, len, found = 0;
    int file_num = 1;
    used_glob_var_file *used_glob_vars_files;
    int            used_glob_vars_num = 1;
    used_glob_vars_files = malloc(sizeof(used_glob_var_file)*UNKNOWN_USED_VARS_NUM);

    // Näçe sany içinde ülňi eden faýl bardygy tanalýar.
    for (i=0; i<UNKNOWN_USED_VARS_NUM; ++i)
    {
        char *f = UNKNOWN_USED_VARS[i].file_name; //get_used_glob_file_name(UNKNOWN_USED_VARS[i].file_name);

        // Sanawda şeýle atly faýl ýok bolsa, içine goşulýar
        for (j=0; j<used_glob_vars_num-1; ++j)
        {
            len = strlen(f)>strlen(used_glob_vars_files[j].file_name)?strlen(f):strlen(used_glob_vars_files[j].file_name);
            // Ulanylan ülňiniň faýly eýýäm goşulypdyr.
            if (strncmp(f, used_glob_vars_files[j].file_name, len)==0)
            {
                found = 1;
            }
        }
        if (!found)
        {
            ++used_glob_vars_num;
            strncpy(used_glob_vars_files[used_glob_vars_num-2].file_name, f, strlen(f)+1);
            used_glob_vars_files[used_glob_vars_num-2].included_files_num=1;
            used_glob_vars_files[used_glob_vars_num-2].included_files = malloc(sizeof(long)*UNKNOWN_USED_VARS_NUM);
        }
        found = 0;
    }
    --used_glob_vars_num;

    // Her içinde başga faýlda yglan edilen ülňileri ulanýan faýl üçin,
    // içinde ulanylýan ülňileriň yglan edilen faýllarynyň sanawy düzülýär.
    for (i=0; i<used_glob_vars_num; ++i)
    {
        for(j=0; j<UNKNOWN_USED_VARS_NUM; ++j)
        {
            //printf("%s faylda, %s ulanyldy\n", UNKNOWN_USED_VARS[i].file_name, UNKNOWN_USED_VARS[i].ident);
            len = strlen(UNKNOWN_USED_VARS[j].file_name)>strlen(used_glob_vars_files[i].file_name)?
                strlen(UNKNOWN_USED_VARS[j].file_name):strlen(used_glob_vars_files[i].file_name);
            if (strncmp(UNKNOWN_USED_VARS[j].file_name, used_glob_vars_files[i].file_name, len)!=0)
                continue;

            char *f = get_header_source_by_source(get_used_glob_file_name(UNKNOWN_USED_VARS[j].ident));

            //printf("%s headeri goshmaly %d-%d\n", f, UNKNOWN_USED_VARS_NUM, file_num);

            // Faýllar sanawyň içine goşmaly
            for (z=0; z<used_glob_vars_files[i].included_files_num-1; ++z)
            {
                len = strlen(f)>strlen(used_glob_vars_files[i].included_files[z])?
                    strlen(f):strlen(used_glob_vars_files[i].included_files[z]);
                // Ulanylan ülňiniň faýly eýýäm goşulypdyr.
                if (strncmp(f, used_glob_vars_files[i].included_files[z], len)==0)
                {
                    found = 1;
                }

            }
            if (!found)
            {
                ++used_glob_vars_files[i].included_files_num;
                strncpy(used_glob_vars_files[i].included_files[used_glob_vars_files[i].included_files_num-2], f, strlen(f)+1);
            }
            found = 0;
        }
        used_glob_vars_files[i].included_files_num--;
    }


    // Fayla yazmaly
    for(i=0; i<used_glob_vars_num; ++i)
    {
        FILE *s = fopen(get_c_source_by_source(used_glob_vars_files[i].file_name), "r+");

        add_includes_to_source(s, used_glob_vars_files[i].included_files, used_glob_vars_files[i].included_files_num);

        /*for (j=0; j<used_glob_vars_files[i].included_files_num; ++j)
        {
            printf("'%s' fayla '%s' fayly goshmaly\n", used_glob_vars_files[i].file_name,
                   used_glob_vars_files[i].included_files[j]);
        }*/

        fclose(s);
    }

    // Pamyat boşadylýar
    for (i=0; i<used_glob_vars_num; ++i)
    {
        free(used_glob_vars_files[i].included_files);
    }
    free(used_glob_vars_files);

    // Ülňi hiç yglan edilmändir.

    return 1;
}
