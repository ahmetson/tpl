/*
All Command information
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "cmds.h"
#include "dev_debug.h"
#include "error.h"

// Command can contain maximum 3 tokens
const int CMD_MAX_TOKENS = 4;
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


void init_cmd(command *cmd, char free_items)
{
	if (free_items && cmd->items_num)
	{
	    free(cmd->items);
	    cmd->items = 0;			// Kop free() funksiyany chagyrmakdan gorayar
	}
	cmd->items_num = 0;
	cmd->is_compl = 0;
	cmd->cmd_type = 0;
	cmd->cmd_class = 0;
	cmd->ns = -1;
	
}


int recognize_cmd(command *cmd)
{
	int prev_part = CUR_PART;
	CUR_PART = 4;
	//COPY CMD DIYMELI

	//printf("birinji tokeni eken\n");
	
	
	//printf("birinji token komanda goshuldy\n");
	// Komandanyn ikinji tokeninden, ta gutaryancha
	//if (!parse_cmd(cmd))
		{
			//printf("Komanda saygarylmady\n");
			//debug_cmd(cmd);
			//init_token(cmd->tokens_num--;
		}
		//else
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
	
	// Komandany saygaryp bolmady ya ol gutarylmadyk
	if (cmd->cmd_class<1 && cmd->cmd_type<1 || !cmd->is_compl)
	{
		//debug_cmd(cmd);
		return 0;
   }
   //if (cmd->items_num==4)
//	debug_cmd(cmd);
	CUR_PART = prev_part;
	// Komandany saygaryp boldy
	
	return 1;
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
		//debug_cmd(cmd);
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
	if (cmd->items_num>DEF_VAR_MAX_TOKENS)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class = 0;
		cmd->cmd_type = 0;
		printf("Tokenlerin sany gaty kan\n");
		return 0;
	}
	
	// Maksimum bolup biljek tokenlerden duryar, diymek global ulni yglan edilyar
	// Emma global ulni yglan edilmandir
	if (cmd->items_num==DEF_VAR_MAX_TOKENS && cmd->items[0].type==1 &&
	cmd->items[0].tok.type_class!=GLOB_TYPE_CLASS)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class = 0;
		cmd->cmd_type = 0;
		printf("Umumy ulni bolmaly");
		return 0;
	}

	int next=-1;	
	// Birinji token umumy ya maglumat tipi bolup bilyar
	if (cmd->items[0].type==1 &&
	cmd->items[0].tok.type_class==GLOB_TYPE_CLASS)
	{
		def_var_cmd_mod(cmd, &cmd->items[0].tok, 0);
		//debug_cmd(cmd);
		next = DEF_TYPE_TYPE_CLASS;
	}
	else if (cmd->items[0].type==1 &&
	cmd->items[0].tok.type_class==DEF_TYPE_TYPE_CLASS)
	{
		def_var_cmd_mod(cmd, &cmd->items[0].tok, 0);
		next = IDENT_TYPE_CLASS;
	}
	else
	{
		//printf("Hich zat tanalmady\n");
	}
	
	// Eger birinji umumylygy anladyan token bolsa onda ikinji token maglumat
	// 	tipi bolup bilyar
	// Yada identifikator bolup bilyar
	if (cmd->items[1].type==1 &&
	cmd->items[1].tok.type_class==next)
	{
		
		def_var_cmd_mod(cmd, &cmd->items[1].tok, 1);
		if (cmd->items[0].tok.type_class==GLOB_TYPE_CLASS)
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
	if (next>0)
	{ 
		if(cmd->items[2].type==1 &&
		next==cmd->items[2].tok.type_class)
		{
			def_var_cmd_mod(cmd, &cmd->items[2].tok, 2);
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
	else if (tok->type_class==DEF_TYPE_TYPE_CLASS)
	{
		// Eger ulni global ulninin yglan edilshi bolsa, ikinji token,
		// yogsa komanda-da birinji token bolmaly
		if ((cmd->items[0].type==1 && 
			 cmd->items[0].tok.type_class==GLOB_TYPE_CLASS &&
			 tok_num==1) ||
			 tok_num==0)
		{
			cmd->cmd_class = 1;
			cmd->cmd_type = 1;
		
			return 1;
		}
		return 0;
	}
	else if (tok->type_class==GLOB_TYPE_CLASS)
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

/*
 * Tokeni komanda salyar
 *
 * 0 - yalnyshlyk bar
 * 1 - hemme zat showly boldy
**/
int add_to_cmd(command *cmd, token *tok)
{
	// Komanda-da gaty kan tokenler bar
	if (cmd->items_num==CMD_MAX_TOKENS)
		return 0;

	command_item cmd_item = {};
	cmd_item.type = 1;
	cmd_item.tok = *tok;
	
	long size = (cmd->items_num+1)*sizeof(command_item);
	command_item *tmp = realloc(cmd->items,size);
	if (tmp)
	{
		cmd->items = tmp;
		cmd->items[cmd->items_num++] = cmd_item;
		//printf("Kuchada yer berildi (tokenlerin sany:%d gowrumi:%d)\n", cmd->items_num, size);
		return 1;
	}
	else
	{
		//printf("Kuchada yer tapylmady\n");
		return 0;
	}
	
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
	}
	else if (is_def_var_cmd(&cmd))
	{
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


/*
 * Bir komandany bashga komanda kopiyasyny yasayar
**/
int copy_cmd(command *to, command *from)
{
	to->cmd_class = from->cmd_class;
	to->cmd_type  = from->cmd_type;
	to->is_compl  = from->is_compl;
	to->ns        = from->ns;
	to->items_num = from->items_num;
	if (from->items_num)
	{
		int i;
		long size = sizeof(from->items[0]) * from->items_num;
		to->items = malloc(size);
		for(i=0; i<to->items_num; ++i)
		{
			to->items[i] = from->items[i];
		}
	}
	return 1;
}

/*
 * ichindaki obyektleri kuchadan boshadyar
**/
int free_items(command *c)
{
	if (c->items_num)
		free(c->items);
	return 1;
}
