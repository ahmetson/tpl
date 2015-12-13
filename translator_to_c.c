/*
 * Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "fns.h"
#include "cmds.h"
#include "cmd/ctrl_statement.h"
#include "cmd/block.h"
#include "translator_to_c.h"

// file length - 255, command length - 95
#define MAX_PREP_LEN 350


/** ŞERT #7: Ýasalýan kodda: içki komandalaryň, enelerinden bir TAB saga süýşürilip tämiz görkezilmeli.

    1) Ýasalýan kodyň häzir haýsy blogyň içindeliginiň basgançagyny görkezýän global ülňi bolýar.
    Adatça ol 0-njy derejede bolýar.
    2) Funksiýa berlen basgançagyň sanynyň möçberinde '\t' harplary, berlen tekste goşýär.
    (write_tabs_c_code(FILE *f, char **l, int *llen, int tabs_num))

    3) Her kodly setiri ýasaljak koda ýazýan funksiýadan öň write_tabs_c_code() funksiýasy çagyrylýar.

    4) algoritmleri ýasaljak koda geçirýän funksiýa-da, eger komanda blok açýan bolsa (is_open_block_cmd(command *cmd))
        (Meselem Eger, Tä komandalary çagyrylanda)
            Içindeliginiň basgançagy bir san ulalýar.
    5) algoritmleri ýasaljak koda geçirýän funksiýa-da, eger komanda blok ýapýan bolsa (is_close_block_md(command *cmd))
            Içindeliginiň basgançagy bir san kiçelýär.
    6) Eger ýasalýan koda ýazýan funksiýa, C dilindäki blogy açýan bolsa, onda funksiýa çagyrylan soň
        (Meselem Main funksiýasy, ýa kodly faýllaryň baş funksiýalary çagyrylanda)
            Içindeliginiň basgançagy bir san ulalýar.
    7) Eger ýasalýan koda ýazýan funksiýa, C dilindäki blogy ýapýan bolsa, onda funksiýa çagyrylan soň
            Içindeliginiň basgançagy bir san kiçelýär.

*/

int TRANS_C_BLOCK_DEPTH = 0;

/** Kodly setiri ýazýar. Eger kodly setir blogyň içinde bolsa, onda öňünden TAB goýýar */
void write_code_line(FILE *f, char **l, int *llen, int tabs_num, char *code)
{
    /// Öňünden TAB goýýar.
    write_tabs_c_code(l, llen, tabs_num);
    if (!(*llen))
    {
        fputs(code, f);
    }
    else
    {
        *llen += strlen(code);
        *l = realloc(*l, *llen);
        strncat(*l, code, strlen(code));
        fputs(*l, f);

        free(*l);
        *l = NULL;
        *llen = 0;
    }
}

/** Berlen kodly ýazga, içindeliginiň basgançagyna görä, '\t' goýýar */
void write_tabs_c_code(char **l, int *llen, int tabs_num)
{
    if (!tabs_num)
        return;
    char *str = malloc(sizeof(char)*(tabs_num+1));
    strncpy(str, "\t", strlen("\t")+1);
    int i;
    for (i=1; i<tabs_num; ++i)
    {
        strncat(str, "\t", strlen("\t"));
    }

    // Ýasaljak faýla ýazmaly
    if (!(*llen))
    {
        *llen = strlen(str)+1;
        *l = realloc(*l, *llen);
        strncpy(*l, str, strlen(str)+1);
    }
    else
    {
        *llen += strlen(str);
        *l = realloc(*l, *llen);
        strncat(*l, str, strlen(str));
    }
    free(str);
}

// C kod nirede saklanyar?
char *C_SOURCE_FOLDER = "c:\\\\c_by_tpl";

// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, char *f_name)
{
    char *fline = "//Created by TPL.\n";
    char *sline = "//TPL author: Medet Atabayev.\n";
    char *line3 = malloc(MAX_PREP_LEN);
        strncpy(line3, "#ifndef ", strlen("#ifndef ")+1);
        strncat(line3, f_name, strlen(f_name));
        strncat(line3, "_H \n", strlen("_H \n"));
    char *line4 = malloc(MAX_PREP_LEN);
        strncpy(line4, "#define ", strlen("#define ")+1);
        strncat(line4, f_name, strlen(f_name));
        strncat(line4, "_H \n", strlen("_H \n"));


    char *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, sline);
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line3);
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line4);

	free(line3);
	free(line4);

	fputs("\n\n", f);

	return 1;
}


