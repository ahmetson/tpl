/*
All Command information
*/
#include <stdio.h>
#include <string.h>

#include "tpl.h"
#include "cmd.h"
#include "dev_debug.h"
#include "error.h"

// Command can contain maximum 3 tokens
const int CMD_MAX_TOKENS = 3;
const int DEF_VAR_MAX_TOKENS = 3;

// Komandalaryn sanawy
int CMDS_TYPES_NUM = 1;
#define DEF_VAR_CLASS_NUM 1
#define DEF_VAR_TYPE_NUM 1

#define GLOB 0
#define LOCAL 1

is_cmd_item cmd_types[] = {
	   {is_cmd_def_var}
};

int cmd_classes_numbers[] = {1};
// Dine debug uchin ulanylyar. Komanda tiplerinin atlary
char *cmd_classes[] = {
	"var"
};

#define CONST_MAX_CLASS_TYPES 1
char *cmd_class_types[][CONST_MAX_CLASS_TYPES] = {
	{"def"}						// Var class
};

// Komandanyn bashynda bolup bilyan tokenlerin sany
int CMD_FIRST_TOKENS_NUM = 2;

// Bashynda bolup bilyan tokenlerin klasy
int cmd_first_tokens_classes[] = {
	1,
	2
};


void init_cmd(command *cmd)
{
	cmd->tokens_num = 0;
	cmd->is_compl = 0;
	cmd->max_tokens_num = 0;
	cmd->cmd_type = 0;
	cmd->cmd_class = 0;
	cmd->ns = -1;
}


int recognize_cmd(command *cmd)
{
	int prev_part = CUR_PART;
	CUR_PART = 4;
	int i, first_tok = 0, tok_num=0;				// komanda-da tokenin nomeri
	command prev_cmd; init_cmd(&prev_cmd);
	prev_cmd = *cmd; init_cmd(cmd);
	
	// Birinji token hokman komandanyn bashy bolmaly
	for (i=0; i<CMD_FIRST_TOKENS_NUM; i++)
	{
		if (cmd_first_tokens_classes[i]==prev_cmd.tokens[tok_num].type_class)
		{
			//debug_token(&cmd->tokens[tok_num]);
			first_tok=1;
			break;
		}
	}
	if (!first_tok)
	{
		print_err(CODE4_CMD_HASNT_FIRST_TOKEN);
	}
	//printf("birinji tokeni eken\n");
	cmd_add_token(cmd, prev_cmd.tokens[tok_num]);
		
	//printf("birinji token komanda goshuldy\n");
	// Komandanyn ikinji tokeninden, ta gutaryancha
	for (tok_num=1; tok_num<prev_cmd.tokens_num; tok_num++)
	{
		cmd_add_token(cmd, prev_cmd.tokens[tok_num]);

		if (!parse_cmd(cmd))
		{
			//printf("Komanda saygarylmady\n");
			//debug_cmd(cmd);
			//init_token(cmd->tokens_num--;
		}
		else
		{
			//printf("Komanda saygaryldy\n");
		}
		// Eger komanda tanalmadyk bolsa
		//   sonky token pozulyar
		//   taze komanda yasalyar
		//   onki komanda taze komandanyn birinji komandasy diyilip yazylyar
		//   hazirki token komandanyn itemi diyilip yazylyar
		//   Eger komanda saygarylmasa
		//     komanda saygarylmady diyilip yalnyshlyk gaytarylyar.

		//printf("hawa");
		
	}
	
	// Komandany saygaryp bolmady ya ol gutarylmadyk
	if (cmd->cmd_class<1 && cmd->cmd_type<1 || !cmd->is_compl)
	{
		//debug_cmd(cmd);
		return 0;
   }
	CUR_PART = prev_part;
	// Komandany saygaryp boldy
	return 1;
}


/*
 * Tokeni komandanyn ichine goshyar
**/
int cmd_add_token(command *to, token tok)
{
	to->tokens[to->tokens_num++] = tok;
	//to->ns = tok.ns;
	
	return 0;
}

/*
 * Tokeni saygarylyar
**/
int parse_cmd(command *cmd)
{
	int i;
	for (i=0; i<CMDS_TYPES_NUM; i++)
	{		
		cmd_types[i].is_cmd(cmd);
	}
	
	// Komanda saygarylmandyr
	if (cmd->cmd_class==0 && cmd->cmd_type==0)
	{
		//printf("Debug! Komanda tanalmady\n");
		return 0;												// Token tanalmady
	}
	//printf("Debug! Komanda tanaldy\n");
	return 1;
}


// ==================================
// Komandalary saygaryan funksiyalar

