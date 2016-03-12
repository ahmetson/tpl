/** Translyator, yagny C kodyny yasamak bilen ishleyan funksiyalar */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tpl.h"
#include "fns.h"
#include "algor.h"
#include "cmds.h"
#include "cmd/ctrl_statement.h"
#include "cmd/block.h"
#include "cmd/fn_def.h"
#include "cmd/def_var.h"
#include "translator_to_c.h"
#include "translator_to_c/includes.h"
#include "main/user_def_type.h"
#include "main/conv_basic_types.h"
#include "token/helpers.h"
#include "fns/fn_helpers.h"
#include "fns/3rdparty/std/kabul_et.h"
// C kod nirede saklanyar?
wchar_t *C_SOURCE_FOLDER = L"c:\\\\c_by_tpl";

/** ŞERT #7: Ýasalýan kodda: içki komandalaryň, enelerinden bir TAB saga süýşürilip tämiz görkezilmeli.

    1) Ýasalýan kodyň häzir haýsy blogyň içindeliginiň basgançagyny görkezýän global ülňi bolýar.
    Adatça ol 0-njy derejede bolýar.

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
            Içindeliginiň basgançagy bir san kiçelýär.*/

int TRANS_C_BLOCK_DEPTH = 0;

/** Kodly setiri ýazýar. Eger kodly setir blogyň içinde bolsa, onda öňünden TAB goýýar */
void write_code_line(FILE *f, wchar_t **l, int *llen, wchar_t *code)
{
    wcsadd_on_heap(l, llen, code);
    fputws(*l, f);

    free(*l);
    *l = NULL;
    *llen = 0;
}


