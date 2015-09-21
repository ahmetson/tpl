/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "translator_to_c.h"

// file length - 255, command length - 95
#define MAX_PREP_LEN 350

// C kod nirede saklanyar?
char *C_SOURCE_FOLDER = "c:\\\\c_by_tpl";

// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, char *f_name)
{
	fputs("//Created by TPL.\n", f);
	fputs("//TPL author: Medet Atabayev.\n", f);

	char *line1 = malloc(MAX_PREP_LEN);
	strncpy(line1, "#ifndef ", strlen("#ifndef ")+1);
	strncat(line1, f_name, strlen(f_name));
	strncat(line1, "_H \n", strlen("_H \n"));
	fputs(line1, f);
	free(line1);

	
	line1 = malloc(MAX_PREP_LEN);
	strncpy(line1, "#define ", strlen("#define ")+1);
	strncat(line1, f_name, strlen(f_name));
	strncat(line1, "_H \n", strlen("_H \n"));
	fputs(line1, f);
	free(line1);
	
	fputs("\n\n", f);
	
	return 1;
}


int prepare_c_source(FILE *f, char *h_source)
{
	fputs("//Created by TPL.\n", f);
	fputs("//TPL author: Medet Atabayev.\n", f);

	char *line1 = malloc(MAX_PREP_LEN);
	strncpy(line1, "#include \"", strlen("#include \"")+1);
	strncat(line1, h_source, strlen(h_source));
	strncat(line1, "\" \n", strlen("\" \n"));
	fputs(line1, f);
	free(line1);
	
	fputs("\n\n", f);
	
	return 1;
}


int c_trans_header_add_glob_def_var(FILE *f)
{
	int i;
	long line_size = 100;
	long line_len = 0;
	
	int type_num;
	
	fputs("// Global yglan edilen ulniler.\n", f);
	
	for(i=0; i<glob_def_vars_cmds; ++i)
	{
		if (strlen(glob_def_vars[i].file_name)==strlen(cur_parse_file_name) &&
			!strncmp(glob_def_vars[i].file_name, cur_parse_file_name, strlen(cur_parse_file_name)))
		{
			type_num = glob_def_vars[i].tok_type;
			
			char *line = malloc(line_size);
			line_len = strlen("extern ")+1;
			strncpy(line, "extern ", line_len);
			
			// Ulninin tipi goshulyar
			if (line_len+strlen(def_type_list[type_num].value)>= line_size)
			{
				line_len += strlen(def_type_list[type_num].value)+100;
				line = realloc(line, line_len);
			}
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

			strncat(line, " ", strlen(" "));
			
			// Ulninin ady goshulyar
			if (line_len+strlen(glob_def_vars[i].tok_name)>= line_size)
			{
				line_len += strlen(glob_def_vars[i].tok_name)+100;
				line = realloc(line, line_len);
			}
			strncat(line, glob_def_vars[i].tok_name, strlen(glob_def_vars[i].tok_name));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;
			
			free(line);
		}
	}
	
	fputs("\n\n", f);
}


int c_trans_source_add_glob_def_var(FILE *f)
{
	int i;
	long line_size = 100;
	long line_len = 0;
	
	int type_num;
	
	fputs("// Global yglan edilen ulniler.\n", f);
	
	for(i=0; i<glob_def_vars_cmds; ++i)
	{
		if (strlen(glob_def_vars[i].file_name)==strlen(cur_parse_file_name) &&
			!strncmp(glob_def_vars[i].file_name, cur_parse_file_name, strlen(cur_parse_file_name)))
		{
			type_num = glob_def_vars[i].tok_type;
			
			char *line = malloc(line_size);
			// Ulninin tipi
			line_len = strlen(def_type_list[type_num].value)+100;
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

			strncat(line, " ", strlen(" "));
			
			// Ulninin ady goshulyar
			if (line_len+strlen(glob_def_vars[i].tok_name)>= line_size)
			{
				line_len += strlen(glob_def_vars[i].tok_name)+100;
				line = realloc(line, line_len);
			}
			strncat(line, glob_def_vars[i].tok_name, strlen(glob_def_vars[i].tok_name));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;
			
			free(line);
		}
	}
	
	fputs("\n\n", f);
}


// Esasy funksiyanyn bashy yazylyar
int prepare_main_func(FILE *f)
{
	fputs("int main() {\n", f);
	
	return 1;
}

// Esasy funksiyanyn sonyny taynlayar
int finishize_main_func(FILE *f)
{
	fputs("\treturn 1;\n", f);
	fputs("}\n\n", f);
	
	return 1;
}

// Header fayly sonlayar
int finishize_h_source(FILE *f)
{
	fputs("#endif \n", f);
	
	return 1;
}


