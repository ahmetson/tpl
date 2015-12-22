/*
Additonal functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "tokens.h"
#include "fns.h"


// Folder name = 255, command name = 45
#define MAX_SYS_CMD_LEN 300

/**2 sozi we bir harpy goshyar
 *
 * @to        - final string
 * @from      - first  adding string
 * @C         - second adding char
 * @from_len  - number of characters of first string
 * @to_len    - number of characters of final string
**/
int strstrchcat(char *to, char *from, char c)
{
    strncpy(to, from, strlen(from)+1);

	to[strlen(to)+1] = '\0';	        		// Sonky \0 bire suyshirilyar
	to[strlen(to)] = c;

	return 1;
}

/* Checks first occurance of substring in string started from offset
 *
 * @string      - search in
 * @sub         - search for
 * @offset      - start searching from this position
 *
 * Return 0  if strings are equal
 * Return -1 if substring is longer than string
 * Return -2 if string is not found
 * Return 1  when substring found in string
**/
int strstr_by_offset(const char *string, const char*sub, unsigned int offset)
{
	int i;
	int len     = strlen(string);
	int sub_len = strlen(sub);

	// Can not search long string in smaller one;
	if (len<sub_len)
		return -1;

	// The strings are equal
	if (strncmp(string, sub, len)==0)
		return 0;

	for(i=0; offset<sub_len; offset++, i++)
	{
		if (string[offset]!=sub[i])
			return -2;
	}

	return 1;
}


// Taze papka yasayar
int sys_mkdir(char *folder_name, char rm_prev_dir)
{
	if (rm_prev_dir)
		// Onki faylyn yzlary pozulyar
		sys_rmdir(folder_name);
	// Max file length + command name length
	char sys_cmd[MAX_SYS_CMD_LEN] = "mkdir >nul 2>nul ";		// >nul hides result, and
	strncat(sys_cmd, folder_name, strlen(folder_name));			// 2>nul hides errors
	return !system(sys_cmd);
}

int sys_rmdir(char *folder_name)
{
	char sys_cmd[MAX_SYS_CMD_LEN] = "rmdir /S /Q >nul 2>nul ";
	strncat(sys_cmd, folder_name, strlen(folder_name));
	return !system(sys_cmd);
}


// Berlen fayl adynyn onundaki papkalaryn atlaryny pozyar.
char *remove_dirnames(char *f)
{
	int i;
	for (i=0; i<strlen(f); ++i)
	{
		if (f[i]=='/')
		    f[i]='\\';
	}
	i = 0;

	if (strrchr(f, '\\'))
	{
	    char *t = strrchr(f, '\\');

	    if (strlen(t)>1)
		{
			int i;
		    for (i=1; i<strlen(t); ++i)
		    {
			    f[i-1] = t[i];
		    }
		    f[i-1] = '\0';
		}
	}

	return f;
}

// Berlen fayl adynyn gornushini pozyar.
char *remove_ext(char *f, char *e)
{
	//if (strlen(e)>=strlen(f))
	//	return f;

	if (strstr(f,e)!=NULL)
	{
		f[strlen(f)-strlen(e)] = '\0';
	}

	return f;
}

/*
 * Berlen sozun bolmaly uzynlygyndaky,
 * ichindaki hemme harplaryn deregine \0 bilen dolduryar.
 *
 * Eger-de bolmaly uzynlygy tanalmasy, yagny argument -1 bolsa,
 * ol strlen() funksiyasy arkaly tanalyar.
 *   Shonda dine sozde birinji \0 dushyancha hemme harplaryn deregine \0 goyular
**/
char *empty_string(char *f, int len)
{
	int i, last;
	if (len<0)
		last = strlen(f)-1;
	else
		last = len-1;

	for (i=0; i<=last; ++i)
	{
		f[i] = '\0';
	}
	return f;
}



void return_last_char(FILE *f)
{
    fpos_t pos;
    fgetpos(f, &pos);
    pos--;
    fsetpos(f, &pos);
}


void divide_string(char *source, char d, char ***out, int *items)
{
    int i;
    char *tmp = malloc(sizeof(char)*10);

    tmp[0] = '\0';
    for (i=0; i<strlen(source); ++i)
    {
        if (source[i]==d)
        {
            if (strlen(tmp))
            {
                *items += 1;
                *out = realloc(*out, sizeof(**out)*(*items));
                (*out)[*items-1] = NULL;
                (*out)[*items-1] = realloc((*out)[*items-1], sizeof(***out)*(strlen(tmp)+1));
                strncpy((*out)[*items-1], tmp, strlen(tmp)+1);

                free(tmp);

                tmp = malloc(sizeof(char)*10);
                tmp[0] = '\0';
            }
        }
        else
        {
            if (strlen(tmp))
            {
                int last = strlen(tmp)-1;
                tmp[last+1] = source[i];
                tmp[last+2]   = '\0';
            }
            else
            {
                tmp[0] = source[i];
                tmp[1] = '\0';
            }
        }
    }
    if (strlen(tmp))
    {
        *items += 1;
        //free((*out)[*items-2]);
        *out = realloc(*out, sizeof(**out)*(*items));
        (*out)[*items-1] = NULL;
        (*out)[*items-1] = realloc((*out)[*items-1], sizeof(***out)*(strlen(tmp)+1));
        strncpy((*out)[*items-1], tmp, strlen(tmp)+1);
    }
    free(tmp);
}


/** Harplyň daşynda duran goşa dyrnaklary aýyryp, yzyna gaýtarýar.

    @withKuotes - daşy goşa dyrnakly tekst,
    @unKutoed   - goşa dyrnaklar aýrylan görnüşi (Harplaryň sanawynyň uzynlygy @withKuotes sanawynyňky ýaly bolmaly)*/
void string_helper_remove_dquotes(char *unquoted, char *with_quotes)
{
    int u, q; // unquoted position, quoted position
    for(u=0, q=1;q<(strlen(with_quotes)-1); ++q, ++u)
    {
        unquoted[u] = with_quotes[q];
    }
    unquoted[u] = '\0';
}

