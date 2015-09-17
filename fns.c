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

/* Concationation of string and character
 *  
 * @to        - final string
 * @from      - first  adding string
 * @c         - second adding char
 * @from_len  - number of characters of first string
 * @to_len    - number of characters of final string
**/
void strcat_ch(token_word *to, token_word *from, char c)
{
	strncpy(to->val, from->val, from->len);
	to->len = from->len;
		
	to->val[to->len-1] = c;	        		// Delete last \0
	to->len++;
	to->val[to->len-1] = '\0';			// Add to end \0
}
// Soze soz bilen harpy goshyar
void strcat_ch2(char *to, char *from, char c)
{
	strncpy(to, from, strlen(from)+1);	// +1 sebabi gutaryan \0 goshmaly
		
	// Harp goshulyar.
	int len = strlen(to);
	to[len] = c;
	to[len+1] = '\0';
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
