/**
 * Ülňini yglan edýän komanda.
**/
#include <stdlib.h>
#include <string.h>
#include "../main/glob.h"
#include "../main/inf.h"
#include "../translator_to_c.h"
#include "../translator_to_c/includes.h"
#include "../error.h"
#include "../cmds.h"
#include "../fns/fn_helpers.h"
#include "../fns.h"
#include "array.h"
#include "def_var.h"

int DEF_VAR_TYPE_NUM = 0;

/**
 * Ülňi yglan etmek komandany saýgarýar
**/
int is_cmd_def_var(command *cmd)
{
	if (cmd->items_num>CMD_MAX_ITEMS[CMD_CLASS_DEF_VAR][0] || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		return 0;
	}

	// Maksimum bolup biljek tokenlerden duryar, diymek global ulni yglan edilyar
	// Emma global ulni yglan edilmandir
	command_item *fci = get_cmd_item(cmd->items,0);
	if (cmd->items_num==CMD_MAX_ITEMS[CMD_CLASS_DEF_VAR][0] && fci->type==TOKEN_ITEM && fci->tok.type_class!=TOK_CLASS_GLOB)
	{
		return 0;
	}

	int is_glob = 0;
	// Birinji token umumy ya maglumat tipi bolup bilyar
	if (fci->type==TOKEN_ITEM && fci->tok.type_class==TOK_CLASS_GLOB)
	{
	    def_var_cmd_mod(cmd, &fci->tok, 0);
		is_glob = 1;
	}
	else if (fci->type==1 && (fci->tok.type_class==TOK_CLASS_DEF_TYPE ||
                             fci->tok.type_class==TOK_CLASS_UTYPE_CON))
	{
	    def_var_cmd_mod(cmd, &fci->tok, 0);
	}
    else
	{
		return 0;
	}

	// Eger birinji umumylygy anladyan token bolsa onda ikinji token maglumat
	// 	tipi bolup bilyar
	// Yada identifikator bolup bilyar
	if (cmd->items_num>1)
	{
        command_item *sci = get_cmd_item(cmd->items,1);
		if (is_glob)
		{
		    if (sci->type==TOKEN_ITEM && (sci->tok.type_class==TOK_CLASS_DEF_TYPE ||
                                         sci->tok.type_class==TOK_CLASS_UTYPE_CON))
            {
                def_var_cmd_mod(cmd, &sci->tok, 1);
            }
            else
            {
                return 0;
            }

		}
		else if (sci->type==TOKEN_ITEM && sci->tok.type_class==TOK_CLASS_IDENT)
        {
            def_var_cmd_mod(cmd, &sci->tok, 1);
        }
        else
		{
			return 0;
		}
	}

	// Eger birinji umumylygy anladyan token bolmasa,
	//	identifikator bolup bilyar.
	if (cmd->items_num>2)
	{
	    command_item *tci = get_cmd_item(cmd->items,2);
		if(is_glob && tci->type==TOKEN_ITEM && tci->tok.type_class==TOK_CLASS_IDENT)
		{
			def_var_cmd_mod(cmd, &tci->tok, 2);
		}
		else
		{
			return 0;
		}
	}
	return 1;
}



// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
void def_var_cmd_mod(command *cmd, token *tok, int tok_num)
{
	if (tok->type_class==TOK_CLASS_IDENT)
	{
		// Komandanyn in sonky tokeni bolmaly
		if (tok_num==cmd->items_num-1)
		{
			cmd->cmd_class = CMD_CLASS_DEF_VAR;
			cmd->cmd_type = DEF_VAR_TYPE_NUM;

			cmd->is_compl = 1;

			// Global ulni yglan edilmandir
			// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
			if (cmd->items_num<CMD_MAX_ITEMS[cmd->cmd_class][cmd->cmd_type])
				cmd->ns = 1;
		}
	}
	else if (tok->type_class==TOK_CLASS_DEF_TYPE)
	{
		// Eger ulni global ulninin yglan edilshi bolsa, ikinji token,
		// yogsa komanda-da birinji token bolmaly
		command_item *fci = get_cmd_item(cmd->items, 0);
		if ( (fci->type==1 && fci->tok.type_class==TOK_CLASS_GLOB && tok_num==1) || tok_num==0)
		{
			cmd->cmd_class = CMD_CLASS_DEF_VAR;
			cmd->cmd_type = DEF_VAR_TYPE_NUM;
			cmd->value_class = tok->potentional_types[0].type_class;
			cmd->value_type  = tok->potentional_types[0].type_num;
		}
	}
	else if (tok->type_class==TOK_CLASS_GLOB)
	{
		// Komandanyn birinji tokeni bolmaly
		if (tok_num==0)
		{
			cmd->cmd_class = CMD_CLASS_DEF_VAR;
			cmd->cmd_type = DEF_VAR_TYPE_NUM;

			cmd->ns = 0;
		}
	}
}



