/*
All Command information
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "cmds.h"
#include "token/token_types.h"
#include "dev_debug.h"
#include "error.h"

// Komandanyň birliginiň nomeri
const int TOKEN_ITEM = 1;
const int CMD_ITEM   = 2;

// Command can contain maximum 3 tokens
const int CMD_MAX_TOKENS = 4;
const int DEF_VAR_MAX_TOKENS = 3;
const int ASSIGN_MAX_ITEMS = 3;

// Komandalaryn sanawy
int CMDS_TYPES_NUM = 2;
#define CONST_DEF_VAR_CLASS_NUM 1
int DEF_VAR_CLASS_NUM = 1;
#define DEF_VAR_TYPE_NUM 1
#define CONST_ASSIGN_CLASS_NUM 2
int ASSIGN_CLASS_NUM = 2;
#define ASSGNI_LEFT_TYPE_NUM 1
#define ASSIGN_RIGHT_TYPE_NUM 2

#define CONST_GLOB 0
int GLOB = 0;
#define CONST_LOCAL 1
int LOCAL = 1;

is_cmd_item cmd_types[] = {
	   {is_cmd_def_var},
	   {is_cmd_assign}
};


// Dine debug uchin ulanylyar. Komanda tiplerinin atlary
char *cmd_classes[] = {
	"var",
	"assign"
};

#define CONST_MAX_CLASS_TYPES 2
char *cmd_class_types[][CONST_MAX_CLASS_TYPES] = {
	{"def", 0},			// Var class
	{"left", "right"}
};

/* Komandanyn bashynda bolup bilyan birlilerin sany
 *
 * Ulni yglan etmek uchin:
 *		global, tip
 * Baglanma uchin (Hazirlikche dine chep gornushi uchin):
 *		ulni yglan etmek
 *      identifikator
 *
 * Jemi 4 sany birlikden bashlap bilyar:
 *      global, tip, identifikator, ulni yglan etmek
**/
int CMD_FIRST_ITEMS_NUM = 4;

// Bashynda bolup bilyan tokenlerin klasy
command_item_recog cmd_first_items_classes[] = {
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0}
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
	// Komandany saygaryp bolmady ya ol gutarylmadyk
	if (cmd->cmd_class<1 && cmd->cmd_type<1 || !cmd->is_compl)
	{
		//debug_cmd(cmd);
		return 0;
    }

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
	if (cmd->items_num>DEF_VAR_MAX_TOKENS || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class=0;
		cmd->cmd_type=0;
		return 0;
	}

	// Maksimum bolup biljek tokenlerden duryar, diymek global ulni yglan edilyar
	// Emma global ulni yglan edilmandir
	if (cmd->items_num==DEF_VAR_MAX_TOKENS && cmd->items[0].type==TOKEN_ITEM &&
		cmd->items[0].tok.type_class!=GLOB_TYPE_CLASS)
	{
		// Komandany saygaryp bolmady
		//printf("Umumy ulni bolmaly");
		//cmd->cmd_class=0;
		//cmd->cmd_type=0;
		return 0;
	}

	//printf("Birinji token barlanmaly\n");
	int next=-1;
	// Birinji token umumy ya maglumat tipi bolup bilyar
	if (cmd->items[0].type==TOKEN_ITEM &&
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
		// Komandany saygaryp bolmady
		//cmd->cmd_class = 0;
		//cmd->cmd_type = 0;
		//printf("Umumy ulni bolmaly");
		return 0;
	}
		//printf("Birinji token barlanmady: %d\n", next);

	// Eger birinji umumylygy anladyan token bolsa onda ikinji token maglumat
	// 	tipi bolup bilyar
	// Yada identifikator bolup bilyar
	if (cmd->items_num>=1)
	{
		if ( cmd->items[1].type==TOKEN_ITEM &&
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
			// Komandany saygaryp bolmady
			//cmd->cmd_class=0;
			//cmd->cmd_type=0;
			return 0;

		}
	}
	//debug_cmd(cmd);
	//printf("Ikinji token barlanmady\n");
	// Eger birinji umumylygy anladyan token bolmasa,
	//	identifikator bolup bilyar.
	if (cmd->items_num>=2)
	{
		if(next>0 && cmd->items[2].type==TOKEN_ITEM &&
		next==cmd->items[2].tok.type_class)
		{
			def_var_cmd_mod(cmd, &cmd->items[2].tok, 2);
		}
		else
		{
			//printf("Uchunji tapgyrda-da tanalmady\n");
			//debug_cmd(cmd);
			// Komandany saygaryp bolmady
			//printf("Umumy ulni bolmaly");
			// Komandany saygaryp bolmady
			//cmd->cmd_class=0;
			//cmd->cmd_type=0;
			return 0;
		}
	}
	//printf("Uchunji token barlanmady\n");
	return 1;
}

