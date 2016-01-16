/*
 * Kompilyatoryn (Has takygy programma dilinin ozi bilen bagly komandalara - pragma diyilyar.)
 * Pragmalar bilen ishleyan we gerekli maglumatlar
**/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../cmds.h"
#include "../parser.h"
#include "pragma.h"
#include "../tpl.h"
#include "../error.h"
#include "../main/inf.h"
#include "../main/user_def_type.h"
#include "../token/harpl.h"
#include "../fns.h"

wchar_t PRAGMA_START_CHAR = L'#';
wchar_t PRAGMA_END_CHAR   = L'\n';

wchar_t PRAGMA_MODE = 1;


act_pragma_item act_pragma_items[] = {
	{PRAGMA_MAIN_FILE,             act_pragma_main_file},
	{PRAGMA_INCLUDE_GLOB_DEC_FILE, act_pragma_include_glob_decl_file}
};


int parser_mode_pragma( FILE *source )
{
    if( CUR_CHAR!=PRAGMA_START_CHAR )
        return 0;

    parse_pragma( source );
    return 1;
}


// Pragmany ulanmana tayynlayar.
void init_pragma(pragma *prag)
{
    wchar_t *endstr = L"\0";
	prag->is_compl = 0;
	wcsncpy(prag->name, endstr, wcslen(endstr)+1);
}


/** Pragma bilen işleýän parseriň bölümi
**/
void parse_pragma(FILE *s)
{
    // Parseriň Pragma modynda, pragmalary saýgarmak üçin
	pragma prev_prag; init_pragma(&prev_prag);
	pragma prag;	  init_pragma(&prag);

    while (1)
    {
        if( TMP_CHAR )
        {
            CUR_CHAR = TMP_CHAR;
            TMP_CHAR = 0;
        }
        else
        {
            if( ( CUR_CHAR=fgetwc( s ) )==WEOF )
                break;
        }

        update_inf();

        if (CUR_CHAR==PRAGMA_END_CHAR)
        {
            // Pragma tanalmady.
            if (wcslen(prev_prag.name))  print_err(CODE2_PRAGMA_NOT_END, &inf_tok);
            return_last_wchar_t();
            return;
        }
        //else if (iswspace(CUR_CHAR))
        //    continue;
        else
        {
            wcsstrchcat( prag.name, prev_prag.name, CUR_CHAR );

            if ( !recognise_pragma( &prag ) )
            {
                // Pragma bilen işlenilip durka ýalňyşlyk çyksa, nireden çykanyny bilmek üçin.
                inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
                print_err(CODE2_PRAGMA_NOT_IDENT, &inf_tok);
            }
            else
            {
                //printf(L"pragma tanaldy, pragma '%s', gutarylanmy: %d\n", prag->name, prag->is_compl);
                prev_prag = prag;
                if (prev_prag.is_compl)
                {
                    //printf(L"Onki pragma: '%s', gutarylanmy: %d\n", prev_prag->name, prev_prag->is_compl);
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
    if ( wcslen(prag->name) && prag->name[0]==PRAGMA_INCLUDE_GLOB_DEC )
    {
        if (wcslen(prag->name)>=2 && prag->name[1]==PRAGMA_ADDR_QUOTE)
        {
            if (wcslen(prag->name)>=3 && is_valid_string_const_data(prag->name, 2))
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
	if ( wcslen( prag->name ) && prag->name[0]==L'b' )
	{
	    if (wcslen(prag->name)==2 && prag->name[1]==L'1')
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
		if (wcsncmp(act_pragma_items[i].name, prag->name, wcslen(act_pragma_items[i].name))==0)
		{
			act_pragma_items[i].act(prag);
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

	if (wcslen(MAIN_FILE_NAME)<1)
	{
		wcsncpy(MAIN_FILE_NAME, CUR_FILE_NAME, wcslen(CUR_FILE_NAME)+1);
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
	wchar_t fn[MAX_FILE_LEN] = {0};
	int i;
	for(i=2; i<wcslen(p->name)-1; ++i)
    {
        fn[i-2] = p->name[i];
    }

    // Öň bu faýl parsing edilipdir
    if (is_glob_defs_file_exist(fn))
        return;

    glob_defs_file_add(fn);
    add_file_info(fn);

	FILE *source = _wfopen(fn, L"r, ccs=UTF-8");

	if (source==NULL)
	{
	    inf_add_to_token(&inf_tok, CUR_CHAR, CUR_CHAR_POS, CUR_LINE);
		print_err(CODE0_CANT_OPEN_FILE, &inf_tok);
	}

    // Adaty parser komandalary saýgarýar
	while(1)
	{
	    if ( !process_char( source ) )
            break;

	    if ( !iswspace( CUR_CHAR )               &&
             !parser_mode_pragma( source )       &&
             !parser_mode_paren( source, &cmd )  &&
             !parser_mode_string( source, &cmd ) &&
             !parser_mode_end_cmd( source ) )
        {
            if (CUR_CHAR==CMD_END)
            {
                if ( !is_glob_decl_support_cmd( &cmd ) )
                    print_err(CODE0_UNSUPPORT_INCLUDE_FILE_CMD, (token *)inf_get_last_token(&cmd));
                else
                {
                    glob_vars_decl_add(&cmd);
                    glob_arrs_decl_add(&cmd);
                    glob_fns_decl_add(&cmd);
                }
                init_cmd( &cmd, 0);
            }
            else
            {
                token tok = parse_token(source);
                tok.inf_file_num = CUR_FILE_NUM-1;
                work_with_token( &tok, &cmd );
                if (tok.type_class==TOK_CLASS_TRIANGLE_BLOCK && tok.potentional_types[0].type_num==TOKEN_TRIANGLE_BLOCK_OPEN_TYPE)
                {
                    parse_triangle_block_inside(source);
                }
            }
        }

	}
	// Eger token bar bolsa, diymek komanda salynmandyr
	if ( cmd.items_num )
        print_err(CODE2_REMAIN_TOKEN, (token *)inf_get_last_token(&cmd));

    fclose(source);

	free_locals();
    CUR_LINE = 1;
    CUR_CHAR_POS = 1;

	CUR_PART = prev_part;
}