// Komandanyn global ulni yglan etmedigini barlayar
int is_glob_def_var_cmd(command *cmd)
{
	if (cmd->is_compl && is_def_var_cmd(cmd) && cmd->ns==GLOB)
		return 1;
	return 0;
}

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_var_cmd(command *cmd)
{
	if (cmd->cmd_class==CMD_CLASS_DEF_VAR && cmd->cmd_type==DEF_VAR_TYPE_NUM && cmd->is_compl)
		return 1;
	return 0;
}

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_arr_cmd(command *cmd)
{
	if (cmd->cmd_class==CMD_CLASS_ARR && cmd->cmd_type==CMD_CLASS_ARR_DEF && cmd->is_compl)
		return 1;
	return 0;
}


/** Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.
**/
int add_to_def_var_list(command *cmd)
{
	if (!is_def_var_cmd(cmd))
	{
	    int i;
	    for ( i=0; i<cmd->items_num; ++i )
        {
            command_item *ci = get_cmd_item( cmd->items, i );
            if ( ci->type!=CMD_ITEM )
                continue;
            if ( !is_def_var_cmd( &ci->cmd ) )
                add_to_def_var_list( &ci->cmd );
            else
            {
                if ( ci->cmd.ns==GLOB)
                    var_def_add( &ci->cmd, 1);
                else
                    var_def_add( &ci->cmd, 0);
            }
        }
        // Ulni yglan etme komanda dal
        return 0;
	}
    if (cmd->ns==GLOB)
        var_def_add(cmd, 1);
    else
        var_def_add(cmd, 0);
    return 1;
}

/** Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.
**/
int add_to_def_arr_list(command *cmd)
{
	if (!is_def_arr_cmd(cmd))
	{
	    int i;
	    for ( i=0; i<cmd->items_num; ++i )
        {
            command_item *ci = get_cmd_item( cmd->items, i );
            if ( ci->type!=CMD_ITEM )
                continue;
            if ( !is_def_arr_cmd( &ci->cmd ) )
                add_to_def_arr_list( &ci->cmd );
            else
            {
                if ( ci->cmd.ns==GLOB )
                    arr_def_add( &ci->cmd, 1);
                else
                    arr_def_add( &ci->cmd, 0);
            }
        }
        // Ulni yglan etme komanda dal
        return 0;
	}
    if (cmd->ns==GLOB)
        arr_def_add(cmd, 1);
    else
        arr_def_add(cmd, 0);
    return 1;
}

