/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "fns.h"
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

    fputs("//.\n", f);
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

	for(i=0; i<USER_VAR_DEFS_NUM; ++i)
	{
	    if (USER_VAR_DEFS[i].ns!=GLOB)
            continue;
		if (strlen(USER_VAR_DEFS[i].file_name)==strlen(CUR_FILE_NAME) &&
			!strncmp(USER_VAR_DEFS[i].file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)))
		{
			type_num = USER_VAR_DEFS[i].tok_type;

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
			if (line_len+strlen(USER_VAR_DEFS[i].ident)>= line_size)
			{
				line_len += strlen(USER_VAR_DEFS[i].ident)+100;
				line = realloc(line, line_len);
			}
			strncat(line, USER_VAR_DEFS[i].ident, strlen(USER_VAR_DEFS[i].ident));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;

			free(line);
		}
	}

	fputs("\n\n", f);

	return 1;
}


int c_trans_source_add_glob_def_var(FILE *f)
{
	int i;
	long line_len;

	int type_num;

	fputs("// Global yglan edilen ulniler.\n", f);

	for(i=0; i<USER_VAR_DEFS_NUM; ++i)
	{
	    if (USER_VAR_DEFS[i].ns!=GLOB)
            continue;
		if (strlen(USER_VAR_DEFS[i].file_name)==strlen(CUR_FILE_NAME) &&
			!strncmp(USER_VAR_DEFS[i].file_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)))
		{
			type_num = USER_VAR_DEFS[i].tok_type;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);

			// Ulninin tipi
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ")+1;
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
            line_len += strlen(USER_VAR_DEFS[i].ident)+1;
            line = realloc(line, line_len);
			strncat(line, USER_VAR_DEFS[i].ident, strlen(USER_VAR_DEFS[i].ident));

			// Ülňilere başlangyç maglumatlar baglamaly
            line_len += strlen(" = ")+1;
            line = realloc(line, line_len);
			strncat(line, " = ", strlen(" = "));

            // Ülňileriň başlangyç maglumatlary
            line_len += strlen(def_type_list[type_num].init_val)+1;
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].init_val, strlen(def_type_list[type_num].init_val));


			// Komanda gutardy
			line_len += strlen("; \n")+1;
            line = realloc(line, line_len);
			strncat(line, "; \n", strlen("; \n"));

			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			line_len = 100;

			free(line);
		}
	}

	fputs("\n\n", f);

	return 1;
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
	fputs("\t//, bashga programmanyn bash bolegi gutardy \n\n\n\treturn 1;\n", f);
	fputs("}\n\n", f);

	return 1;
}


