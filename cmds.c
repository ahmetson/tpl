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
#include "cmd/fn_call.h"
#include "dev_debug.h"
#include "error.h"
#include "main/inf.h"

// Komandanyň birliginiň nomeri
const int UNKNOWN_ITEM  = 0;
const int TOKEN_ITEM    = 1;
const int CMD_ITEM      = 2;
const int PAREN_ITEM    = 3;

// Command can contain maximum 3 tokens
const int CMD_MAX_TOKENS = 4;
const int DEF_VAR_MAX_TOKENS = 3;
const int ASSIGN_MAX_ITEMS = 3;
const int FN_CALL_MAX_ITEMS = 2;

// Komandalaryn sanawy
int CMD_CLASS_UNKNOWN = 0;
int CMD_CLASS_DEF_VAR = 1;
#define CONST_CMD_CLASS_ASSIGN 2
int CMD_CLASS_ASSIGN = 2;
#define ASSGNI_LEFT_TYPE_NUM 1
#define ASSIGN_RIGHT_TYPE_NUM 2
int CMD_CLASS_FN = 3;
#define FN_CALL 0

#define CONST_GLOB 0
int GLOB = 0;
#define CONST_LOCAL 1
int LOCAL = 1;

is_cmd_item cmd_types[] = {
	   {is_cmd_def_var},
	   {is_cmd_assign},
	   {is_cmd_fn_call}
};

// Dine debug uchin ulanylyar. Komanda tiplerinin atlary
char *cmd_classes[] = {
	"var",
	"assign",
	"function"
};

char *cmd_class_types[][MAX_CLASS_TYPES] = {
	{"def", 0},			// Var class
	{"left", "right"},
	{"call", 0}
};


int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type) = {
    {empty_cmd_return_type, empty_cmd_return_type},
    {cmd_def_var_return_type, empty_cmd_return_type},//CMD_CLASS_DEF_VAR = 1;
    {empty_cmd_return_type, empty_cmd_return_type},//CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_return_type, empty_cmd_return_type} //CMD_CLASS_FNS = 3;
};


// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd) = {
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},
    {empty_cmd_checking_semantic, empty_cmd_checking_semantic},//CMD_CLASS_DEF_VAR = 1;
    {semantic_cmd_assign,         empty_cmd_checking_semantic},//CMD_CLASS_ASSIGN = 2;
    {semantic_cmd_fn_call,        empty_cmd_checking_semantic} //CMD_CLASS_FNS = 3;
};


// Komandalaryň klasy we tipi boýunça, komandanyň tekstini ýazýan funksiýa çagyrylýar.
void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, char **l, int *len) = {
    {empty_cmd_c_code,    empty_cmd_c_code},
    {empty_cmd_c_code,    empty_cmd_c_code}, //CMD_CLASS_DEF_VAR = 1;
    {cmd_assign_c_code,   cmd_assign_c_code},//CMD_CLASS_ASSIGN = 2;
    {cmd_fn_call_c_code,  empty_cmd_c_code}  //CMD_CLASS_FNS = 3;
};


void init_cmd(command *cmd, char free_items)
{
	if (free_items && cmd->items_num)
	{
	    free(cmd->items);
	    cmd->items = 0;			// Kop free() funksiyany chagyrmakdan gorayar
	}
	cmd->items_num =
	cmd->is_compl =
	cmd->cmd_type =
	cmd->cmd_class =
	cmd->value_class =
	cmd->value_type =
	cmd->parenthesis = 0;
	cmd->ns = -1;

}


int recognize_cmd(command *cmd)
{
	// Komandany saygaryp bolmady ya ol gutarylmadyk
	if ((cmd->cmd_class<1 && cmd->cmd_type<1) || !cmd->is_compl)
	{
		//debug_cmd(cmd);
		return 0;
    }
	// Komandany saygaryp boldy
	return 1;
}

/**
 * Tokeni saygarylyar
**/
int parse_cmd(command *cmd)
{

	int i;
	for (i=0; i<CMDS_TYPES_NUM; i++)
	{
		if (cmd_types[i].is_cmd(cmd))
            return 1;
	}
	return 0;
}


// ==================================
// Komandalary saygaryan funksiyalar

/**
 * Tokeni komanda salyar
 *
 * 0 - yalnyshlyk bar
 * 1 - hemme zat showly boldy
**/
int add_to_cmd(command *cmd, int type, token tok, parenthesis paren, command subcmd)
{
	// Komanda-da gaty kan tokenler bar
	if (cmd->items_num==CMD_MAX_TOKENS)
		return 0;

	command_item cmd_item = {};
	cmd_item.type = type;
	if (type==TOKEN_ITEM)
        cmd_item.tok = tok;
    else if(type==CMD_ITEM)
        cmd_item.cmd = subcmd;
    else if (type==PAREN_ITEM)
        cmd_item.paren = paren;

	long size = (cmd->items_num+1)*sizeof(command_item);
	command_item *tmp = realloc(cmd->items, size);
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

    // TODO
    // KOMANDA gutarylmadyk bolsa, yalnyshlyk haty komanda gutarylmady diyip gaytaryar
	// Komanda saygarylyp showly gutardy
	if (!recognize_cmd(&cmd))
	{
	    //debug_cmd(&cmd);
	    //printf("Salam");
		print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(&cmd));
	}

	if (!add_to_def_var_list(&cmd)) // Komanda ulnini yglan etme dal eken
    {
		// Komandany algoritme goshulyar
		algor_add_cmd(cmd);
	}

	CUR_PART = prev_part;
	return 1;

}