// Header fayly ulanmaga tayynlayar
int prepare_h_source(FILE *f, wchar_t *f_name)
{
    wchar_t *ifndef = L"#ifndef ",
            *no_dot_h = L"_H \n",
            *define = L"#define ";

    wchar_t *line3 = malloc( MAX_PREP_LEN );
        wcsncpy( line3, ifndef, wcslen( ifndef )+1 );
        wcsncat( line3, f_name, wcslen( f_name ) );
        wcsncat( line3, no_dot_h, wcslen( no_dot_h ) );
    wchar_t *line4 = malloc(MAX_PREP_LEN);
        wcsncpy( line4, define, wcslen( define )+1 );
        wcsncat( line4, f_name, wcslen( f_name ) );
        wcsncat( line4, no_dot_h, wcslen( no_dot_h ) );


    wchar_t *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen , line3);
    write_code_line(f, &l, &llen , line4);

	free(line3);
	free(line4);

    add_addtn_headers(f);

	fputws(L"\n\n", f);

	return 1;
}
int prepare_c_source(FILE *f, wchar_t *h_source)
{
    wchar_t *include = L"#include \"",
            *dquote_endl = L"\" \n";
    wchar_t *line3 = malloc( MAX_PREP_LEN );
        wcsncpy( line3, include, wcslen( include )+1 );
        wcsncat( line3, h_source, wcslen( h_source) );
        wcsncat( line3, dquote_endl, wcslen( dquote_endl ) );

    wchar_t *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen , line3);

    add_addtn_headers(f);

    free(line3);

    fputws(L"//.\n", f);
	fputws(L"\n\n", f);

	return 1;
}
int c_trans_header_add_glob_def_var(FILE *f)
{
	int i, line_len = 0;

	int type_num;

    wchar_t *l = NULL,
            *extern_str = L"extern ",
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
	{
		if (GLOB_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
            line_len = 0;
			wchar_t *line = NULL;
            wcsadd_on_heap( &line, &line_len, extern_str );

			// Ulninin tipi goshulyar
			get_type_c_code(GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, GLOB_VAR_DEFS[i].name );

            get_cmd_end_c_code(&line, &line_len);

            write_code_line(f, &l, &llen , line);

			free(line);
		}
	}

	fputws(L"\n\n", f);

	return 1;
}
int c_trans_header_add_glob_def_arr(FILE *f)
{
	int i;
	int line_len = 0;

	int type_num;


    wchar_t *l = NULL,
            *extern_str = L"extern ",
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
	{
		if (GLOB_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOB_ARR_DEFS[i].type_num;

            line_len = 0;
			wchar_t *line = NULL;
			wcsadd_on_heap( &line, &line_len, extern_str );

			// Ulninin tipi goshulyar
            get_type_c_code(GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, GLOB_ARR_DEFS[i].ident );

			/** Elementler hakda maglumat goşulýar */
            add_arr_elem_inf_c_code( &line, &line_len, 0, i, GLOB_ARR_DEFS[i].incs);

			// Komanda gutardy
            get_cmd_end_c_code( &line, &line_len );

            write_code_line( f, &l, &llen , line );

			free( line );
		}
	}

	fputws(L"\n\n", f);

	return 1;
}
int c_trans_source_add_glob_def_var(FILE *f)
{
	int i, line_len;

	int type_num;

	wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
	{
		if (GLOB_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			type_num = GLOB_VAR_DEFS[i].type_num;

            line_len = 0;
			wchar_t *line = NULL;

			// Ulninin tipi
			get_type_c_code(GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
            wcsadd_on_heap( &line, &line_len, GLOB_VAR_DEFS[i].name );

			// Ülňilere başlangyç maglumatlar baglamaly
            get_type_init_val_c_code(0, GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            // Ülňileriň başlangyç maglumatlary
            //wcsadd_on_heap( &line, &line_len, def_type_list[type_num].init_val );

			// Komanda gutardy
            get_cmd_end_c_code(&line, &line_len);

            write_code_line(f, &l, &llen , line);

			line_len = 100;

			free(line);
		}
	}

	return 1;
}
int c_trans_source_add_glob_def_arr(FILE *f)
{
	int i, line_len;

	wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
	{
		if (GLOB_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
            line_len = 0;
			wchar_t *line = NULL;

			// Ulninin tipi
            get_type_c_code(GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
            wcsadd_on_heap( &line, &line_len, GLOB_ARR_DEFS[i].ident );

            // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 0, i, GLOB_ARR_DEFS[i].incs);

			// Ülňilere başlangyç maglumatlar baglamaly
            get_type_init_val_c_code(1, GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            // Komandany soňlaýan bölüm
            get_cmd_end_c_code(&line, &line_len);

            write_code_line(f, &l, &llen , line);

			free(line);
		}
	}

	fputws(L"\n\n", f);

	return 1;
}
// Esasy funksiyanyn bashy yazylyar
int prepare_main_func(FILE *f)
{
    add_conv_basic_prepere_atexit(f);
    add_turkmen_locale_support_lib(f);

	wchar_t *line = L"int main() {\n\tchar *tk = setlocale(LC_ALL, \".1250\");\n\
\tsystem(\"chcp 65001 > nul\");\n\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen , line);

	return 1;
}
// Esasy funksiyanyn sonyny taynlayar
int finishize_main_func(FILE *f)
{
    add_conv_basic_free_atexit(f);
    wchar_t *line1 = L"\n\n\treturn 1;\n";
    wchar_t *line2 = L"}\n\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_code_line(f, &l, &llen , line1);

    write_code_line(f, &l, &llen , line2);

	return 1;
}
// Esasy funksiyanyn sonyny taynlayar
int finishize_void_func(FILE *f)
{
    wchar_t *line1 = L"}\n\n";
    wchar_t *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen , line1);

	return 1;
}
// Header fayly sonlayar
int finishize_h_source(FILE *f)
{
    wchar_t *line1 = L"#endif \n";
    wchar_t *l = NULL;
    int llen = 0;

    write_code_line(f, &l, &llen , line1);

	return 1;
}
// Lokal yglan edilen funksiyalar yglan edilyar
int c_trans_source_add_loc_def_var(FILE *f, wchar_t main_file)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (main_file)
	{
		int i, type_num;
		long line_len;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
            int  line_len = 0;
			wchar_t *line = NULL;
			get_type_c_code(LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);


            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_VAR_DEFS[i].name );

            // Ülňiniň başlangyç maglumatlary goýulýar.
            get_type_init_val_c_code(0, LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

			// Komanda gutardy
			get_cmd_end_c_code( &line, &line_len );

			write_code_line(f, &l, &llen , line);

			free(line);
		}
	}
	else
	{
		int i, type_num, len;
		long line_len;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
            if (LOCAL_VAR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
                continue;


            int line_len = 0;
			wchar_t *line = malloc(line_len);

			get_type_c_code(LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_VAR_DEFS[i].name );

            // Ülňilere başlangyç maglumatlar baglamaly
            get_type_init_val_c_code(0, LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

			// Komanda gutardy
			get_cmd_end_c_code(&line, &line_len);

			//printf(L"%s global ulna goshulyar\n", line);
			write_code_line(f, &l, &llen , line);

			free(line);
		}
	}
	return 1;
}
// Lokal yglan edilen funksiyalar yglan edilyar
int c_trans_source_add_loc_def_arr(FILE *f, wchar_t main_file)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (main_file)
	{
		int i, type_num, line_len;

        for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
			type_num = LOCAL_ARR_DEFS[i].type_num;

            int line_len = 0;
			wchar_t *line = NULL;
			get_type_c_code(LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_ARR_DEFS[i].ident );

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

            // Ülňilere başlangyç maglumatlar baglamaly
            get_type_init_val_c_code(1, LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            // Komanda gutardy
			get_cmd_end_c_code(&line, &line_len);

            write_code_line(f, &l, &llen , line);

			//printf(L"%s global ulna goshulyar\n", line);
			free(line);
		}

		fputws(L"\n\n", f);

	}
	else
	{
		int i, type_num, len, line_len;

		for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
            if (LOCAL_ARR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
                continue;

			int line_len = 0;
			wchar_t *line = NULL;
			get_type_c_code(LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_ARR_DEFS[i].ident );

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

            // Başlangyç maglumatlar goşulýar
            get_type_init_val_c_code(1, LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            // Komanda gutardy
			get_cmd_end_c_code(&line, &line_len);


            write_code_line(f, &l, &llen, line);

			//printf(L"%s global ulna goshulyar\n", line);
			line_len = 0;

			free(line);

		}
	}
	return 1;
}
// Lokal fayla degishli bolan ahli maglumatlary fayla terjime edyar
// @main_file - Lokal fayl bash faylmy?
int work_with_translator()
{
    wchar_t *dot_h = L".h",
            main_file;
    file_item *fi = &FILES[ CUR_FILE_NUM-1 ];


    wchar_t f_name[ MAX_FILE_LEN ] = {0};
    wcsncpys( f_name, CUR_FILE_NAME );

	// Fayllar achylyar:
	FILE *c_source = _wfopen(fi->c_source, L"w, ccs=UTF-8");
	FILE *h_source = _wfopen(fi->h_source, L"w, ccs=UTF-8");

    // Ýasaljak faýllaryň başy
	prepare_h_source(h_source, fi->name);
	prepare_c_source(c_source, wcsncat( fi->name, dot_h, wcslen( dot_h ) ) );


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
	if ( is_wcseq( MAIN_FILE_NAME, CUR_FILE_NAME ) )
	{
		main_file = 1;
		// Bash sahypada esasy funksiya achylyar.
		prepare_main_func( c_source );
	}
	else
    {
        main_file = 0;

        MAIN_FILE_INCLUDES_NUM++;
        MAIN_FILE_INCLUDES = realloc(MAIN_FILE_INCLUDES, MAIN_FILE_INCLUDES_NUM*(sizeof(*MAIN_FILE_INCLUDES)));

        // Baş faýla ýazmaly funksiýanyň prototipi yglan edilen faýl goşulýar
        wcsncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][0], fi->name, wcslen(fi->name)+1);

        // Faýlyň adyndan funksiýanyň ady ýazylýar.
        remove_ext(f_name, dot_h);
       // printf(L"Faýlyň ady:%s\t", f_name);

        // Baş funksiýanyň prototipi
        c_trans_write_file_fn_prototype(h_source, f_name);
        // Baş funksiýanyň açylýan ýeri, kodly faýlda ýazylýar
        c_trans_write_file_fn_open(c_source, f_name);

        // Baş faýla ýazmaly funksiýanyň prototipi.

        wchar_t fn_prototype[MAX_FILE_LEN];
        c_trans_get_file_fn_prototype(f_name, fn_prototype);
        wcsncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][1], fn_prototype, wcslen(fn_prototype)+1);
        //printf(L"Funksiyanyn ady:%s\n", fn_prototype);
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
    write_to_hsource_loc_fns_proto(h_source);
    write_to_csource_loc_fns(c_source);

    write_source_std_kabul_et_source_code();

	finishize_h_source(h_source);

	// Fayllar yapylyar
	fclose(c_source);
	fclose(h_source);

	return 1;
}