// Esasy funksiyanyn sonyny taynlayar
int finishize_void_func(FILE *f)
{
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
		long line_len;

		for(i=0; i<USER_VAR_DEFS_NUM; ++i)
		{
		    if (USER_VAR_DEFS[i].ns!=LOCAL)
                continue;
			type_num = USER_VAR_DEFS[i].tok_type;

            line_len = strlen("\t")+1;
			char *line = malloc(line_len);
			strncpy(line, "\t", strlen("\t")+1);

			// Ulninin tipi goshulyar
			line_len += strlen(def_type_list[type_num].value)+1;
			line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

            line_len += strlen(" ")+1;
			line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(USER_VAR_DEFS[i].ident)+1;
            line = realloc(line, line_len);
			strncat(line, USER_VAR_DEFS[i].ident, strlen(USER_VAR_DEFS[i].ident));

            // Ülňilere başlangyç maglumatlar baglamaly
            line_len += strlen(" = ");
            line = realloc(line, line_len);
			strncat(line, " = ", strlen(" = "));

            // Ülňileriň başlangyç maglumatlary
            line_len += strlen(def_type_list[type_num].init_val);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].init_val, strlen(def_type_list[type_num].init_val));

			// Komanda gutardy
			line_len += strlen("; \n")+1;
            line = realloc(line, line_len);
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
		fputs("\t// Dine hazirki faylyn ichinde ulanylyp bolynyan ulniler\n", f);

		int i, type_num, len;
		long line_len;

		for(i=0; i<USER_VAR_DEFS_NUM; ++i)
		{


            len = strlen(USER_VAR_DEFS[i].file_name)>strlen(CUR_FILE_NAME)?strlen(USER_VAR_DEFS[i].file_name):strlen(CUR_FILE_NAME);
            if (strncmp(USER_VAR_DEFS[i].file_name, CUR_FILE_NAME, len)!=0)
                continue;
			type_num = USER_VAR_DEFS[i].tok_type;

            line_len = strlen("\t")+1;
			char *line = malloc(line_len);
			strncpy(line, "\t", strlen("\t")+1);

			// Ulninin tipi goshulyar
			line_len += strlen(def_type_list[type_num].value);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

            line_len += strlen(" ");
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(USER_VAR_DEFS[i].ident);
            line = realloc(line, line_len);
			strncat(line, USER_VAR_DEFS[i].ident, strlen(USER_VAR_DEFS[i].ident));

            // Ülňilere başlangyç maglumatlar baglamaly
            line_len += strlen(" = ");
            line = realloc(line, line_len);
			strncat(line, " = ", strlen(" = "));

            // Ülňileriň başlangyç maglumatlary
            line_len += strlen(def_type_list[type_num].init_val);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].init_val, strlen(def_type_list[type_num].init_val));

			// Komanda gutardy
			line_len += strlen("; \n");
            line = realloc(line, line_len);
			strncat(line, "; \n", strlen("; \n"));
			//printf("%s global ulna goshulyar\n", line);
			fputs(line, f);
			fputs("\n\n", f);

			free(line);
		}
	}
	return 1;
}


// Lokal fayla degishli bolan ahli maglumatlary fayla terjime edyar
// @main_file - Lokal fayl bash faylmy?
int work_with_translator(char main_file)
{
	char f_name[MAX_FILE_LEN] = {0};// = CUR_FILE_NAME;
	strncpy(f_name, CUR_FILE_NAME, strlen(CUR_FILE_NAME)+1);

	remove_dirnames(f_name);
	remove_ext(f_name, ".tepl");
    add_to_file_list_name(f_name);

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

	add_to_file_list_h_source(h_path);
	add_to_file_list_c_source(c_path);

	// Faylyn ichine maglumat yazylyar:
	//printf("%s-%s-\n\n", CUR_FILE_NAME);
	//printf("%s\n", f_name);

    // Ýasaljak faýllaryň başy
	prepare_h_source(h_source, f_name);
	prepare_c_source(c_source, strncat(f_name, ".h", strlen(".h")));

	if (is_glob_def_var_in_cur())
	{
		//printf("Global ulni yglan edilipdir: %s\n", CUR_FILE_NAME);
		c_trans_header_add_glob_def_var(h_source);
		c_trans_source_add_glob_def_var(c_source);
	}

	// Baş faýl bolsa, ýasalmaly sahypa main funksiýa ýazylmaly
	if (strlen(MAIN_FILE_NAME)==strlen(CUR_FILE_NAME) &&
	    !strncmp(MAIN_FILE_NAME, CUR_FILE_NAME, strlen(MAIN_FILE_NAME)))
	{
		main_file = 1;

		// Bash sahypada esasy funksiya achylyar.
		prepare_main_func(c_source);
	}
	else
    {
        main_file = 0;

        // Baş faýla ýazmaly funksiýanyň prototipi yglan edilen faýl goşulýar
        strncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][0], f_name, strlen(f_name)+1);

        // Faýlyň adyndan funksiýanyň ady ýazylýar.
        remove_ext(f_name, ".h");
        //printf("Faýlyň ady:%s\n", f_name);

        // Baş funksiýanyň prototipi
        c_trans_write_file_fn_prototype(h_source, f_name);
        // Baş funksiýanyň açylýan ýeri, kodly faýlda ýazylýar
        c_trans_write_file_fn_open(c_source, f_name);

        // Baş faýla ýazmaly funksiýanyň prototipi.
        MAIN_FILE_INCLUDES_NUM++;
        MAIN_FILE_INCLUDES = realloc(MAIN_FILE_INCLUDES, MAIN_FILE_INCLUDES_NUM*(sizeof(*MAIN_FILE_INCLUDES)));
        char fn_prototype[MAX_FILE_LEN];
        c_trans_get_file_fn_prototype(f_name, fn_prototype);
        strncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][1], fn_prototype, strlen(fn_prototype)+1);
    }

	// Lokal yglan edilen funksiyalar
	c_trans_source_add_loc_def_var(c_source, main_file);

    // Algoritm goshulmaly
    c_trans_source_add_algor(c_source, main_file);

    // Funksiýany soylaýar
    if (main_file)
        finishize_main_func(c_source);
    else
        finishize_void_func(c_source);

	finishize_h_source(h_source);

	// Fayllar yapylyar
	fclose(c_source);
	fclose(h_source);

	return 1;
}