/** Komanda birlik hökmünde başga komandany goşýar
    \cmd    - içine goşulýar
    \addcmd - goşulmaly komanda

**/
int cmd_add_subcmd(command *cmd, command addcmd)
{
    return 1;
}

/** Komanda birlik hökmünde başga komandany goşýar
    \cmd    - içine goşulýar
    \paren  - goşulmaly komanda

**/
int cmd_add_subparen(command *cmd, parenthesis paren)
{
    int prev_part = CUR_PART;
	CUR_PART = 4;

	// Komandanyň öňki birligi gutarylan bolmaly
	if (cmd->items_num!=0 &&
       ((cmd->items[cmd->items_num-1].type==TOKEN_ITEM && cmd->items[cmd->items_num-1].tok.is_compl==0) ||
        (cmd->items[cmd->items_num-1].type==CMD_ITEM   && cmd->items[cmd->items_num-1].cmd.is_compl==0)))
    {
        CUR_PART = 3;
        print_err(CODE3_PREV_TOK_INCORRECT, &inf_tok);
    }

    //printf("Token komanda goshulmana tayynlandy\n");
	if (!add_to_cmd(cmd, PAREN_ITEM, inf_tok, paren, *cmd))  print_err(CODE4_CANT_ADD_TOKEN, &inf_tok);


	//printf("komanda barlanmana gechmeli\n");
	if (!parse_cmd(cmd))
	{
		cmd->items_num--;
		if (!cmd->items_num || !parse_cmd(cmd))
        {
            cmd->items_num++;

			print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
        }
		else
		{
		    // Eger ulni yglan etme bolsa, onda ichki komandany ulni yglan etmelerin ichine gechirmeli
		    add_to_def_var_list(cmd);

			// Ichki komandalaryn sanyny kopeltmeli
			GLOB_SUBCMDS_NUM++;
			GLOB_SUBCMD_ITEMS_LIST = realloc(GLOB_SUBCMD_ITEMS_LIST, sizeof(command_item*)*GLOB_SUBCMDS_NUM);
			if (GLOB_SUBCMD_ITEMS_LIST==NULL)
			{
				//printf("Ichki komandalar uchin yer taplymady\n");
				debug_cmd(cmd);
				token *t=(token*)inf_get_last_token(cmd);
				print_err(CODE4_CANT_IDENT_CMD, t);
			}
			else
			{
				//printf("Ichki komandalar uchin yer bar\n");

				// Ichki komanda uchin yer.
				long size = cmd->items_num * sizeof(command_item);
				GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1] = malloc(size);

				// Ichki komanda uchin yere, birlikler gechirilyar.
				int i=0;
				for (i=0; i<cmd->items_num; ++i)
				{
					GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1][i] = cmd->items[i];
				}

				command new_cmd;
				new_cmd = *cmd;
				new_cmd.items = GLOB_SUBCMD_ITEMS_LIST[GLOB_SUBCMDS_NUM-1];

				init_cmd(cmd, 0);
				cmd->items_num = 2;

				command_item *tmp = realloc(cmd->items, cmd->items_num*sizeof(command_item));
				if (tmp==NULL)
				{
					//printf("Komanda ichki komandany goshmak uchin yer tapylmady\n");
					print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
				}
				else
				{
					//printf("Komanda ichki komandany goshmak uchin yer bar\n");
					cmd->items = tmp;

					// Komandanyň birinji birligi - Önki komanda goşulýar
					command_item cmd_item = {};
					cmd_item.type = CMD_ITEM;
					cmd_item.cmd = new_cmd;
					cmd->items[0] = cmd_item;

					// Komandanyň ikinji birligi   - Täze token goşulýar
					command_item cmd_tok_item;
					cmd_tok_item.type  = PAREN_ITEM;
					cmd_tok_item.paren = paren;
					cmd->items[1] = cmd_tok_item;

					// Täze token öňki tokenlerden emele gelen komanda bilen täze komandany emele getirmedi
					if (!parse_cmd(cmd))
                    {
                        //printf("Salam");
                        print_err(CODE4_CANT_IDENT_CMD, (token *)inf_get_last_token(cmd));
                    }
				}
			}
		}
	}

	CUR_PART = prev_part;
	return 1;
}


int empty_cmd_return_type(command *cmd, int *cmd_class, int *cmd_type)
{
    *cmd_class = TOK_CLASS_UNDEFINED;
    return 0;
}

int empty_cmd_checking_semantic(command *cmd)
{
    return 0;
}


/** Nätanyş ulanylan komandalara goşulýar

    \cmd - nätanyş komanda
    \cmdClass - gabat gelen komandanyň klasy
    \cmdType - gabat gelen komandanyň tipi
    \arg - komanda-da, şu tokeniň deregine garaşylan maglumat tipi
**/
void unknown_cmd_add(command *cmd, int cmd_class, int cmd_type, int waited_class, int waited_type)
{
    unknown_cmd unk;
    unk.cmd = cmd;
    unk.cmd_class = cmd_class;
    unk.cmd_type = cmd_class;
    unk.waited_class = waited_class;
    unk.waited_type = waited_type;

    ++UNKNOWN_CMDS_NUM;

    long size;
    size = sizeof(unk)*UNKNOWN_CMDS_NUM;
    UNKNOWN_CMDS = realloc(UNKNOWN_CMDS, size);

    UNKNOWN_CMDS[UNKNOWN_CMDS_NUM-1] = unk;
}



void empty_cmd_c_code(command *cmd, char **l, int *len)
{
    return;
}
