/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
**/
#include <stdio.h>
#include <string.h>

#include "pragma.h"
#include "../tpl.h"
#include "../error.h"

#define PRAGMA_MAIN_FILE "b1"
#define PRAGMA_MIN_LEN 2

// Pragmalaryn sany
const int PRAGMAS_NUM = 1;

const char PRAGMA_START_CHAR = '#';
const char PRAGMA_END_CHAR   = '\n';

char PRAGMA_MODE = 1;

char *pragmas[] = {
	PRAGMA_MAIN_FILE
};

act_pragma_item act_pragma_items[] = {
	{PRAGMA_MAIN_FILE, act_pragma_main_file}
};


// Pragmany ulanmana tayynlayar.
void init_pragma(pragma *prag)
{
	prag->is_compl = 0;
	strncpy(prag->name, "\0", strlen("\0")+1);
}

/*
 * Pragmany tanayan we jogabyny gaytaryan funksiya
 * Eger pragma tanalsa we gutaran bolsa, onda getirilen pragma gutaryldy diyilip goyulyar.
 * 
 * Gaytaryar:
 *  Pragmanyn saygarylanynyn statusy   
**/
int recognise_pragma(pragma *prag)
{
	int i, answer;
	
	// Pragma atlary minimum 2 s=harpdan ybarat bolmaly, bir harp we bir san
	if (strlen(prag->name)>=PRAGMA_MIN_LEN)
	{
		// Pragmanyn sony san bolup, galan yerinde san bolmaly dal
		int prag_name_last_char = strlen(prag->name)-1;
		
		for (i=0; i<prag_name_last_char; ++i)
		{
			if (isdigit(prag->name[i]))
				return 0;
		}
	}
	
	// Pragmamy, sanaw boyuncha barlanyar
	for (i=0; i<PRAGMAS_NUM; ++i )
	{
		answer = strstr_by_offset(pragmas[i], prag->name, 0);
		
		// Tapyldy we gutarnykly
		if (answer==0)
		{
			prag->is_compl = 1;
			return 1;
		}
		else if (answer>0)
		{
			prag->is_compl = 0;
			return 1;
		}
	}
	
	// Hich hili pragma komandalaryn gornushine degishli dal
	return 0;
}

// Pragmanyn komandasy boyuncha, pragmany yerine yetiryar
void act_pragma(pragma *prag)
{
	int i, len_to_cmp;
	for (i=0; i<PRAGMAS_NUM; ++i)
	{
		if (strncmp(act_pragma_items[i].name, prag->name, strlen(prag->name))==0)
		{
			act_pragma_items[i].act(prag);
			return;
		}
	}
	print_err(0, CODE0_PRAGMA_ACT_NOT_IDENTIFIED);
}


void act_pragma_main_file(pragma *prag)
{
	//printf("Fayl chagyryldy\n");
	if (strlen(main_file_name)<1)
	{
		strncpy(main_file_name, cur_parse_file_name, strlen(cur_parse_file_name)+1);
	}
	else
	{
		print_err(0, CODE0_MAIN_FILE_ALREADY_CHOSED);
	}
}