/** Baş däl faýllaryň içinde ulanylýan funksiýanyň, headerdäki prototipini ýazýar
 *
 * @f_h - header faýl
 * @f_h - funksiýanyň ady
**/
int c_trans_write_file_fn_prototype(FILE *f_h, wchar_t *fn_name)
{
    wchar_t *l = NULL;
    int llen = 0;

    // void _fn_name_tepl();
    int size = 0;
    wchar_t *line = NULL;

    // bashy
    wchar_t *start = L"void _";
    wcsadd_on_heap( &line, &size, start );

    // ortak
    if (wcslen(line)+wcslen(fn_name)>size)
    {
        size += wcslen(fn_name)+1;
    }
    wcsadd_on_heap( &line, &size, fn_name );

    // sony
    wchar_t *end = L"_tepl(); \n";
    if (wcslen(line)+wcslen(end)>size)
    {
        size += wcslen(end)+1;
    }
    wcsadd_on_heap( &line, &size, end );

    /// Ýasaljak koda ýazylýar
    write_code_line(f_h, &l, &llen , line);

    free(line);

    return 1;
}


int c_trans_write_file_fn_open(FILE *f_h, wchar_t *fn_name)
{
    wchar_t *l = NULL;
    int llen = 0;

    // void _fn_name_tepl();
    int size = 0;
    wchar_t *line = NULL;

    // bashy
    wchar_t *start = L"void _";
    wcsadd_on_heap( &line, &size, start );

    // ortak
    wcsadd_on_heap( &line, &size, fn_name );

    // sony
    wchar_t *end = L"_tepl(){ \n";
    wcsadd_on_heap( &line, &size, end );

    /// Ýasaljak koda ýazylýar
    write_code_line(f_h, &l, &llen , line);

    free(line);

    return 1;
}