/* Baş däl faýllaryň içinde ulanylýan funksiýanyň, headerdäki prototipini ýazýar
 *
 * @f_h - header faýl
 * @f_h - funksiýanyň ady
**/
int c_trans_write_file_fn_prototype(FILE *f_h, char *fn_name)
{
    // void _fn_name_tepl();
    long size = 32;
    char *line = malloc(size);

    // bashy
    char *start = "void _";
    strncpy(line, start, strlen(start)+1);

    // ortak
    if (strlen(line)+strlen(fn_name)>size)
    {
        size += strlen(fn_name)+1;
    }
    strncat(line, fn_name, strlen(fn_name));

    // sony
    char *end = "_tepl(); \n";
    if (strlen(line)+strlen(end)>size)
    {
        size += strlen(end)+1;
    }
    strncat(line, end, strlen(end));

    fputs(line, f_h);

    free(line);

    return 1;
}


int c_trans_write_file_fn_open(FILE *f_h, char *fn_name)
{
    // void _fn_name_tepl();
    long size = 32;
    char *line = malloc(size);

    // bashy
    char *start = "void _";
    strncpy(line, start, strlen(start)+1);

    // ortak
    if (strlen(line)+strlen(fn_name)>size)
    {
        size += strlen(fn_name)+1;
    }
    strncat(line, fn_name, strlen(fn_name));

    // sony
    char *end = "_tepl(){ \n";
    if (strlen(line)+strlen(end)>size)
    {
        size += strlen(end)+1;
    }
    strncat(line, end, strlen(end));

    fputs(line, f_h);

    free(line);

    return 1;
}


// Faýlyň içine algoritmi goşýar.
int c_trans_source_add_algor(FILE *f, int main_file)
{
    int i, len=0; char *l = NULL;

    for (i=0; i<CUR_ALGOR_ITEMS_NUM; ++i)
    {
        CMD_GET_C_CODE[CUR_ALGOR[i].cmd_class][CUR_ALGOR[i].cmd_type](&CUR_ALGOR[i], &l, &len);
        //debug_cmd(&CUR_ALGOR[i]);
        //printf("%s\n", l);
        fputs(l, f);
        if (l!=NULL)
        {
            free(l);
            l = NULL;
            len = 0;
        }
    }
    fputs("\n\n", f);

    return 1;
}


// Faýlyň funksiýasynyň prototipini gaýtarýar
char *c_trans_get_file_fn_prototype(char *fn_name, char *line)
{
    char *start = "_";

    strncpy(line, start, strlen(start)+1);

    // ortasy
    strncat(line, fn_name, strlen(fn_name));
    //

    // sony
    char *end = "_tepl();";
    strncat(line, end, strlen(end));

    return line;
}
