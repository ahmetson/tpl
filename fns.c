/*
Additonal functions
*/
#include <stdio.h>
#include <string.h>

#include "tpl.h"
#include "tokens.h"
#include "fns.h"


// Folder name = 255, command name = 45
#define MAX_SYS_CMD_LEN 300

/* 2 sozi we bir harpy goshyar
 *
 * @to        - final string
 * @from      - first  adding string
 * @c         - second adding char
 * @from_len  - number of characters of first string
 * @to_len    - number of characters of final string
**/
void strstrchcat(char *to, char *from, char c)
{
    strncpy(to, from, strlen(from)+1);

	to[strlen(to)+1] = '\0';	        		// Sonky \0 bire suyshirilyar
	to[strlen(to)] = c;
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
