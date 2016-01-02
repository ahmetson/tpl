/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
**/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../parser.h"
#include "pragma.h"
#include "../tpl.h"
#include "../error.h"
#include "../main/inf.h"
#include "../token/harpl.h"
#include "../fns.h"

char PRAGMA_START_CHAR = '#';
char PRAGMA_END_CHAR   = '\n';

char PRAGMA_MODE = 1;


act_pragma_item act_pragma_items[] = {
	{PRAGMA_MAIN_FILE,             act_pragma_main_file},
	{PRAGMA_INCLUDE_GLOB_DEC_FILE, act_pragma_include_glob_decl_file}
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
void parse_pragma(FILE *s)
{
    // Parseriň Pragma modynda, pragmalary saýgarmak üçin
	pragma prev_prag; init_pragma(&prev_prag);
	pragma prag;	  init_pragma(&prag);

    while ((CUR_CHAR=fgetc(s))!=EOF)
    {
        update_inf();

        if (CUR_CHAR==PRAGMA_END_CHAR)
        {
            // Pragma tanalmady.
            if (strlen(prev_prag.name))  print_err(CODE2_PRAGMA_NOT_END, &inf_tok);
            return_last_char(s);
            return;
        }
        //else if (isspace(CUR_CHAR))
        //    continue;
        else
        {
            //printf("Pragma moda gechildi. Indi parser pragmalary saygarmana chalshar.\n");
            strstrchcat(prag.name, prev_prag.name, CUR_CHAR);

            if (!recognise_pragma(&prag))
            {
                // Pragma bilen işlenilip durka ýalňyşlyk çyksa, nireden çykanyny bilmek üçin.
                inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
                print_err(CODE2_PRAGMA_NOT_IDENT, &inf_tok);
            }
            else
            {
                //printf("pragma tanaldy, pragma '%s', gutarylanmy: %d\n", prag->name, prag->is_compl);
                prev_prag = prag;
                if (prev_prag.is_compl)
                {
                    //printf("Onki pragma: '%s', gutarylanmy: %d\n", prev_prag->name, prev_prag->is_compl);
                    act_pragma(&prev_prag);

                    init_pragma(&prag);
                    init_pragma(&prev_prag);
                }
            }
        }
    }
    // TODO : pragma gutardy emma tanalmady
    //print_err();
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

    ///# Pragma birinji: global yglan etmeleriň maglumatlary.
    /// 1)Eger pragma '@' ýa @" diýip başlaýan bolsa
    ///   a)Eger pragma =" däl we " harp bilen gutarýan bolsa
    ///      I)pragma gutarylmadyk edip bellenilýär.
    ///     ýogsa
    ///      II)pragma gutarylan edip bellenilýär.

    /// 2)Eger pragma 'b' diýip başlaýan bolsa
    ///   a)Eger pragma '1' diýip gutarýan bolsa
    ///      I)pragma gutarylan diýip bellenilýär.
    ///     ýogsa
    ///      II)pragma gutarylmadyk diýip bellenilýär.

    /// 1)
    if (strlen(prag->name) && prag->name[0]==PRAGMA_INCLUDE_GLOB_DEC)
    {
        if (strlen(prag->name)>=2 && prag->name[1]==PRAGMA_ADDR_QUOTE)
        {
            if (strlen(prag->name)>=3 && is_valid_string_const_data(prag->name, 2))
            {
                if  (is_string_const_data_compl(prag->name, 1))
                {
                    prag->is_compl = 1;
                }
                else
                {
                    prag->is_compl = 0;
                }
            }
            else
            {
                prag->is_compl = 0;
            }
        }
        else
            prag->is_compl = 0;
        return 1;
    }


    /// 2)
	// Pragma atlary minimum 2 s=harpdan ybarat bolmaly, bir harp we bir san
	if (strlen(prag->name) && prag->name[0]=='b')
	{
	    if (strlen(prag->name)==2 && prag->name[1]=='1')
        {
            prag->is_compl = 1;
        }
        else
            prag->is_compl = 0;
        return 1;
	}

	// Hich hili pragma komandalaryn gornushine degishli dal
	return 0;
}

// Pragmanyn komandasy boyuncha, pragmany yerine yetiryar
void act_pragma(pragma *prag)
{
	int prev_part = CUR_PART;
	CUR_PART = 0;

	int i;
	for (i=0; i<PRAGMAS_NUM; ++i)
	{
		if (strncmp(act_pragma_items[i].name, prag->name, strlen(act_pragma_items[i].name))==0)
		{
			act_pragma_items[i].act(prag);
			//printf("ss");
			CUR_PART = prev_part;
			return;
		}
	}
	inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
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
	    inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
		print_err(CODE0_MAIN_FILE_ALREADY_CHOSED, &inf_tok);
	}

	CUR_PART = prev_part;
}


