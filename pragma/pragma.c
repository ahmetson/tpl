/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
**/
#include <stdio.h>
#include <string.h>

#include "../parser.h"
#include "pragma.h"
#include "../tpl.h"
#include "../error.h"

#define PRAGMA_MAIN_FILE "b1"
#define PRAGMA_MIN_LEN 2

// Pragmalaryn sany
int PRAGMAS_NUM = 1;

char PRAGMA_START_CHAR = '#';
char PRAGMA_END_CHAR   = '\n';

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


/**
 * Pragma bilen işleýän parseriň bölümi
**/
int work_with_pragma(pragma *prag, pragma *prev_prag, char *mode, char c, int c_pos, int line)
{
    if (c==PRAGMA_END_CHAR)
    {
        // Pragma tanalmady.
        if (strlen(prev_prag->name))  print_err(CODE2_PRAGMA_NOT_END, &inf_tok);

        *mode = PARSER_DEFAULT_MODE;
        return 2;
    }
    else if (isspace(c))
        return 2;
    else
    {
        //printf("Pragma moda gechildi. Indi parser pragmalary saygarmana chalshar.\n");
        strstrchcat(prag->name, prev_prag->name, c);

        if (!recognise_pragma(prag))
        {
            // Pragma bilen işlenilip durka ýalňyşlyk çyksa, nireden çykanyny bilmek üçin.
            inf_add_to_token(&inf_tok, c, c_pos, line);
            //printf("pragma tanalmady\n");
            print_err(CODE2_PRAGMA_NOT_IDENT, &inf_tok);
        }
        else
        {
            //printf("pragma tanaldy, pragma '%s', gutarylanmy: %d\n", prag->name, prag->is_compl);
            *prev_prag = *prag;
            if (prev_prag->is_compl)
            {
                //printf("Onki pragma: '%s', gutarylanmy: %d\n", prev_prag->name, prev_prag->is_compl);
                act_pragma(prev_prag);

                init_pragma(prag);
                init_pragma(prev_prag);
            }
        }
    }
    return 1;
}

/**
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
	int prev_part = CUR_PART;
	CUR_PART = 0;

	int i, len_to_cmp;
	for (i=0; i<PRAGMAS_NUM; ++i)
	{
		if (strncmp(act_pragma_items[i].name, prag->name, strlen(prag->name))==0)
		{
			act_pragma_items[i].act(prag);
			//printf("ss");
			CUR_PART = prev_part;
			return;
		}
	}
	print_err(CODE0_PRAGMA_ACT_NOT_IDENTIFIED, &inf_tok);
}


void act_pragma_main_file(pragma *prag)
{
	int prev_part = CUR_PART;
	CUR_PART = 0;
	//printf("Fayl chagyryldy\n");
	if (strlen(MAIN_FILE_NAME)<1)
	{
		strncpy(MAIN_FILE_NAME, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);
	}
	else
	{
		print_err(CODE0_MAIN_FILE_ALREADY_CHOSED, &inf_tok);
	}

	CUR_PART = prev_part;
}
