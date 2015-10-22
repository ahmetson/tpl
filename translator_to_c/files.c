/**
 * Ýasalan kodlar bilen işleýän funksiýalar hem-de
 * Ýasalan kodlara degişli global ülňilerdäki magtlumatlar bilen işleýän funksiýalar
**/
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "../fns.h"
#include "../main/glob.h"


/**
 * Ýasalan koddaky global ülňiniň yglan edilen faýlynyň adyny gaýtarýar.
 *
 * @ident - global ülňiniň ady
**/
char *get_used_glob_file_name(char *ident)
{
    int i, len;
    for(i=0; i<USER_VAR_DEFS_NUM; ++i)
    {
        if (USER_VAR_DEFS[i].ns!=GLOB)
            continue;
        len = (strlen(ident)<strlen(USER_VAR_DEFS[i].ident))?strlen(USER_VAR_DEFS[i].ident):strlen(ident);
        if (strncmp(USER_VAR_DEFS[i].ident, ident, len)==0)
            return USER_VAR_DEFS[i].file_name;
    }
    return "";
}


/**
 * Faýllaryň sanawyndan, berlen faýlyň adyna görä, faýlyň ýasalan headery gaýtarylýar
**/
char *get_header_source_by_source(char *s)
{
    int i, len;
    for (i=0; i<CUR_FILE_NUM; i++)
    {
        len = (strlen(s)<strlen(FILES[i].source))?strlen(FILES[i].source):strlen(s);
        if (strncmp(FILES[i].source, s, len)==0)
            return remove_dirnames(FILES[i].h_source);
    }
    return "";
}

/**
 * Faýllaryň sanawyndan, berlen faýlyň adyna görä, faýlyň ýasalan headery gaýtarylýar
**/
char *get_c_source_by_source(char *s)
{
    int i, len;
    for (i=0; i<CUR_FILE_NUM; i++)
    {
        len = (strlen(s)<strlen(FILES[i].source))?strlen(FILES[i].source):strlen(s);
        if (strncmp(FILES[i].source, s, len)==0)
            return FILES[i].c_source;
    }
    return "";
}


/**
 * Ýasalan kodyň içine: #include "" goýýar.
 *
 * @s               - Içine goýmaly ýasalan kod.
 * @included_files  - #include komanda-da ulanylmaly faýl atlary
 * @files_num       - #include komanda-da ulanylýan faýl atlarynyň möçberi
**/
int add_includes_to_source(FILE *s, char (*included_files)[255], int files_num)
{
	int i, j;
	int len = 1, c_len = 1;

	char c, prev_c = '\0', prev_c2 = '\0',
        cmode = 0;

	char *l = malloc(len);
	char *ct = malloc(c_len);

	while ((c=fgetc(s))!=EOF)
	{
		if (prev_c=='/' && prev_c2=='/' && c=='.')
		{
			l[len-1] = '\0';
			len--;
			l[len-1] = '\0';
			len--;

			cmode = 1;
			continue;
		}

		if (cmode==1)
		{
		    if (c=='\n')
			{
				cmode=2;
				continue;
			}
			ct[c_len-1] = c;

			c_len++;
			ct = realloc(ct, c_len);
		}
		else if(!cmode)
		{
			l[len-1] = c;
			prev_c2 = prev_c;
			prev_c = c;

			len++;
			l = realloc(l, len);
		}
		else
		{
            // Fayllar goshulyar
		    for(i=0; i<files_num; ++i)
            {
                //printf("Goshmaly:%s %d\n", included_files[i], files_num);
                char putme[270];
                strncpy(putme, "#include \"", strlen("#include \"")+1);
                strncat(putme, included_files[i], strlen(included_files[i]));
                strncat(putme, "\" \n", strlen("\" \n"));

                for(j=0; j<strlen(putme); ++j)
                {
                    l[len-1] = putme[j];

                    len++;
                    l = realloc(l, len);
                }
            }

            // Header goşmalary gutarandygy bellenilýär.
            char *end_of_headers = "//.\n\n";
            for(i=0; i<strlen(end_of_headers); ++i)
            {

               l[len-1] = end_of_headers[i];

                len++;
                l = realloc(l, len);
            }

			cmode = 0;
		}
	}
	l[len-1] = '\0';
	fseek(s, 0, SEEK_SET);
	fwrite(l, sizeof(char), strlen(l), s);
	//printf("%s\n", l);

	free(l);
	free(ct);

    return 1;
}