int prepare_c_source(FILE *f, char *h_source)
{
    char *fline = "//Created by TPL.\n";
    char *sline = "//TPL author: Medet Atabayev.\n";
    char *line3 = malloc(MAX_PREP_LEN);
        strncpy(line3, "#include \"", strlen("#include \"")+1);
        strncat(line3, h_source, strlen(h_source));
        strncat(line3, "\" \n", strlen("\" \n"));

    char *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, sline);
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line3);

    free(line3);

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

	char *fline = "// Global yglan edilen ulniler.\n";

    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);

	for(i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
	{
		if (GLOBAL_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOBAL_VAR_DEFS[i].type_num;

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
			if (line_len+strlen(GLOBAL_VAR_DEFS[i].name)>= line_size)
			{
				line_len += strlen(GLOBAL_VAR_DEFS[i].name)+100;
				line = realloc(line, line_len);
			}
			strncat(line, GLOBAL_VAR_DEFS[i].name, strlen(GLOBAL_VAR_DEFS[i].name));
			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			//printf("%s global ulna goshulyar\n", line);
			line_len = 100;

			free(line);
		}
	}

	fputs("\n\n", f);

	return 1;
}

int c_trans_header_add_glob_def_arr(FILE *f)
{
	int i;
	int line_len = 0;

	int type_num;

	char *fline = "// Global yglan edilen birsyhly sanawlar.\n";

    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);

	for(i=0; i<GLOBAL_ARR_DEFS_NUMS; ++i)
	{
		if (GLOBAL_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOBAL_ARR_DEFS[i].type_num;

            line_len = strlen("extern ")+1;

			char *line = malloc(line_len);
			strncpy(line, "extern ", line_len);

			// Ulninin tipi goshulyar
			line_len += strlen(def_type_list[type_num].value);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value));

            line_len += strlen(" ");
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(GLOBAL_ARR_DEFS[i].ident);
            line = realloc(line, line_len);
			strncat(line, GLOBAL_ARR_DEFS[i].ident, strlen(GLOBAL_ARR_DEFS[i].ident));


			/** Elementler hakda maglumat goşulýar */
            add_arr_elem_inf_c_code(&line, &line_len, 0, i, GLOBAL_ARR_DEFS[i].incs);

			// Komanda gutardy
			line_len += strlen("; \n");
            line = realloc(line, line_len);
			strncat(line, "; \n", strlen("; \n"));

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			//printf("%s global ulna goshulyar\n", line);
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

	char *fline = "// Global yglan edilen ulniler.\n";

	char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);

	for(i=0; i<GLOBAL_VAR_DEFS_NUMS; ++i)
	{
		if (GLOBAL_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOBAL_VAR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);

			// Ulninin tipi
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ")+1;
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
            line_len += strlen(GLOBAL_VAR_DEFS[i].name)+1;
            line = realloc(line, line_len);
			strncat(line, GLOBAL_VAR_DEFS[i].name, strlen(GLOBAL_VAR_DEFS[i].name));

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

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			line_len = 100;

			free(line);
		}
	}

	fputs("\n\n", f);

	return 1;
}

int c_trans_source_add_glob_def_arr(FILE *f)
{
	int i, line_len;

	int type_num;

	char *fline = "// Global yglan edilen birsyhly sanawlar.\n";

	char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, fline);

	for(i=0; i<GLOBAL_ARR_DEFS_NUMS; ++i)
	{
		if (GLOBAL_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOBAL_ARR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);

			// Ulninin tipi
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ");
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
            line_len += strlen(GLOBAL_ARR_DEFS[i].ident);
            line = realloc(line, line_len);
			strncat(line, GLOBAL_ARR_DEFS[i].ident, strlen(GLOBAL_ARR_DEFS[i].ident));

            // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 0, i, GLOBAL_ARR_DEFS[i].incs);

			// Ülňilere başlangyç maglumatlar baglamaly
            line_len += strlen(" = {");
            line = realloc(line, line_len);
			strncat(line, " = {", strlen(" = {"));

            // Ülňileriň başlangyç maglumatlary
            line_len += strlen(def_type_list[type_num].init_val);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].init_val, strlen(def_type_list[type_num].init_val));


			// Komanda gutardy
			line_len += strlen("}; \n");
            line = realloc(line, line_len);
			strncat(line, "}; \n", strlen("}; \n"));

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			free(line);
		}
	}

	fputs("\n\n", f);

	return 1;
}