// 1. Ulni yglan etmani saygaryan komanda
int is_cmd_def_var(command *cmd)
{
	if (cmd->tokens_num>DEF_VAR_MAX_TOKENS)
	{
		return 0;
	}
	
	// Maksimum bolup biljek tokenlerden duryar, diymek global ulni yglan edilyar
	// Emma global ulni yglan edilmandir
	if (cmd->tokens_num==DEF_VAR_MAX_TOKENS && cmd->tokens[0].type_class!=GLOB_TYPE_CLASS)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class = 0;
		cmd->cmd_type = 0;
		return 0;
	}

	int next=-1;	
	// Birinji token umumy ya maglumat tipi bolup bilyar
	if (cmd->tokens[0].type_class==GLOB_TYPE_CLASS)
	{
		if (cmd->tokens_num==3)

		def_var_cmd_mod(cmd, &cmd->tokens[0], 0);

		next = DEF_TYPE_TYPE_CLASS;
	}
	else if (cmd->tokens[0].type_class==DEF_TYPE_TYPE_CLASS)
	{
		def_var_cmd_mod(cmd, &cmd->tokens[0], 0);
		next = IDENT_TYPE_CLASS;
	}
	else
	{
		//printf("Hich zat tanalmady\n");
	}
	
	// Eger birinji umumylygy anladyan token bolsa onda ikinji token maglumat
	// 	tipi bolup bilyar
	// Yada identifikator bolup bilyar
	if (cmd->tokens[1].type_class==next)
	{
		def_var_cmd_mod(cmd, &cmd->tokens[1], 1);
		if (cmd->tokens[0].type_class==GLOB_TYPE_CLASS)
			next = IDENT_TYPE_CLASS;
		else
			next = -1;
	}
	else
	{
		//printf("hich zat ikinji tapgyrda-da tanalmady\n");
	}
	
	// Eger birinji umumylygy anladyan token bolmasa,
	//	identifikator bolup bilyar.
if (cmd->tokens_num==3)
	if (next>0)
	{ 
		if(next==cmd->tokens[2].type_class)
		{
			def_var_cmd_mod(cmd, &cmd->tokens[2], 2);
		}
		else
		{
			//printf("Uchunji tapgyrda-da tanalmady\n");
			//debug_cmd(cmd);
		}
	}
	//debug_cmd(cmd);
	return 1;
}


// Def_var komandasy uchin tokene gora maglumatlaryny uytgedyar
int def_var_cmd_mod(command *cmd, token *tok, int tok_num)
{
	if (tok->type_class==IDENT_TYPE_CLASS)
	{
		if (tok_num==cmd->tokens_num-1)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;
			
			cmd->is_compl = 1;
			cmd->ns = 1;
		
			return 1;
		}
		return 0;
	}
	else if (tok->type_class==DEF_TYPE_TYPE_CLASS)
	{
		if (cmd->tokens_num>tok_num+1)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;

			cmd->ns = 1;
		
			return 1;
		}
		return 0;
	}
	else if (tok->type_class==GLOB_TYPE_CLASS)
	{
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

/*
 * Tokeni komanda salyar
 *
 * 0 - yalnyshlyk bar
 * 1 - hemme zat showly boldy
**/
int add_to_cmd(command *cmd, token *tok)
{
	// Komanda-da gaty kan tokenler bar
	//if (cmd->tokens_num==CMD_MAX_TOKENS)
	//	return 0;

	cmd->tokens[cmd->tokens_num++] = *tok;
	return 1;
}

// Komandanyn global ulni yglan etmedigini barlayar
int is_glob_def_var_cmd(command *cmd)
{
	if (cmd->cmd_class==DEF_VAR_CLASS_NUM && cmd->cmd_type==DEF_VAR_TYPE_NUM && cmd->ns==GLOB)
		return 1;
	return 0;
}

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_var_cmd(command *cmd)
{
	if (cmd->cmd_class==DEF_VAR_CLASS_NUM && cmd->cmd_type==DEF_VAR_TYPE_NUM)
		return 1;
	return 0;
}

// Komandalar bilen ishleyan bolum
int work_with_cmd()
{
	// Hazir TPL-in fayly komandalar bilen ishleyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 4;
	// Komanda saygarylyp showly gutardy
	if (!recognize_cmd(&cmd))
	{
		
		print_err(CODE4_CANT_IDENT_CMD);
	}
	
	//printf("Komanda showly saygaryldy\n");
	if (is_glob_def_var_cmd(&cmd))
	{
		// Komandany global funksiyalara goshjak bolyas
		glob_vars_add_cmd(cmd);
		//printf("global ulni yglan edilipdir\n");
	}
	else if (is_def_var_cmd(&cmd))
	{
		//printf("Lokal ulni yglan edilipdir\n");
		//debug_cmd(&cmd);
		loc_vars_add_cmd(cmd);
	}
	else
	{
		// Komandany algoritme goshulyar
		algor_add_cmd(cmd);
		//printf("Komanda algoritme goshuldy\n");
	}
	CUR_PART = prev_part;
	return 1;
}