// Lokal yglan edilen funksiyalar yglan edilyar
int c_trans_source_add_loc_def_var(FILE *f, char main_file)
{
	
	if (main_file)
	{
		fputs("\t// Dine main funksiyanyn ichinde ulanyp bolyan ulniler\n", f);
		
		int i, type_num;
		long line_size = 100, line_len = 0;
		
		for(i=0; i<loc_def_vars_num; ++i)
		{
			type_num = loc_def_vars[i].tok_type;
				
			char *line = malloc(line_size);
			strncpy(line, "\t", strlen("\t")+1);
			
			// Ulninin tipi goshulyar
			if (line_len+strlen(def_type_list[type_num].value)>= line_size)
			{
				line_len += strlen(def_type_list[type_num].value)+100;
				line = realloc(line, line_len);
			}
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

			strncat(line, " ", strlen(" "));
				
			// Ulninin ady goshulyar
			if (line_len+strlen(loc_def_vars[i].tok_name)>= line_size)
			{
				line_len += strlen(loc_def_vars[i].tok_name)+100;
				line = realloc(line, line_len);
			}
			strncat(line, loc_def_vars[i].tok_name, strlen(loc_def_vars[i].tok_name));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;
				
			free(line);
		}
		
		fputs("\n\n", f);
		
	}
	else
	{
		fputs("// Dine hazirki faylyn ichinde ulanylyp bolynyan ulniler\n", f);
		
		int i, type_num;
		long line_size = 100, line_len = 0;
		
		for(i=0; i<loc_def_vars_num; ++i)
		{
			type_num = loc_def_vars[i].tok_type;
				
			char *line = malloc(line_size);
			strncpy(line, "static ", strlen("static ")+1);
			
			// Ulninin tipi goshulyar
			if (line_len+strlen(def_type_list[type_num].value)>= line_size)
			{
				line_len += strlen(def_type_list[type_num].value)+100;
				line = realloc(line, line_len);
			}
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

			strncat(line, " ", strlen(" "));
				
			// Ulninin ady goshulyar
			if (line_len+strlen(loc_def_vars[i].tok_name)>= line_size)
			{
				line_len += strlen(loc_def_vars[i].tok_name)+100;
				line = realloc(line, line_len);
			}
			strncat(line, loc_def_vars[i].tok_name, strlen(loc_def_vars[i].tok_name));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;
				
			free(line);
		}
	}
	
	return 1;
}


// Lokal fayla degishli bolan ahli maglumatlary fayla terjime edyar
// @main_file - Lokal fayl bash faylmy?
int work_with_translator(char main_file)
{
	char f_name[MAX_FILE_LEN] = {0};// = cur_parse_file_name;
	strncpy(f_name, cur_parse_file_name, strlen(cur_parse_file_name)+1);

	remove_dirnames(f_name);
	remove_ext(f_name, ".tepl");

	// TODO. Eger faylyn ady fayllaryn sanawynda eyyam bar bolsa
	//		Onda yalnyshlyk chykarylyar.
	
	// .h faylyn boljak adresi
	char h_path[MAX_FILE_LEN] = {0};
	strncat(h_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(h_path, "\\", strlen("\\"));
	strncat(h_path, f_name, strlen(f_name));
	strncat(h_path, ".h", strlen(".h"));

	// .c faylyn boljak adresi
	char c_path[MAX_FILE_LEN] = {0};
	strncat(c_path, C_SOURCE_FOLDER, strlen(C_SOURCE_FOLDER));
	strncat(c_path, "\\", strlen("\\"));
	strncat(c_path, f_name, strlen(f_name));
	strncat(c_path, ".c", strlen(".c"));

	// Fayllar achylyar:
	FILE *c_source = fopen(c_path, "w");
	FILE *h_source = fopen(h_path, "w");
		
	// Faylyn ichine maglumat yazylyar:
	//printf("%s-%s-\n\n", cur_parse_file_name);
	//printf("%s\n", f_name);
	
	prepare_h_source(h_source, f_name);
	prepare_c_source(c_source, strncat(f_name, ".h", strlen(".h")));
	
	if (glob_def_vars_cmds && is_glob_def_var_in_cur())
	{
		//printf("Global ulni yglan edilipdir\n");
		c_trans_header_add_glob_def_var(h_source);
		c_trans_source_add_glob_def_var(c_source);
	}
	
	// Bash fayl bolsa, bash fayla degishli maglumatlar salynyar
	if (strlen(main_file_name)==strlen(cur_parse_file_name) &&
	    !strncmp(main_file_name, cur_parse_file_name, strlen(main_file_name)))
	{
		main_file = 1;
		// Bash sahypada esasy funksiya achylyar.
		prepare_main_func(c_source);
	}
	
	// Lokal yglan edilen funksiyalar
	c_trans_source_add_loc_def_var(c_source, main_file);
	
	if (main_file)
		finishize_main_func(c_source);
	
	finishize_h_source(h_source);
	
	// Fayllar yapylyar
	fclose(c_source);
	fclose(h_source);

}
