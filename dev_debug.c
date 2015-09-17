/*
 * This file includes functions for debugging while compiler building
**/
#include <stdio.h>
#include <string.h>

#include "tpl.h"
#include "tokens.h"
#include "cmd.h"
#include "algor.h"
#include "dev_debug.h"

void debug_cmd(command *cmd)
{
	char *cmd_class = get_cmd_class(cmd->cmd_class);
	char *cmd_type = get_cmd_class_type(cmd->cmd_class, cmd->cmd_type);
	
	char *complete = (cmd->is_compl) ? "Complete" : "Not complete";
	
	char *ns = (cmd->ns==1)?"local":"global";
	
	printf("\nINFO ABOUT COMMAND++++++++++++++++++++++++++\n");
	printf("'%s', '%s' command, in '%s'->'%s'\n", ns, complete, cmd_class, cmd_type);
	printf("Has %d tokens\n\n", cmd->tokens_num);
	if (cmd->tokens_num>0)
	{
		int i;
		printf("Tokens are:\n");
		//printf("  potentional types: %d", tok->potentional_types_num);
		for(i=0; i<cmd->tokens_num; i++)
			debug_token(&cmd->tokens[i]);
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
	
	if (tok_type->type_class==DEF_TYPE_TYPE_CLASS)
		strncpy(type, def_type_list[tok_type->type_num].tk_name, strlen(def_type_list[tok_type->type_num].tk_name)+1);
	else
		strncpy(type, "", strlen("")+1);
		
	printf("\tTOK_TYPE------------\n");
	printf("\t('%s') '%s'->'%s' token type\n", complete, type_class, type);
	
	if (tok_type->need_value==0)
		printf("\tValue doesn't required\n");
	else
		printf("\tValue: '%s'\n", tok_type->value);
	printf("\n\n");
}


char *get_type_class(int type_class_num)
{
	if (type_class_num>0)
		return type_classes[type_class_num-1];
	
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

void debug_glob_def_vars(global_def_var *g)
{
	printf("\tGLOBAL YGLAN EDILEN ULNILER----\n");
	if (glob_def_vars_cmds<1)
	{
		printf("\n\tGlobal ulniler yglan edilmandir\n");
	}
	else
	{
		printf("\n");
		int i;
		for(i=0; i<glob_def_vars_cmds; ++i)
		{
			char *type_class = get_type_class(g[i].tok_class);

			const char *type       = def_type_list[ g[i].tok_type ].tk_name;
			printf("\t%d. %s faylda, (%s) %s %s\n", i+1, 
				g[i].file_name,
				type_class, 
				type, 
				g[i].tok_name);
		}
	}
	printf("\n\n\n");
}

void debug_loc_def_vars(local_def_var *l)
{
	printf("\tLOKAL YGLAN EDILEN ULNILER----\n");
	if (loc_def_vars_num<1)
	{
		printf("\n\tFaylda ulniler yglan edilmandir\n");
	}
	else
	{
		printf("\n");
		int i;
		for(i=0; i<loc_def_vars_num; ++i)
		{
			char *type_class = get_type_class(l[i].tok_class);

			const char *type       = def_type_list[ l[i].tok_type ].tk_name;
			printf("\t%d. %s faylda, (%s) %s %s\n", i+1, 
				cur_parse_file_name,
				type_class, 
				type, 
				l[i].tok_name);
		}
	}
	printf("\n\n\n");
}
