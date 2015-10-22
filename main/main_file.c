/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tpl.h"
#include "main_file.h"

// Ýasaljak programmanyň baş faýlynyň ady
char MAIN_FILE_NAME[];
// Baş faýlda başga faýllaryň algoritmlerini saklaýan funksiýalaryň we faýllaryň sanawy
int  MAIN_FILE_INCLUDES_NUM;
char (*MAIN_FILE_INCLUDES)[2][265];


/**
 * Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.
**/
int add_addtn_file_fns()
{
    FILE *s = fopen(get_c_source_by_source(MAIN_FILE_NAME), "r+");

    int i, j;
	int len = 1,        // Harplaryň sany
        c_len = 1,      // Birinji teswirdäki harplaryň sany, funksiýalaryň yglan edilen faýllaryny goýmak üçin
        c2_len=1;       // Ikinji teswirdäki harplaryň sany, funksiýalary, algoritmiň soňuna goýmak üçin.

	char c,             // Goýmaly kodyň ýerleri üç harpdan ybarat
         prev_c = '\0', // belgiler bilen bellenýär.
         prev_c2 = '\0',
         cmode = 0;     // häzir ýasalan kodyň haýsy bölegindediginiň statusy

	char *l = malloc(len);
	char *ct = malloc(c_len);       // Birinji teswiriň zolagy: //.
	char *ct2 = malloc(c2_len);     // Ikinj teswir zolagy: //,

	while ((c=fgetc(s))!=EOF)
	{
	    // Birinji teswir zolagyna düşüldi, diýmek inkludler gutarypdyr: //.
		if (prev_c=='/' && prev_c2=='/' && c=='.')
		{
			l[len-1] = '\0';
			len--;
			l[len-1] = '\0';
			len--;

			cmode = 1;      // Birinji teswir zolagyna geçmeli
			continue;
		}
		// Ikinji teswir zolagyna düşüldi, diýmek baş blok gutarypdyr: //,
        if (prev_c=='/' && prev_c2=='/' && c==',')
		{
			l[len-1] = '\0';
			len--;
			l[len-1] = '\0';
			len--;

			cmode = 3;      // Ikinji teswir zolagyna geçmeli
			continue;
		}

        // BIRINJI TESWIR ZOLAGY
		if (cmode==1)
		{
		    if (c=='\n')
			{
				cmode=2;    // Funksiýalaryň yglan edilen faýllaryny häzirki ýasalan koda inklud edýän bölüme geçilýär.
				continue;
			}
			ct[c_len-1] = c;

			c_len++;
			ct = realloc(ct, c_len);
		}
		// IKINJI TESWIR ZOLAGY
		else if (cmode==3)
		{
		    if (c=='\n')
			{
				cmode=4;    // Funksiýalary häzirki ýasalan koda goşýan bölüme geçilýär.
				continue;
			}
			ct[c_len-1] = c;

			c_len++;
			ct = realloc(ct, c_len);
		}
		else if(!cmode) // cmode = 0
		{
			l[len-1] = c;
			prev_c2 = prev_c;
			prev_c = c;

			len++;
			l = realloc(l, len);
		}
		// Funksiýalary häzirki ýasalan koda goşýan bölüme geçirýän bölüm
		else if(cmode==4)
        {
            // Header goşmalary gutarandygy bellenilýär.
            char *end_of_headers = "//, bash blok gutardy\n\t//  Programmanyn bashga bolekleri yerine yetirilmeli\n";
            for(i=0; i<strlen(end_of_headers); ++i)
            {

               l[len-1] = end_of_headers[i];

                len++;
                l = realloc(l, len);
            }

            // Prototipler çagyrylýar
		    for(i=0; i<MAIN_FILE_INCLUDES_NUM-1; ++i)
            {
                //printf("Goshmaly:%s %d\n", included_files[i], files_num);
                char putme[270];
                strncpy(putme, "\t", strlen("\t")+1);
                strncat(putme, MAIN_FILE_INCLUDES[i][1], strlen(MAIN_FILE_INCLUDES[i][1]));
                strncat(putme, " \n", strlen(" \n"));

                for(j=0; j<strlen(putme); ++j)
                {
                    l[len-1] = putme[j];

                    len++;
                    l = realloc(l, len);
                }
            }

			cmode = 0;

        }
        // Funksiýalaryň yglan edilen faýllaryny häzirki ýasalan koda inklud edýän bölüme geçilýär.
        else    // çmode - 2
		{
            // Fayllar goshulyar
		    for(i=0; i<MAIN_FILE_INCLUDES_NUM-1; ++i)
            {
                //printf("Goshmaly:%s %d\n", included_files[i], files_num);
                char putme[270];
                strncpy(putme, "#include \"", strlen("#include \"")+1);
                strncat(putme, MAIN_FILE_INCLUDES[i][0], strlen(MAIN_FILE_INCLUDES[i][0]));
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
	free(ct2);

    fclose(s);

    return 1;
}