// 2. Maglumat baglama komandany saygaryar
int is_cmd_assign(command *cmd)
{
	if (cmd->items_num>ASSIGN_MAX_ITEMS || !cmd->items_num)
	{
		// Komandany saygaryp bolmady
		cmd->cmd_class=0;
		cmd->cmd_type=0;
		return 0;
	}

	//printf("Birinji birlik barlanmaly\n");
	int next_item_class=-1, next_item_type=-1;
	// Birinji token ya ulni yglan etmek bolmaly ya identifikator.
	if (cmd->items[0].type==TOKEN_ITEM &&
		cmd->items[0].tok.type_class==IDENT_TYPE_CLASS)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=ASSIGN_TYPE_CLASS;
	}
	else if (cmd->items[0].type==CMD_ITEM &&
		cmd->items[0].cmd.cmd_class==DEF_VAR_CLASS_NUM)
	{
		assign_cmd_mod(cmd, 0);
		next_item_class=ASSIGN_TYPE_CLASS;
		// Indiki token dine chepe tarap baglanma bolmaly
		next_item_type= LEFT_ASSIGN_TOK_NUM;
	}
	else
	{
		//debug_cmd(cmd);
		//printf("Birinji bolum, tokeninki dal\n");
		// Komandany saygaryp bolmady
		//printf("Umumy ulni bolmaly");
		// Komandany saygaryp bolmady
		//cmd->cmd_class=0;
		//cmd->cmd_type=0;
		return 0;
	}
	//debug_cmd(cmd);
	//printf("Birinji birlik tanaldy\n");

	// Ikinji birlik, eger birinji ulni yglan edilme bolsa,
	// Baglanmanyn chepe gidyan token gornushi bolmaly, yogsa ikisem bolyar
	if(cmd->items_num>=1)
	{
		if (next_item_class>0 &&
			cmd->items[1].type==TOKEN_ITEM &&
			cmd->items[1].tok.type_class==ASSIGN_TYPE_CLASS)
		{
			// Token dine chepe tarap baglanma bolmaly
			if (next_item_type>0 && !cmd->items[1].tok.potentional_types[0].type_num==next_item_type)
			{
				//printf("Token chepe tarap bakyan dal\n");
				// Komandany saygaryp bolmady
				//cmd->cmd_class = 0;
				//cmd->cmd_type = 0;
				//printf("Umumy ulni bolmaly");
				return 0;
			}
			assign_cmd_mod(cmd, 1);
			//printf("Birlik baglanmanynky eken\n");
		}
		else
		{
			//printf("Ikinji tapgyrda-da tanalmady\n");
			// Komandany saygaryp bolmady
			//printf("Umumy ulni bolmaly");
			// Komandany saygaryp bolmady
			//cmd->cmd_class=0;
			//cmd->cmd_type=0;
			return 0;
			//debug_cmd(cmd);
		}
	}
	//debug_cmd(cmd);
	//printf("Ikinji bolum tanaldy");

	// Uchunji token identifikator bolmaly.
	if (cmd->items_num>=2)
	{
	    if(cmd->items[2].type==TOKEN_ITEM &&
		   cmd->items[2].tok.type_class==IDENT_TYPE_CLASS)
		{
			assign_cmd_mod(cmd, 2);
		//printf("Uchunji birlik identifikator eken\n");
		}
		else
		{
		//printf("Uchunji tapgyrda-da tanalmady\n");
		// Komandany saygaryp bolmady
		//printf("Umumy ulni bolmaly");
		// Komandany saygaryp bolmady
		//cmd->cmd_class=0;
		//cmd->cmd_type=0;
			return 0;
		//debug_cmd(cmd);
		}
	}

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


// Assign komandasy uchin tokene gora maglumatlaryny uytgedyar
int assign_cmd_mod(command *cmd, int tok_num)
{
	// Birinji birlik
	if (tok_num==0)
	{

		cmd->cmd_class = ASSIGN_CLASS_NUM;

		cmd->is_compl = 0;

		// Eger birlik global ulni yglan etme bolsa
		// Bu token in sonky. Shonun uchin lokalmy ya yokdugyny barlap bolyar
		if (cmd->items[0].type==CMD_ITEM &&
			is_def_var_cmd(&cmd->items[0].cmd) &&
			!is_glob_def_var_cmd(&cmd->items[0].cmd) ||
			cmd->items[0].type==TOKEN_ITEM &&
			cmd->items[0].tok.ns==LOCAL)
		{
			cmd->ns = LOCAL;
		}
		return 1;
	}
	else if (tok_num==1)
	{
		cmd->cmd_type = cmd->items[1].tok.potentional_types[0].type_num;
		cmd->cmd_class = ASSIGN_CLASS_NUM;

		cmd->is_compl = 0;

		return 1;
	}
	else if (tok_num==2)
	{
		cmd->cmd_class = ASSIGN_CLASS_NUM;

		cmd->is_compl = 1;
		return 1;
	}
	return 0;
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
	if (!add_to_def_var_list(&cmd)) // Komanda ulnini yglan etme dal eken
    {
		// Komandany algoritme goshulyar
		algor_add_cmd(cmd);
		//printf("Komanda algoritme goshuldy\n");
	}
	CUR_PART = prev_part;
	return 1;
}



/*
 * Eger komanda ulny yglan etmek bolsa, ulni yglan etmanin sanawynda goshyar.
**/
int add_to_def_var_list(command *cmd)
{
    // Hazir TPL-in fayly komandalar bilen ishleyan boluminde
	int prev_part = CUR_PART;
	CUR_PART = 4;

	//printf("Komanda showly saygaryldy\n");
	if (is_glob_def_var_cmd(cmd))
	{
		// Komandany global funksiyalara goshjak bolyas
		if (!glob_vars_add_cmd(*cmd))
            print_err(CODE4_VARS_IDENT_USED);
	}
	else if (is_def_var_cmd(cmd))
	{
		//debug_cmd(&cmd);
		if (!loc_vars_add_cmd(*cmd))
            print_err(CODE4_VARS_IDENT_USED);
	}
	else
    {
        // Ulni yglan etme komanda dal
        return 0;
    }

	CUR_PART = prev_part;
    return 1;
}