// Esasy funksiyanyn bashy yazylyar
int prepare_main_func(FILE *f)
{
	char *line = "int main() {\n";
    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

    /// #7.6
    ++TRANS_C_BLOCK_DEPTH;

	return 1;
}

// Esasy funksiyanyn sonyny taynlayar
int finishize_main_func(FILE *f)
{
    char *line1 = "//, bashga programmanyn bash bolegi gutardy \n\n\n\treturn 1;\n";
    char *line2 = "}\n\n";
    char *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

    /// #7.7
    --TRANS_C_BLOCK_DEPTH;

    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line2);

	return 1;
}


// Esasy funksiyanyn sonyny taynlayar
int finishize_void_func(FILE *f)
{
    char *line1 = "}\n\n";
    char *l = NULL;
    int llen = 0;

    /// #7.7
    --TRANS_C_BLOCK_DEPTH;

    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

	return 1;
}

// Header fayly sonlayar
int finishize_h_source(FILE *f)
{
    char *line1 = "#endif \n";
    char *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

	return 1;
}


// Lokal yglan edilen funksiyalar yglan edilyar
int c_trans_source_add_loc_def_var(FILE *f, char main_file)
{

    char *l = NULL;
    int llen = 0;

	if (main_file)
	{
        char *line1 = "// Dine main funksiyanyn ichinde ulanyp bolyan ulniler\n";

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

		int i, type_num;
		long line_len;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
			type_num = LOCAL_VAR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ");
			line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(LOCAL_VAR_DEFS[i].name);
            line = realloc(line, line_len);
			strncat(line, LOCAL_VAR_DEFS[i].name, strlen(LOCAL_VAR_DEFS[i].name));

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

			write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			line_len = 100;

			free(line);
		}

		fputs("\n\n", f);

	}
	else
	{
	    char *line1 = "// Dine hazirki faylyn ichinde ulanylyp bolynyan ulniler\n";

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

		int i, type_num, len;
		long line_len;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
            if (LOCAL_VAR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
                continue;
			type_num = LOCAL_VAR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ");
            line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(LOCAL_VAR_DEFS[i].name);
            line = realloc(line, line_len);
			strncat(line, LOCAL_VAR_DEFS[i].name, strlen(LOCAL_VAR_DEFS[i].name));

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
			write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			fputs("\n\n", f);

			free(line);
		}
	}
	return 1;
}


// Lokal yglan edilen funksiyalar yglan edilyar
int c_trans_source_add_loc_def_arr(FILE *f, char main_file)
{

    char *l = NULL;
    int llen = 0;

	if (main_file)
	{
        char *line1 = "// Dine main funksiyanyn ichinde ulanyp bolyan sanawlar\n";

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

		int i, type_num, line_len;

        for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
			type_num = LOCAL_ARR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ");
			line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(LOCAL_ARR_DEFS[i].ident);
            line = realloc(line, line_len);
			strncat(line, LOCAL_ARR_DEFS[i].ident, strlen(LOCAL_ARR_DEFS[i].ident));

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

            // Ülňilere başlangyç maglumatlar baglamaly
            line_len += strlen(" = {");
            line = realloc(line, line_len);
			strncat(line, " = {", strlen(" = {"));

            // Ülňileriň başlangyç maglumatlary
            line_len += strlen(def_type_list[type_num].init_val);
            line = realloc(line, line_len);
			strncat(line, def_type_list[type_num].init_val, strlen(def_type_list[type_num].init_val));

			// Komanda gutardy
			line_len += strlen("}; \n");
            line = realloc(line, line_len);
			strncat(line, "}; \n", strlen("}; \n"));

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			//printf("%s global ulna goshulyar\n", line);
			line_len = 0;

			free(line);
		}

		fputs("\n\n", f);

	}
	else
	{
	    char *line1 = "// Dine hazirki faylyn ichinde ulanylyp bolynyan ulniler\n";

        write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line1);

		int i, type_num, len, line_len;


		for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
            if (LOCAL_ARR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
                continue;
			type_num = LOCAL_ARR_DEFS[i].type_num;

            line_len = strlen(def_type_list[type_num].value)+1;
			char *line = malloc(line_len);
			strncpy(line, def_type_list[type_num].value, strlen(def_type_list[type_num].value)+1);

            line_len += strlen(" ");
			line = realloc(line, line_len);
			strncat(line, " ", strlen(" "));

			// Ulninin ady goshulyar
			line_len += strlen(LOCAL_ARR_DEFS[i].ident);
            line = realloc(line, line_len);
			strncat(line, LOCAL_ARR_DEFS[i].ident, strlen(LOCAL_ARR_DEFS[i].ident));

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

			// Komanda gutardy
			strncat(line, "; \n", strlen("; \n"));

            write_code_line(f, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

			//printf("%s global ulna goshulyar\n", line);
			line_len = 0;

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
		c_trans_header_add_glob_def_var(h_source);
		c_trans_source_add_glob_def_var(c_source);
	}
	if (is_glob_def_arr_in_cur())
    {
        c_trans_header_add_glob_def_arr(h_source);
		c_trans_source_add_glob_def_arr(c_source);
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

        MAIN_FILE_INCLUDES_NUM++;
        MAIN_FILE_INCLUDES = realloc(MAIN_FILE_INCLUDES, MAIN_FILE_INCLUDES_NUM*(sizeof(*MAIN_FILE_INCLUDES)));

        // Baş faýla ýazmaly funksiýanyň prototipi yglan edilen faýl goşulýar
        strncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][0], f_name, strlen(f_name)+1);

        // Faýlyň adyndan funksiýanyň ady ýazylýar.
        remove_ext(f_name, ".h");
       // printf("Faýlyň ady:%s\t", f_name);

        // Baş funksiýanyň prototipi
        c_trans_write_file_fn_prototype(h_source, f_name);
        // Baş funksiýanyň açylýan ýeri, kodly faýlda ýazylýar
        c_trans_write_file_fn_open(c_source, f_name);

        // Baş faýla ýazmaly funksiýanyň prototipi.

        char fn_prototype[MAX_FILE_LEN];
        c_trans_get_file_fn_prototype(f_name, fn_prototype);
        strncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][1], fn_prototype, strlen(fn_prototype)+1);
        //printf("Funksiyanyn ady:%s\n", fn_prototype);
    }

	// Lokal yglan edilen funksiyalar
	c_trans_source_add_loc_def_var(c_source, main_file);
	c_trans_source_add_loc_def_arr(c_source, main_file);

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
    char *l = NULL;
    int llen = 0;

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

    /// Ýasaljak koda ýazylýar
    write_code_line(f_h, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

    free(line);

    return 1;
}


