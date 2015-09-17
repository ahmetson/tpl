/*
 * Algoritmli fayly yasayan funksiya
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "algor.h"
#include "translator_to_c.h"
#include "dev_debug.h"

int algor_add_cmd(command cmd)
{
	/*
	int size = sizeof(cmd);
	if (cmd.tokens_num>0)
		size += (cmd.tokens_num * sizeof(token));*/
	
	// Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar	
	long new_cur_file_algor_size = cur_file_algor_size+sizeof(cmd);
	//printf("%d new size\n", new_cur_file_algor_size);
	cur_file_algor = realloc(cur_file_algor, new_cur_file_algor_size);
	
	cur_file_algor[cur_file_algor_cmds++] = cmd;
	
	cur_file_algor_size = new_cur_file_algor_size;
	
	return 1;
}

// Global yglan edilen funksiyalaryn hataryna yglan edilen ulni goshulyar
int glob_vars_add_cmd(command cmd)
{
	// Yglan edilen ulniler boyuncha gidilyar.
	// Eger eyyam sheyle ulni yglan edilen bolsa, chykylyar.
	
	// Lokal yglan edilen funksiyalar boyuncha barlanyar.
	// Eger eyyam sheyle lokal ulni yglan edilen bolsa, yalnyshlyk gaytarylyar.
	char *tok_name = cmd.tokens[2].potentional_types[0].value;
	/*
	int size = sizeof(cmd);
	if (cmd.tokens_num>0)
		size += (cmd.tokens_num * sizeof(token));*/
	//printf("%s -> %s\n", cur_parse_file_name, tok_name);
	//printf("%d -> %d", cmd.tokens[1].potentional_types[0].type_class, cmd.tokens[1].potentional_types[0].type_num);
	//debug_cmd(&cmd);
	global_def_var new_def = {
		"", 
		"",
		cmd.tokens[1].potentional_types[0].type_class,
		cmd.tokens[1].potentional_types[0].type_num
	};
	strncpy(new_def.file_name, cur_parse_file_name, strlen(cur_parse_file_name)+1);
	strncpy(new_def.tok_name, tok_name, strlen(tok_name)+1);

	
	//debug_token(&cmd.tokens[1]);
	//printf("\n%s -> %s\n", new_def.file_name, new_def.tok_name);
	//printf("%d -> %d\n", new_def.tok_class, new_def.tok_type);

	
	// Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar	
	long new_size = glob_def_vars_size+sizeof(new_def);
	//printf("%d new size\n", new_cur_file_algor_size);
	glob_def_vars = realloc(glob_def_vars, new_size);
	
	glob_def_vars[glob_def_vars_cmds++] = new_def;
	
	glob_def_vars_size = new_size;
	
	return 1;
	
}


void init_loc_def_vars()
{
	loc_def_vars_size = loc_def_vars_num = 0;
}


int loc_vars_add_cmd(command cmd)
{
	char *tok_name = cmd.tokens[1].potentional_types[0].value;
	/*
	int size = sizeof(cmd);
	if (cmd.tokens_num>0)
		size += (cmd.tokens_num * sizeof(token));*/
	//printf("%s -> %s\n", cur_parse_file_name, tok_name);
	//printf("%d -> %d", cmd.tokens[0].potentional_types[0].type_class, cmd.tokens[0].potentional_types[0].type_num);
	//debug_cmd(&cmd);
	//debug_cmd(&cmd);
	//debug_token_type(&cmd.tokens[0].potentional_types[0]);
	local_def_var new_def = {
		"",
		cmd.tokens[0].potentional_types[0].type_class,
		cmd.tokens[0].potentional_types[0].type_num
	};
	strncpy(new_def.tok_name, tok_name, strlen(tok_name)+1);

	
	//debug_token(&cmd.tokens[1]);
	//printf("%d -> %d\n", new_def.tok_class, new_def.tok_type);

	
	// Taze gosuljak komandanyn gowrumi, kuchadaky eyelenen gowrume goshulyar	
	long new_size = loc_def_vars_size+sizeof(new_def);
	//printf("%d new size\n", new_cur_file_algor_size);
	loc_def_vars = realloc(loc_def_vars, new_size);
	
	loc_def_vars[loc_def_vars_num++] = new_def;
	
	loc_def_vars_size = new_size;
	
	return 1;
}


/*void write_loc_def_var()
{
	char source[MAX_FILE_LEN];
	strncat(source, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(source, "\\", strlen("\\"));
	strncat(source, cur_parse_file_name, strlen(cur_parse_file_name));
	strncat(source, ".def_var", strlen(".def_var"));
	printf("Yazylmaly fayl: %s\n", source);
}*/

// Lokal fayllar uchin
char loc_head_file[] = ".h";
char loc_source_file[] = ".c";

int is_glob_def_var_in_cur()
{
	int i;
	for(i=0; i<glob_def_vars_cmds; ++i)
	{
		if (strlen(glob_def_vars[i].file_name)==strlen(cur_parse_file_name) &&
			!strncmp(glob_def_vars[i].file_name, cur_parse_file_name, strlen(cur_parse_file_name)))
			return 1;
	}
	return 0;
}