/** Ýasalan kodda başga faýllarda yglan edilen global ülňileriň çagyrylanlarynyň sanawyna täze ülňini goşýar
    Bu sanaw C kody üçin bölümde gerek. Sebäbi ýasalan kodda çagyrylan ülňiniň yglan edilen ýeriniň .h hem goşulmaly.
*/
void global_called_vars_add(command *cmd)
{
    if (!cmd->is_compl || cmd->cmd_class!=CMD_CLASS_CALL_GLOB_VAR)
        return;
    command_item *lci = get_cmd_item(cmd->items,cmd->items_num-1); // lci - last command item
    int  *fnum = &lci->tok.inf_file_num;
    wchar_t *ident= lci->tok.potentional_types[0].value;

    /// Eger ülňi yglan edilen faýlynda çagyrylýan bolsa, onda ülňiniň yglan edilen .h faýly eýýäm inklud edildi.
    int i, len;
    for(i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
    {
        if ((wcslen(GLOB_VAR_DEFS[i].name)==wcslen(ident) &&
            wcsncmp(GLOB_VAR_DEFS[i].name, ident, wcslen(ident))==0) &&
            *fnum==GLOB_VAR_DEFS[i].inf_file_num)
            return;
    }

    if (*fnum+1<=GLOB_CALLED_VARS_NUM)
    {
        called_var *cv = &GLOB_CALLED_VARS[*fnum];

        /// Eger eýýäm şeýle ülňi öňem çagyrylan bolsa, onda ikinji gezek goşmak nämä gerek?
        for(i=0; i<cv->num; ++i)
        {
            if (wcslen(cv->ident[i])==wcslen(ident) && wcsncmp(cv->ident[i], ident, wcslen(ident)==0))
                return;
        }

        cv->num++;
        cv->ident = realloc( cv->ident, sizeof( *cv->ident )*cv->num );

        wcsncpy(cv->ident[cv->num-1], ident, wcslen( ident )+1 );
    }
    else
    {
        /// Eger çagyrylan ülňiniň faýlynda intäk hiç hili ülňi çagyrylmadyk bolsa
        /// Ýasaljak kody çagyrylan ülňileri bolan faýllaryň sanawyna goşulýar
        ++GLOB_CALLED_VARS_NUM;

        GLOB_CALLED_VARS = realloc(GLOB_CALLED_VARS, sizeof(*GLOB_CALLED_VARS)*GLOB_CALLED_VARS_NUM);
        called_var newf;
        newf.ident = NULL;
        newf.num   = 0;
        GLOB_CALLED_VARS[GLOB_CALLED_VARS_NUM-1] = newf;
        GLOB_CALLED_VARS[GLOB_CALLED_VARS_NUM-1].ident = malloc(sizeof(*GLOB_CALLED_VARS[GLOB_CALLED_VARS_NUM-1].ident));
        GLOB_CALLED_VARS[GLOB_CALLED_VARS_NUM-1].num   = 1;

        wcsncpy(GLOB_CALLED_VARS[GLOB_CALLED_VARS_NUM-1].ident[0], ident, wcslen(ident)+1);
    }
}



/** Komandanyň gaýtarýan maglumatynyň tipini berýän funksiýa:
    Iň soňky tokeniň maglumaty
**/
int cmd_def_var_return_type(command *cmd, int *return_class, int *return_type)
{
    int ident_prev = cmd->items_num-2;
    command_item *ipci = get_cmd_item(cmd->items,ident_prev); // ipci - iden previoud command item
    *return_class = ipci->tok.potentional_types[0].type_class;
    *return_type  = ipci->tok.potentional_types[0].type_num;
    set_def_type_alias_const_data(return_class, return_type);
    return 1;
}


/// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
void work_with_called_glob_vars()
{
    int i, j;
    file_incs *fi = NULL;
    for (i=0; i<GLOB_CALLED_VARS_NUM; ++i)
    {
        if (i+1>INCLUDES_NUM)
        {
            fi = includes_add_new();
        }
        else
        {
            fi = &INCLUDES[i];
        }

        for(j=0; j<GLOB_CALLED_VARS[i].num; ++j)
        {
            wchar_t *dquote = L"\"", *dot_h = L".h";
            glob_ident *gi = glob_vars_def_get_by_name(GLOB_CALLED_VARS[i].ident[j]);
            wchar_t var_def_f[MAX_FILE_LEN] = {0};
            wcsncpy(var_def_f, dquote, wcslen(dquote)+1);
            wcsncat(var_def_f, FILES[gi->inf_file_num].name, wcslen(FILES[gi->inf_file_num].name));
            wcsncat(var_def_f, dot_h, wcslen(dot_h));
            wcsncat(var_def_f, dquote, wcslen(dquote));

            includes_file_add_include(fi, var_def_f);
        }
    }
}


/** Eger ülňi yglan edilen wagty programçy tarapyndan maglumat bilen baglanylsa,
    Bu ýagdaý iki tapgyra bölünýär.
    Birinji ülňiniň özi yglan edilýär we TPL tarapyndan başlangyç maglumat içine salynýar.
    Ikinjiden bolsa, ülňi çagyrylyp, programçynyň maglumaty bilen baglanylýar.

    Bu funksiýa ikinji tapgyry edýär. Sebäbi Ülňiniň yglan edilmegi algoritmi ýazýan funksiýadan başga funksiýa edýär.
**/
void cmd_def_var_as_subcmd_c_code(command *cmd, wchar_t **l, int *llen)
{
    wchar_t *space = L" ";
    /** Yglan etme başga komandalaryň birligi boljak bolsa, çagyrylmaly. Şonuň üçin identifikatory ýazylýar */
    /** Eger-de funksiýa yglan etmegiň argumenti hökümnde bolsa, onda tipi hem çap edilmeli*/
    if (TEST==100)
    {
        command_item *lci = get_cmd_item(cmd->items,cmd->items_num-2);
        token *t = &lci->tok;

        get_type_c_code(t->potentional_types[0].type_class, t->potentional_types[0].type_num, l, llen);

        wcsadd_on_heap( l, llen, space );
    }
    command_item *lci = get_cmd_item(cmd->items,cmd->items_num-1);
    token *t = &lci->tok;
    tok_get_c_code(t, l, llen);
}


int semantic_cmd_def_var(command *cmd)
{
    if (is_inside_fn())
    {
        command_item *ident_item = get_cmd_item(cmd->items, 1);
        wchar_t *ident = ident_item->tok.potentional_types[0].value;
        if (is_ident_used(&ident_item->tok, 3) || is_tmp_fn_ident_used(ident) )
        {
            CUR_PART = 4;
            print_err(CODE4_VARS_IDENT_USED, (token *)inf_get_last_token(cmd));

        }
    }
    else if (cmd->ns==GLOB && GLOB_BLOCK_INCLUDES)
    {
        CUR_PART = 7;
        print_err(CODE7_GLOB_DEF_IN_BLOCK, (token *)inf_get_last_token(cmd));
    }
    return 1;
}