int c_trans_write_file_fn_open(FILE *f_h, char *fn_name)
{
    char *l = NULL;
    int llen = 0;

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

    /// Ýasaljak koda ýazylýar
    write_code_line(f_h, &l, &llen, TRANS_C_BLOCK_DEPTH, line);

    /// #7.6
    ++TRANS_C_BLOCK_DEPTH;

    free(line);

    return 1;
}


// Faýlyň içine algoritmi goşýar.
int c_trans_source_add_algor(FILE *f, int main_file)
{
    int i, len=0; char *l = NULL;
    char *putl = NULL;
    int putllen = 0;

    for (i=0; i<CUR_ALGOR_ITEMS_NUM; ++i)
    {

        CMD_GET_C_CODE[CUR_ALGOR[i].cmd_class][CUR_ALGOR[i].cmd_type](&CUR_ALGOR[i], &l, &len);
        if (!is_open_block_cmd(&CUR_ALGOR[i]) && !is_close_block_cmd(&CUR_ALGOR[i]))
            cmd_wrapper_c_code(&l, &len);
        else
            cmd_block_wrapper_c_code(&l, &len);
        //debug_cmd(&CUR_ALGOR[i]);
        //printf("%s\n", l);

        /// #7.5)
        if (is_close_block_cmd(&CUR_ALGOR[i]))
            --TRANS_C_BLOCK_DEPTH;

        write_code_line(f, &putl, &putllen, TRANS_C_BLOCK_DEPTH, l);

        /// #7.4)
        if (is_open_block_cmd(&CUR_ALGOR[i]))
            ++TRANS_C_BLOCK_DEPTH;

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


int is_open_block_cmd(command *cmd)
{
    if (cmd->is_compl)
    {
        if (cmd->cmd_class==CMD_CLASS_CTRL_STTMNT || cmd->cmd_class==CMD_CLASS_LOOP_STTMNT)
            return 1;
    }
    return 0;
}

int is_close_block_cmd(command *cmd)
{
    if (cmd->is_compl)
    {
        if ((cmd->cmd_class==CMD_CLASS_CTRL_STTMNT && cmd->cmd_type!=CMD_CLASS_CTRL_STTMNT_IF) ||
            (cmd->cmd_class==CMD_CLASS_BLOCK && cmd->cmd_type==CMD_CLASS_BLOCK_CLOSE))
            return 1;
    }
    return 0;
}



