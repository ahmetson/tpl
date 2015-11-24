/*
 * This file includes functions for debugging while compiler building
**/
#include <stdio.h>
#include <string.h>

#include "tpl.h"
#include "tokens.h"
#include "cmds.h"
#include "algor.h"
#include "paren/types.h"
#include "dev_debug.h"

void debug_cmd(command *cmd)
{
	char *cmd_class = get_cmd_class(cmd->cmd_class);
	char *cmd_type = get_cmd_class_type(cmd->cmd_class, cmd->cmd_type);

	char *complete = (cmd->is_compl) ? "Complete" : "Not complete";

	char *ns = (cmd->ns==1)?"local":"global";

	printf("\nINFO ABOUT COMMAND++++++++++++++++++++++++++\n");
	printf("'%s', '%s' command, in '%s'->'%s'\n", ns, complete, cmd_class, cmd_type);
	printf("Has %d tokens\n\n", cmd->items_num);
	if (cmd->items_num>0)
	{
		int i;
		printf("Tokens are:\n");
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<cmd->items_num; i++)
		{
			if (cmd->items[i].type==TOKEN_ITEM)
				debug_token(&cmd->items[i].tok);
			else if(cmd->items[i].type==CMD_ITEM)
				debug_cmd(&cmd->items[i].cmd);
            else if(cmd->items[i].type==PAREN_ITEM)
                debug_paren(&cmd->items[i].paren);
		}
	}
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");

}

void debug_token(token *tok)
{
	char *ns = (tok->ns==1)?"local":"global";

	char *type_class = get_type_class(tok->type_class);

	char *complete = (tok->is_compl) ? "Complete" : "Not complete";

	// Print results
	printf("\n  INFO ABOUT TOKEN===================\n");
	printf("File: '%s'; Line: %d; Char: %c, position is: %d\n", FILES[tok->inf_file_num].source, tok->inf_line_num,
        tok->inf_char==-1 ? '?' : tok->inf_char, tok->inf_char_num);
	printf("  '%s', '%s' token, in '%s' class\n", ns, complete, type_class);
	printf("  Can be in %d types\n\n", tok->potentional_types_num);
	if (tok->potentional_types_num>0)
	{
		int i;
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<tok->potentional_types_num; i++)
			debug_token_type(&tok->potentional_types[i]);
	}
}


void debug_token_type(token_type *tok_type)
{
	char type[256];
	char *complete = (tok_type->is_compl) ? "Complete" : "Not complete";
	char *type_class = get_type_class(tok_type->type_class);

	if (tok_type->type_class==TOK_CLASS_DEF_TYPE)
		strncpy(type, def_type_list[tok_type->type_num].tk_name, strlen(def_type_list[tok_type->type_num].tk_name)+1);
	else if (tok_type->type_class==TOK_CLASS_ASSIGN)
	{
		strncpy(type, ASSIGN_TOK_NUM_WORDS[tok_type->type_num], strlen(ASSIGN_TOK_NUM_WORDS[tok_type->type_num])+1);
	}
	else if (tok_type->type_class==TOK_CLASS_CONST_DATA)
    {
        printf("%s %d", CONST_DATA_TOK_NUM_WORDS[tok_type->type_num], tok_type->type_num);
        strncpy(type, CONST_DATA_TOK_NUM_WORDS[tok_type->type_num], strlen(CONST_DATA_TOK_NUM_WORDS[tok_type->type_num])+1);
    }
    else
		strncpy(type, "", strlen("")+1);


	printf("\tTOK_TYPE------------\n");
	printf("\t('%s') '%s'->'%s' token type\n", complete, type_class, type);

	if (tok_type->need_value==0)
		printf("\tValue doesn't required\n");
	else
		printf("\tValue: '%s'\n", get_tok_type_value(tok_type));
	printf("\n\n");
}

char *get_tok_type_value(token_type *tok_type)
{
    if (tok_type->type_class==TOK_CLASS_CONST_DATA && tok_type->type_num==STRING_CONST_DATA_TOK_NUM)
    {
        return *tok_type->string_value;
    }
    return tok_type->value;
};

char *get_type_class(int type_class_num)
{
	if (type_class_num>0)
		return type_classes[type_class_num];

	return "Tipin klasy nabelli";
}

char *get_cmd_class(int cmd_class_num)
{
	if (cmd_class_num>0)
		return cmd_classes[cmd_class_num-1];

	return "Komandanyn klasy nabelli";
}

char *get_cmd_class_type(int cmd_class_num, int cmd_type_num)
{
	if (cmd_class_num>0 && cmd_type_num>0)
		return cmd_class_types[cmd_class_num-1][cmd_type_num-1];

	return "Komandanyn tipi nabelli";
}

void debug_GLOB_VAR_DEFS()
{
	printf("\tGLOBAL YGLAN EDILEN ÜLŇILER------------------\n");

    printf("\n");
    int i;
    for(i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
    {
        char *type_class = get_type_class(GLOBAL_VAR_DEFS[i].type_class);

        const char *type       = def_type_list[ GLOBAL_VAR_DEFS[i].type_num ].tk_name;
        printf("\t%d. %s faýlda, (%s) %s %s\n", i+1,
            FILES[GLOBAL_VAR_DEFS[i].inf_file_num].source,
            type_class,
            type,
            GLOBAL_VAR_DEFS[i].name);
    }
	printf("\n\n\n");
}


void debug_LOCAL_VAR_DEFS()
{
	printf("\tLOKAL YGLAN EDILEN ÜLŇILER------------------\n");

	printf("\n");
    int i;
    for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
    {
        char *type_class = get_type_class(LOCAL_VAR_DEFS[i].type_class);

        const char *type       = def_type_list[LOCAL_VAR_DEFS[i].type_num ].tk_name;
        printf("\t%d. %s faýlda, (%s) %s %s\n", i+1,
            CUR_FILE_NAME,
            type_class,
            type,
            LOCAL_VAR_DEFS[i].name);
    }

	printf("\n\n\n");
}



void debug_paren(parenthesis *paren)
{
	char *paren_type  = get_paren_type(paren->type);

	printf("\nINFO ABOUT PARENTHESIS++++++++++++++++++++++++++\n");
	printf("'%s' parenthesis\n", paren_type);
	printf("Has %d items\n\n", paren->elems_num);
	if (paren->elems_num>0)
	{
		int i;
		printf("Items are:\n");
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<paren->elems_num; i++)
		{
			if (paren->elems[i].type==TOKEN_ITEM)
				debug_token(&paren->elems[i].tok);
			else if(paren->elems[i].type==CMD_ITEM)
				debug_cmd(&paren->elems[i].cmd);
            else if(paren->elems[i].type==PAREN_ITEM)
                debug_paren(&paren->elems[i].paren);
		}
	}
	printf("++++++++++++++++++++++++++++++++++++++++++++\n");

}


char *get_paren_type(int paren_type)
{
    return PAREN_TYPES_WORDS[paren_type];
}
