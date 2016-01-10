/**
 * Baş ýasaljak kodly faýl bilen bagly ähli maglumatlar saklanýar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tpl.h"
#include "main_file.h"
#include "files.h"
#include "../translator_to_c/includes.h"

// Ýasaljak programmanyň baş faýlynyň ady
wchar_t MAIN_FILE_NAME[];
// Baş faýlda başga faýllaryň algoritmlerini saklaýan funksiýalaryň we faýllaryň sanawy
int  MAIN_FILE_INCLUDES_NUM;
wchar_t (*MAIN_FILE_INCLUDES)[2][MAX_INCLUDE_LEN];


/**
 * Baş ýasaljak kodyň soňuna, başga ýasaljak kodlaryň algoritmlerini çagyrjak funksiýalar goýulýar.
**/
int add_addtn_file_fns()
{
    FILE *s = _wfopen(get_c_source_by_source(MAIN_FILE_NAME), L"r+");
    int i, j;
	int len = 1,        // Harplaryň sany
        c_len = 1,      // Birinji teswirdäki harplaryň sany, funksiýalaryň yglan edilen faýllaryny goýmak üçin
        c2_len=1;       // Ikinji teswirdäki harplaryň sany, funksiýalary, algoritmiň soňuna goýmak üçin.

	wchar_t c,             // Goýmaly kodyň ýerleri üç harpdan ybarat
         prev_c = L'\0', // belgiler bilen bellenýär.
         prev_c2 = L'\0',
         cmode = 0;     // häzir ýasalan kodyň haýsy bölegindediginiň statusy

	wchar_t *l = malloc(len);
	wchar_t *ct = malloc(c_len);       // Birinji teswiriň zolagy: //.
	wchar_t *ct2 = malloc(c2_len);     // Ikinj teswir zolagy: //,

	while ((c=fgetwc(s))!=EOF)
	{
		// Ikinji teswir zolagyna düşüldi, diýmek baş blok gutarypdyr: //,
        if (prev_c==L'/' && prev_c2==L'/' && c==L',')
		{
			l[len-1] = L'\0';
			len--;
			l[len-1] = L'\0';
			len--;

			cmode = 3;      // Ikinji teswir zolagyna geçmeli
			continue;
		}
		// IKINJI TESWIR ZOLAGY
		else if (cmode==3)
		{
		    if (c==L'\n')
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
            wchar_t *end_of_headers = L"//, bash blok gutardy\n\t//  Programmanyn bashga bolekleri yerine yetirilmeli\n";
            for(i=0; i<wcslen(end_of_headers); ++i)
            {

               l[len-1] = end_of_headers[i];

                len++;
                l = realloc(l, len);
            }

            // Baş faýla inklud etmeli faýllaryň sanawy
            file_incs *fi = NULL;
            file_item *f = get_file_by_name(MAIN_FILE_NAME);
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

            // Prototipler baş faýlda çagyrylýar, olaryň yglan edilen faýly baş faýla inklud edilmeli faýllaryň
            // sanawyna goşulýar
		    wchar_t *tab = L"\t",
                    *space_nl  = L" \n",
                    *dquote = L"\"";
		    for(i=0; i<MAIN_FILE_INCLUDES_NUM; ++i)
            {
                //printf(L"Goshmaly:%s %d\n", included_files[i], files_num);
                wchar_t putme[MAX_FILE_LEN+20] = {0};
                wcsncpy(putme, tab, wcslen(tab)+1);
                wcsncat(putme, MAIN_FILE_INCLUDES[i][1], wcslen(MAIN_FILE_INCLUDES[i][1]));


                wcsncat(putme, space_nl, wcslen(space_nl));
                for(j=0; j<wcslen(putme); ++j)
                {
                    l[len-1] =
                    putme[j];

                    len++;
                    l = realloc(l, len);
                }

                // 3)
                wchar_t var_def_f[MAX_FILE_LEN] = {0};
                wcsncpy(var_def_f, dquote, wcslen(dquote)+1);
                wcsncat(var_def_f, MAIN_FILE_INCLUDES[i][0], wcslen(MAIN_FILE_INCLUDES[i][0]));
                wcsncat(var_def_f, dquote, wcslen(dquote));
                //printf(L"%s\n", var_def_f);

                includes_file_add_include(fi, var_def_f);
            }

			cmode = 0;

        }
	}
	l[len-1] = L'\0';
	fseek(s, 0, SEEK_SET);
	fwrite(l, sizeof(wchar_t), wcslen(l), s);
	//printf(L"%s\n", l);

	free(l);
	free(ct);
	free(ct2);

    fclose(s);

    return 1;
}
