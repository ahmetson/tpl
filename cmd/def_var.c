/**
 * Ülňini yglan edýän komanda.
**/
#include "../main/glob.h"
#include "../error.h"
#include "def_var.h"

int DEF_VAR_TYPE_NUM = 1;

/**
 * Ülňi yglan etmek komandany saýgarýar
**/
int is_cmd_def_var(command *cmd)
{
	if (cmd->items_num>DEF_VAR_MAX_TOKENS || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		return 0;
	}

	// Maksimum bolup biljek tokenlerden duryar, diymek global ulni yglan edilyar
	// Emma global ulni yglan edilmandir
	if (cmd->items_num==DEF_VAR_MAX_TOKENS && cmd->items[0].type==TOKEN_ITEM &&
		cmd->items[0].tok.type_class!=TOK_CLASS_GLOB)
	{
		return 0;
	}

	//printf("Birinji token barlanmaly\n");
	int next=-1;
	// Birinji token umumy ya maglumat tipi bolup bilyar
	if (cmd->items[0].type==TOKEN_ITEM &&
	cmd->items[0].tok.type_class==TOK_CLASS_GLOB)
	{
		def_var_cmd_mod(cmd, &cmd->items[0].tok, 0);
		//debug_cmd(cmd);
		next = TOK_CLASS_DEF_TYPE;
	}
	else if (cmd->items[0].type==1 &&
	cmd->items[0].tok.type_class==TOK_CLASS_DEF_TYPE)
	{
		def_var_cmd_mod(cmd, &cmd->items[0].tok, 0);
		next = TOK_CLASS_IDENT;
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
		if ( cmd->items[1].type==TOKEN_ITEM &&
		cmd->items[1].tok.type_class==next)
		{
			def_var_cmd_mod(cmd, &cmd->items[1].tok, 1);
			if (cmd->items[0].tok.type_class==TOK_CLASS_GLOB)
				next = TOK_CLASS_IDENT;
			else
				next = -1;
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
		if(next>0 && cmd->items[2].type==TOKEN_ITEM &&
		next==cmd->items[2].tok.type_class)
		{
			def_var_cmd_mod(cmd, &cmd->items[2].tok, 2);
		}
		else
		{
			return 0;
		}
	}
	//printf("Uchunji token barlanmady\n");
	return 1;
}



// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
int def_var_cmd_mod(command *cmd, token *tok, int tok_num)
{
	if (tok->type_class==TOK_CLASS_IDENT)
	{
		// Komandanyn in sonky tokeni bolmaly
		if (tok_num==cmd->items_num-1)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;

			cmd->is_compl = 1;

			// Global ulni yglan edilmandir
			// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
			if (cmd->items_num<DEF_VAR_MAX_TOKENS)
				cmd->ns = 1;

			return 1;
		}
		return 0;
	}
	else if (tok->type_class==TOK_CLASS_DEF_TYPE)
	{
		// Eger ulni global ulninin yglan edilshi bolsa, ikinji token,
		// yogsa komanda-da birinji token bolmaly
		if ((cmd->items[0].type==1 &&
			 cmd->items[0].tok.type_class==TOK_CLASS_GLOB &&
			 tok_num==1) ||
			 tok_num==0)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;
			cmd->value_class = tok->potentional_types[0].type_class;
			cmd->value_type  = tok->potentional_types[0].type_num;

			return 1;
		}
		return 0;
	}
	else if (tok->type_class==TOK_CLASS_GLOB)
	{
		// Komandanyn birinji tokeni bolmaly
		if (tok_num==0)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;

			cmd->ns = 0;

			return 1;
		}
		return 0;
	}
}



// Komandanyn global ulni yglan etmedigini barlayar
int is_glob_def_var_cmd(command *cmd)
{
	if (is_def_var_cmd(cmd) && cmd->ns==GLOB)
		return 1;
	return 0;
}

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_var_cmd(command *cmd)
{
	if (cmd->cmd_class==CMD_CLASS_DEF_VAR && cmd->cmd_type==DEF_VAR_TYPE_NUM)
		return 1;
	return 0;
}


/**
 * Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.
**/
int add_to_def_var_list(command *cmd)
{
    // Hazir TPL-in fayly komandalar bilen ishleyan boluminde


	//printf("Komanda showly saygaryldy\n");
	if (!is_def_var_cmd(cmd))
	{
	     // Ulni yglan etme komanda dal
        return 0;
	}

    // Komandany global funksiyalara goshjak bolyas
    if (!add_user_var_def_item(*cmd))
    {
        CUR_PART = 4;
        print_err(CODE4_VARS_IDENT_USED, (token *)inf_get_last_token(cmd));
    }

    return 1;
}