// Faýlyň içine algoritmi goşýar.
int c_trans_source_add_algor(FILE *f, int main_file)
{
    int i, len=0; wchar_t *l = NULL;
    wchar_t *putl = NULL;
    int putllen = 0;

    for (i=0; i<CUR_ALGOR_ITEMS_NUM; ++i)
    {

        CMD_GET_C_CODE[CUR_ALGOR[i].cmd_class][CUR_ALGOR[i].cmd_type](&CUR_ALGOR[i], &l, &len);
        if (!is_open_block_cmd(&CUR_ALGOR[i]) && !is_close_block_cmd(&CUR_ALGOR[i]))
            cmd_wrapper_c_code(&l, &len);
        else
            cmd_block_wrapper_c_code(&l, &len);

        write_code_line(f, &putl, &putllen , l);

        if (l!=NULL)
        {
            free(l);
            l = NULL;
            len = 0;
        }
    }
    return 1;
}


// Faýlyň funksiýasynyň prototipini gaýtarýar
wchar_t *c_trans_get_file_fn_prototype(wchar_t *fn_name, wchar_t *line)
{
    wchar_t *start = L"_";

    wcsncpy(line, start, wcslen(start)+1);

    // ortasy
    wcsncat(line, fn_name, wcslen(fn_name));
    //

    // sony
    wchar_t *end = L"_tepl();";
    wcsncat(line, end, wcslen(end));

    return line;
}


