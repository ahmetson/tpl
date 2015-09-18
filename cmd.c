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

modify_cmd_item modify_cmd[] = {
	{def_var_cmd_mod}
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

	parse_cmd(cmd);
	// Bir tokenli komanda
	//if (parse_cmd(&new_cmd))
		
	//printf("birinji token komanda goshuldy\n");
	// Komandanyn ikinji tokeninden, ta gutaryancha
	for (tok_num=1; tok_num<prev_cmd.tokens_num; tok_num++)
	{
		//printf("hawa");
		cmd_add_token(cmd, prev_cmd.tokens[tok_num]);

		parse_cmd(cmd);
	}
	
	// Komandany saygaryp bolmady
	if (cmd->cmd_class<1 && cmd->cmd_type<1)
		return 0;
	
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
		if (!cmd_types[i].is_cmd(cmd))
			return 0;
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
	// Maximum 3 sany token bolup bilyar: umumy tip ident
	int max_tok_num = 3;

	if (cmd->tokens_num>max_tok_num)
	{
		// tokenler gaty kop
		return 0;
	}

	// Komanda-da bolup bilyan tokenlerin sanawy
	cmd_token can_be_tokens[] = {
		{GLOB_TYPE_CLASS,     -1, 0},
		{DEF_TYPE_TYPE_CLASS, -1, 1},
		{IDENT_TYPE_CLASS,    -1, 1}
	};
	
	//debug_cmd(cmd);
	int tok_num, list_tok_num;
	for (tok_num=cmd->tokens_num-1, list_tok_num=max_tok_num-1; 
		 tok_num>=0;
		 tok_num--, list_tok_num--)
	{
		if (is_tok_in_list( &(can_be_tokens[list_tok_num]), &(cmd->tokens[tok_num]) ))
		{
			// Tokenin tipine gora, komandany uytgedyan funksiya chagyrylyar
			if (!modify_cmd[DEF_VAR_CLASS_NUM-1].by_token(cmd, &(cmd->tokens[tok_num]), tok_num))
			{
				//printf("Komanda-da maglumatlary uytgedip bolanok!\n");
				return 0;
			}
			//debug_token( &(cmd->tokens[tok_num]));
			//debug_cmd(cmd);
		}
		else
			return 0;
	//printf("Sanawda: %d, Komanda-da: %d\n", can_be_tokens[list_tok_num].token_class, cmd->tokens[tok_num].type_class);
	}

	return 1;
}


// Berlen tokenin sanawdaky token bilen gabat gelyandigini barlayar
int is_tok_in_list(cmd_token *list_tok, token *tok)
{
	if (list_tok->token_class==tok->type_class)
	{
		if (list_tok->token_type!=-1 &&
			list_tok->token_type!=tok->potentional_types[0].type_num)
			return 0;		// Tokenlerin tipleri gabat gelmedi
		return 1;
	}
	return 0;
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
	if (cmd->tokens_num==CMD_MAX_TOKENS)
		return 0;

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