void act_pragma_include_glob_decl_file(pragma *p)
{
    int prev_part = CUR_PART;
	CUR_PART = 0;
	/** Şert 1: Faýl parsing edilmeli.
        Eger komanda yglan edilmeli bolsa, onda
            komanda diňe lokal ülňi yglan etmesi bolmaly.
        Ýogsa
            ".Bash faýllarda diňe yglan edilip bolýan komandalar mümkin. Bärde bolsa näbelli komanda ulanylýar diýip aýdylýar.
	**/
	char fn[MAX_FILE_LEN] = {0};
	int i;
	for(i=2; i<strlen(p->name)-1; ++i)
    {
        fn[i-2] = p->name[i];
    }

    // Öň bu faýl parsing edilipdir
    if (is_glob_defs_file_exist(fn))
        return;

    glob_defs_file_add(fn);
    add_file_info(fn);

	FILE *source = fopen(fn, "r");

	if (source==NULL)
	{
	    inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
		print_err(CODE0_CANT_OPEN_FILE, &inf_tok);
	}

    // Adaty parser komandalary saýgarýar
	//printf("Parsinge başlandy\n");
	while((CUR_CHAR=fgetc(source))!=EOF)
	{
	    // Maglumatlar üçin
	    update_inf();

	    if(!is_valid_char())
            print_err(CODE2_UNKNOWN_TOKENS_CHAR, &inf_tok);

        // 2. Pragma modyna geçmeli
        if(CUR_CHAR==PRAGMA_START_CHAR)
        {
            parse_pragma(source);
        }
        else if (CUR_CHAR==PARENTHESIS_OPEN)
        {
            parenthesis par = parse_paren(source);
            cmd_add_item(&cmd, PAREN_ITEM, par, get_empty_cmd(), get_empty_tok());
        }
        else if (isspace(CUR_CHAR))
        {
            continue;
        }
        else if (CUR_CHAR==HARPL_OPENER )
        {
            token tok = parse_string(source);
            tok.inf_file_num = CUR_FILE_NUM-1;
            if (!work_with_token(&tok, &cmd))
            {
                // TODO
                // Yalnyshlyk peyda boldy, komanda tokeni goshup bolmady
            }
        }
        else if (CUR_CHAR==CMD_END)
        {
            //work_with_token(&tok, prev_tok_string);
            //work_with_cmd();
            if (!is_glob_decl_support_cmd(&cmd))
                print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(&cmd));
            else
            {
                glob_vars_decl_add(&cmd);
                glob_arrs_decl_add(&cmd);
                glob_fns_decl_add(&cmd);
            }
            init_cmd(&cmd, 0);
        }
        else
        {
            token tok = parse_token(source);
            tok.inf_file_num = CUR_FILE_NUM-1;
            if (!work_with_token(&tok, &cmd))
            {
                // TODO
                // Yalnyshlyk peyda boldy, komanda tokeni goshup bolmady
            }
            if (tok.type_class==TOK_CLASS_TRIANGLE_BLOCK && tok.potentional_types[0].type_num==TOKEN_TRIANGLE_BLOCK_OPEN_TYPE)
            {
                parse_triangle_block_inside(source, &cmd);
            }
        }

	}
	// Eger token bar bolsa, diymek komanda salynmandyr
	if (cmd.items_num)  print_err(CODE2_REMAIN_TOKEN, (token *)inf_get_last_token(&cmd));

    fclose(source);

	free_locals();
	CUR_PART = prev_part;
    CUR_LINE = 1;
    CUR_CHAR_POS = 1;

	CUR_PART = prev_part;
}