int is_open_block_cmd(command *cmd)
{
    if (cmd->is_compl)
    {
        int type_num = CMD_FN_DEF_TYPE;
        if (cmd->cmd_class==CMD_CLASS_CTRL_STTMNT || cmd->cmd_class==CMD_CLASS_LOOP_STTMNT || (cmd->cmd_class==CMD_CLASS_FN_DEF &&
                                                                                              cmd->cmd_type ==CMD_FN_DEF_TYPE))
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

/// Tipiň (TPL-ň ýa Ulanyjy tarapyndan ýasalanyň) C kodyny ýazýar.
void get_type_c_code(int type_class, int type_num, wchar_t **l, int *llen)
{
    if (type_class==TOK_CLASS_DEF_TYPE)
    {
        wcsadd_on_heap( l, llen, def_type_list[type_num].value );
    }
    else
    {
        wcsadd_on_heap( l, llen, USER_DEF_TYPES[type_num].ident );
    }
}


void get_type_init_val_c_code(wchar_t is_arr, int type_class, int type_num, wchar_t **l, int *llen)
{
    if ( type_class!=TOK_CLASS_DEF_TYPE )
        return;

    wchar_t *arr_o = L" = {",
            *arr_c = L"}",
            *o     = L" = ",
            *f = NULL;

    if ( is_arr )
    {
        f = arr_o;
    }
    else
    {
        f = o;
    }

    wcsadd_on_heap( l, llen, f );

    // Ülňileriň başlangyç maglumatlary
    wcsadd_on_heap( l, llen, def_type_list[type_num].init_val );

    if (is_arr)
    {
        wcsadd_on_heap( l, llen, arr_c );
    }
}

void get_cmd_end_c_code(wchar_t **l, int *llen)
{
    wchar_t *end = L"; \n";
    wcsadd_on_heap( l, llen, end );
}

void cmd_item_get_c_code( command_item *ci, wchar_t **mem, int *memlen )
{
    if( ci->type==CMD_ITEM )
    {
        cmd_get_c_code(&ci->cmd, mem, memlen);
    }
    else if( ci->type==TOKEN_ITEM )
    {
        tok_get_c_code( &ci->tok, mem, memlen );
    }
    else if( ci->type==PAREN_ITEM )
    {
        paren_get_c_code( &ci->paren, mem, memlen );
    }
}

void paren_item_get_c_code( parenthesis_elem *pe, wchar_t **mem, int *memlen )
{
    if( pe->type==CMD_ITEM )
    {
        cmd_get_c_code( &pe->cmd, mem, memlen );
    }
    else if( pe->type==TOKEN_ITEM )
    {
        tok_get_c_code( &pe->tok, mem, memlen );
    }
    else if( pe->type==PAREN_ITEM )
    {
        paren_get_c_code( &pe->paren, mem, memlen );
    }
}


void tok_get_c_code( token *t, wchar_t **mem, int *memlen )
{
    TOK_GET_C_CODE[ get_token_type_class( t ) ][ get_token_type( t ) ]( t, mem, memlen );
}
void cmd_get_c_code( command *cmd, wchar_t **mem, int *memlen )
{
    CMD_GET_C_CODE[ cmd->cmd_class ][ cmd->cmd_type ]( cmd, mem, memlen );
}



void work_with_translator_whole_project()
{
    /// Ýasalan kodda çagyrylan global ülňileriň yglan edilen .h faýlynyň çagyrylan ýerinde inklud etmeli
    work_with_called_glob_vars();
    work_with_called_glob_arrs();

    /** C kodly faýlynda başga C kodly faýlynda yglan edilen global ülňi ýa funksiýa ulanylyp biliner.
    Mümkin ulanylan maglumatyň ulanylan faýlyndan soňky faýlynda bolup biliner. Şonuň üçin
    Ulanylan kodly faýlynda prototipiň yglan edilen faýly goşulýar.*/
    if (INCLUDES_NUM)
    {
        translator_to_c_add_includes();
    }

    /// Hemme ýasaýlan C projektlere gerek, goşmaça TPL programmanyň öz faýllary hem goşulýar.
    add_utypes_c_code_file();
    add_conv_basic_type_c_code_file();
}


