/*
All Command information
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "cmds.h"
#include "token/token_types.h"
#include "cmd/assign.h"
#include "cmd/def_var.h"
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
	cmd->value_class = 0;
	cmd->value_type = 0;
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

	if (!add_to_def_var_list(&cmd)) // Komanda ulnini yglan etme dal eken
    {
		// Komandany algoritme goshulyar
		algor_add_cmd(cmd);
	}

	CUR_PART = prev_part;
	return 1;
}




