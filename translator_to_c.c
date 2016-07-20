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
#include "fns/3rdparty/std/chap_et.h"


/** . Open file as UTF-8

    . Create additional .C file that will hold additonal code for main .C
    . When created file, open additional file.
    . Concatinate additional file's code to main file

*/


/** Write code instruction */
void write_strings_to_file(FILE *f, wchar_t **l, int *llen, wchar_t *code)
{
    wcsadd_on_heap(l, llen, code);
    fputws(*l, f);

    free(*l);
    *l = NULL;
    *llen = 0;
}


/// Prepare .H source code. Add first piece of code: #IFNDEF #DEFINE ...
int write_headers_to_h_source(FILE *f, wchar_t *f_name)
{
    wchar_t *ifndef = L"\n#ifndef ",
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

    write_strings_to_file(f, &l, &llen , line3);
    write_strings_to_file(f, &l, &llen , line4);

	free(line3);
	free(line4);

    add_addtn_headers(f);

	fputws(L"\n", f);

	return 1;
}

/// Prepare .C source code. Add first piece of code: #include...
int write_headers_to_c_source(FILE *f, wchar_t *h_source)
{
    wchar_t *include = L"\n#include \"",
            *dquote_endl = L"\" \n";
    wchar_t *line3 = malloc( MAX_PREP_LEN );
        wcsncpy( line3, include, wcslen( include )+1 );
        wcsncat( line3, h_source, wcslen( h_source) );
        wcsncat( line3, dquote_endl, wcslen( dquote_endl ) );

    wchar_t *l = NULL;
    int llen = 0;

    write_strings_to_file(f, &l, &llen , line3);

    add_addtn_headers(f);

    free(line3);

    fputws(L"//.\n", f);
	fputws(L"\n\n", f);

	return 1;
}
/** Add Definition of Global variables to .H source:
        extern [type] [varname]; */
int write_glob_var_def_to_h_source(FILE *f)
{
	int i, line_len = 0;

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
			write_type_c_code_analog(GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, GLOB_VAR_DEFS[i].name );

            write_terminator(&line, &line_len);

            write_strings_to_file(f, &l, &llen , line);

			free(line);
		}
	}

	fputws(L"\n", f);

	return 1;
}
/** Add definition of Global arrays to .H source:
        extern [type] [varname][indexes...];
*/
int write_glob_arr_def_to_h_source(FILE *f)
{
	int i;
	int line_len = 0;

	wchar_t *l = NULL,
            *extern_str = L"extern ",
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_ARR_DEFS_NUMS; ++i)
	{
		if (GLOB_ARR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			//type_num = GLOB_ARR_DEFS[i].type_num;

            line_len = 0;
			wchar_t *line = NULL;
			wcsadd_on_heap( &line, &line_len, extern_str );

			// Ulninin tipi goshulyar
            write_type_c_code_analog(GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, GLOB_ARR_DEFS[i].ident );

			/** Elementler hakda maglumat goşulýar */
            add_arr_elem_inf_c_code( &line, &line_len, 0, i, GLOB_ARR_DEFS[i].incs);

			// Komanda gutardy
            write_terminator( &line, &line_len );

            write_strings_to_file( f, &l, &llen , line );

			free( line );
		}
	}

	fputws(L"\n\n", f);

	return 1;
}
/** Add defintion of Global variables to .C source:
        extern [type] [varname] = [inittype];
*/
int write_glob_var_def_to_c_source(FILE *f)
{
	int i, line_len;

	wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	for(i=0; i<GLOB_VAR_DEFS_NUMS; ++i)
	{
		if (GLOB_VAR_DEFS[i].inf_file_num==CUR_FILE_NUM-1)
		{
			//type_num = GLOB_VAR_DEFS[i].type_num;

            line_len = 0;
			wchar_t *line = NULL;

            if ( GLOB_VAR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && GLOB_VAR_DEFS[ i ].type_num==3 )
                add_string_to_free_list( GLOB_VAR_DEFS[ i ].name, &C_CODE_GLOB_FREE_VARS_NUM, &C_CODE_GLOB_FREE_VARS );
            else if ( GLOB_VAR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_utype_string_elems_to_free_list( GLOB_VAR_DEFS[ i ].type_num, GLOB_VAR_DEFS[ i ].name, &C_CODE_GLOB_FREE_VARS_NUM, &C_CODE_GLOB_FREE_VARS );

			// Ulninin tipi
			write_type_c_code_analog(GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
            wcsadd_on_heap( &line, &line_len, GLOB_VAR_DEFS[i].name );

			// Ülňilere başlangyç maglumatlar baglamaly
            write_type_init_val_c_code(0, GLOB_VAR_DEFS[i].type_class, GLOB_VAR_DEFS[i].type_num, &line, &line_len);

            // Ülňileriň başlangyç maglumatlary
            //wcsadd_on_heap( &line, &line_len, def_type_list[type_num].init_val );

			// Komanda gutardy
            write_terminator(&line, &line_len);

            write_strings_to_file(f, &l, &llen , line);

			line_len = 100;

			free(line);
		}
	}

	return 1;
}
/** Add definition of Global arrays to .C source:
        extern [type] [varname] [indexes...];      */
int write_glob_arr_def_to_c_source(FILE *f)
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

            if ( GLOB_ARR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && GLOB_ARR_DEFS[ i ].type_num==3 )
                add_arr_elems_to_free_list( GLOB_ARR_DEFS[ i ].ident, 0, GLOB_ARR_DEFS[ i ].incs , GLOB_ARR_DEFS_ITEMS[ i ] , &C_CODE_GLOB_FREE_VARS_NUM, &C_CODE_GLOB_FREE_VARS );
            else if ( GLOB_ARR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_arr_utype_string_elems_to_free_list( 0, GLOB_ARR_DEFS[ i ].incs, GLOB_ARR_DEFS_ITEMS[ i ], GLOB_ARR_DEFS[ i ].type_num, GLOB_ARR_DEFS[ i ].ident, &C_CODE_GLOB_FREE_VARS_NUM, &C_CODE_GLOB_FREE_VARS );

			// Ulninin tipi
            write_type_c_code_analog(GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
            wcsadd_on_heap( &line, &line_len, GLOB_ARR_DEFS[i].ident );

            // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 0, i, GLOB_ARR_DEFS[i].incs);

			// Ülňilere başlangyç maglumatlar baglamaly
            write_type_init_val_c_code(1, GLOB_ARR_DEFS[i].type_class, GLOB_ARR_DEFS[i].type_num, &line, &line_len);

            // Komandany soňlaýan bölüm
            write_terminator(&line, &line_len);

            write_strings_to_file(f, &l, &llen , line);

			free(line);
		}
	}

	fputws(L"\n\n", f);

	return 1;
}

/** Add definition of user defined types to header files.
    (User defined types in TEPL are equal to structs in C.)
*/
void write_utype_structs( wchar_t *SOURCE, int *SOURCE_POINTER_NUM, int SOURCE_BYTES_NUM  )
{
    file_item *fi = get_file_by_name( MAIN_FILE_NAME );
    FILE *s = _wfopen(fi->h_source, L"r");
    move_file_to_tmp_var( s, &SOURCE, &SOURCE_BYTES_NUM  );

    fclose( s );

    wchar_t *second_part    = wcsstr( SOURCE, L"///.\n" );
    int      second_offset  = second_part-SOURCE;

    /// Write headers

    s = _wfopen(fi->h_source, L"w, ccs=UTF-8");

    write_substr_to_file( 0, second_offset-1, s, SOURCE );

    /// Write utype structs
    if (USER_DEF_TYPES_NUM)
    {
        add_utype_define_c_code( s );
    }

    /// write function definitions and global variable definitions
    fputws( second_part, s );

    if ( SOURCE_BYTES_NUM )
    {
        free( SOURCE );
        SOURCE = NULL;
        SOURCE_BYTES_NUM = 0;
    }

    fclose( s );
}

/** Writes head of main function.
*/
int write_main_func_open_part(FILE *f)
{
    add_conv_basic_prepere_atexit( f );
	wchar_t *line = L"int main() { system(\"chcp 65001 > nul\");\n\
	wchar_t *_tpl_assign_right, *_tpl_assign_left;\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line);

	return 1;
}
/** Finishize end of main function.
    Adds `return 0` and calls blocks of another TEPL sources,
    and additional functions.
*/
int write_main_func_close_part(FILE *f)
{
    add_conv_basic_free_atexit(f);
    wchar_t *line1 = L"_tpl_call_another_files(); return 0; }\n\n";
    wchar_t *l = NULL;
    int llen = 0;
    write_strings_to_file(f, &l, &llen , line1);

	return 1;
}
// Esasy funksiyanyn sonyny taynlayar
/** Finishizes end of functions. This functions are implementation of TEPL sources
*/
int write_void_func_close_part(FILE *f)
{
    wchar_t *line1 = L"}\n";
    wchar_t *l = NULL;
    int llen = 0;

    write_strings_to_file(f, &l, &llen , line1);

	return 1;
}
/** Finishizes .H source codes.
    Adds `#endif` at the end of source codes.
    #endif required for security reasons.
*/
int write_h_source_ending(FILE *f)
{
    wchar_t *line1 = L"\n#endif \n";
    wchar_t *l = NULL;
    int llen = 0;

    write_strings_to_file(f, &l, &llen , line1);

	return 1;
}
/** Adds definitions of local variables inside of MAIN or VOID functions to .C source:
        [type] [varname] = [initval];
*/
int write_loc_var_def(FILE *f, wchar_t main_file)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (main_file)
	{
		int i;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
            int  line_len = 0;
			wchar_t *line = NULL;
			write_type_c_code_analog(LOCAL_VAR_DEFS[ i ].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

            if ( LOCAL_VAR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && LOCAL_VAR_DEFS[ i ].type_num==3 )
                add_string_to_free_list( LOCAL_VAR_DEFS[ i ].name, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );
            else if ( LOCAL_VAR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_utype_string_elems_to_free_list( LOCAL_VAR_DEFS[ i ].type_num, LOCAL_VAR_DEFS[ i ].name, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_VAR_DEFS[i].name );

            // Ülňiniň başlangyç maglumatlary goýulýar.
            write_type_init_val_c_code(0, LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

			// Komanda gutardy
			write_terminator( &line, &line_len );

			write_strings_to_file(f, &l, &llen , line);

			free(line);
		}
	}
	else
	{
		int i;

		for(i=0; i<LOCAL_VAR_DEFS_NUMS; ++i)
		{
            //if (LOCAL_VAR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
            //    continue;

            if ( LOCAL_VAR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && LOCAL_VAR_DEFS[ i ].type_num==3 )
                add_string_to_free_list( LOCAL_VAR_DEFS[ i ].name, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );
            else if ( LOCAL_VAR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_utype_string_elems_to_free_list( LOCAL_VAR_DEFS[ i ].type_num, LOCAL_VAR_DEFS[ i ].name, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );


            int line_len = 0;
			wchar_t *line = malloc(line_len);

			write_type_c_code_analog(LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_VAR_DEFS[i].name );

            // Ülňilere başlangyç maglumatlar baglamaly
            write_type_init_val_c_code(0, LOCAL_VAR_DEFS[i].type_class, LOCAL_VAR_DEFS[i].type_num, &line, &line_len);

			// Komanda gutardy
			write_terminator(&line, &line_len);

			//printf(L"%s global ulna goshulyar\n", line);
			write_strings_to_file(f, &l, &llen , line);

			free(line);
		}
	}
	return 1;
}
/** Adds definitions of local arrays inside of MAIN or VOID functions to .C source:
        [type] [varname][indexes...];

*/
int write_loc_arr_def(FILE *f, wchar_t main_file)
{
    wchar_t *l = NULL,
            *space = L" ";
    int llen = 0;

	if (main_file)
	{
		int i;

        for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
			//type_num = LOCAL_ARR_DEFS[i].type_num;

            if ( LOCAL_ARR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && LOCAL_ARR_DEFS[ i ].type_num==3 )
                add_arr_elems_to_free_list( LOCAL_ARR_DEFS[ i ].ident, 0, LOCAL_ARR_DEFS[ i ].incs , LOCAL_ARR_DEFS_ITEMS[ i ] , &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );
            else if ( LOCAL_ARR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_arr_utype_string_elems_to_free_list( 0, LOCAL_ARR_DEFS[ i ].incs, LOCAL_ARR_DEFS_ITEMS[ i ], LOCAL_ARR_DEFS[ i ].type_num, LOCAL_ARR_DEFS[ i ].ident, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );

            int line_len = 0;
			wchar_t *line = NULL;
			write_type_c_code_analog(LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_ARR_DEFS[i].ident );

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

            // Ülňilere başlangyç maglumatlar baglamaly
            write_type_init_val_c_code(1, LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            // Komanda gutardy
			write_terminator(&line, &line_len);

            write_strings_to_file(f, &l, &llen , line);

			//printf(L"%s global ulna goshulyar\n", line);
			free(line);
		}

		fputws(L"\n\n", f);

	}
	else
	{
		int i;

		for(i=0; i<LOCAL_ARR_DEFS_NUMS; ++i)
        {
            //if (LOCAL_ARR_DEFS[i].inf_file_num!=CUR_FILE_NUM)
            //    continue;

            if ( LOCAL_ARR_DEFS[ i ].type_class==TOK_CLASS_DEF_TYPE && LOCAL_ARR_DEFS[ i ].type_num==3 )
                add_arr_elems_to_free_list( LOCAL_ARR_DEFS[ i ].ident, 0, LOCAL_ARR_DEFS[ i ].incs , LOCAL_ARR_DEFS_ITEMS[ i ] , &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );
            else if ( LOCAL_ARR_DEFS[ i ].type_class!=TOK_CLASS_DEF_TYPE )
                add_arr_utype_string_elems_to_free_list( 0, LOCAL_ARR_DEFS[ i ].incs, LOCAL_ARR_DEFS_ITEMS[ i ], LOCAL_VAR_DEFS[ i ].type_num, LOCAL_ARR_DEFS[ i ].ident, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );

			int line_len = 0;
			wchar_t *line = NULL;
			write_type_c_code_analog(LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            wcsadd_on_heap( &line, &line_len, space );

			// Ulninin ady goshulyar
			wcsadd_on_heap( &line, &line_len, LOCAL_ARR_DEFS[i].ident );

			 // sanawyň elelemntleri hakda maglumat goşulýar
            add_arr_elem_inf_c_code(&line, &line_len, 1, i, LOCAL_ARR_DEFS[i].incs);

            // Başlangyç maglumatlar goşulýar
            write_type_init_val_c_code(1, LOCAL_ARR_DEFS[i].type_class, LOCAL_ARR_DEFS[i].type_num, &line, &line_len);

            // Komanda gutardy
			write_terminator(&line, &line_len);


            write_strings_to_file(f, &l, &llen, line);

			//printf(L"%s global ulna goshulyar\n", line);
			line_len = 0;

			free(line);

		}
	}
	return 1;
}
/** Creates .C and .H source code for .TEPL source code.
    Each TEPL source code implemented as function inside of C source code.
    Function that implements main TEPL source code called MAIN,
    and functions that implements another TEPL source codes called VOID.
*/
int translator_to_c()
{
    wchar_t *dot_h = L".h";
    char     main_file = 0;
    file_item *fi = &FILES[ CUR_FILE_NUM-1 ];

    /// The names of functions that implement TEPL source codes made from source file name.
    wchar_t f_name[ MAX_FILE_LEN ] = {0};
    wcsncpys( f_name, CUR_FILE_NAME );

	/// Create .C and .H source codes
	FILE *c_source = _wfopen(fi->c_source, L"w, ccs=UTF-8");
	FILE *h_source = _wfopen(fi->h_source, L"w, ccs=UTF-8");

    /// Add header files to source codes
	write_headers_to_h_source(h_source, fi->name);
	write_headers_to_c_source(c_source, wcsncat( fi->name, dot_h, wcslen( dot_h ) ) );


    /// Add definitions of global variables defined in this source code
	if ( is_glob_def_var_in_cur() )
	{
		write_glob_var_def_to_h_source(h_source);
		write_glob_var_def_to_c_source(c_source);
	}
	/// Add definitions of global arrays defined in this source code
	if ( is_glob_def_arr_in_cur() )
    {
        write_glob_arr_def_to_h_source(h_source);
		write_glob_arr_def_to_c_source(c_source);
    }

	/// Function that implements TEPL source, must be `main` inside of .C source
	if ( is_wcseq( MAIN_FILE_NAME, CUR_FILE_NAME ) )
	{
		main_file = 1;
		/// Source code that holds main function also holds all additional information needed in program.
		add_conv_basic_type_c_code_file( c_source, h_source );

        fputws( L"///.\n", h_source );

		write_main_func_open_part( c_source );

		//write_utype_structs( h_source );
	}
	else
    {
        main_file = 0;

        /// All VOID functions are called inside of MAIN function. So, source code of MAIN function
        /// needs to know information about void functions.
        MAIN_FILE_INCLUDES_NUM++;
        MAIN_FILE_INCLUDES = realloc(MAIN_FILE_INCLUDES, MAIN_FILE_INCLUDES_NUM*(sizeof(*MAIN_FILE_INCLUDES)));

        /// Add name of VOID function.
        wcsncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][0], fi->name, wcslen(fi->name)+1);

        /// Make function name from file name.
        remove_ext(f_name, dot_h);
       // printf(L"Faýlyň ady:%s\t", f_name);

        /// Make prototype of VOID function inside of H source code for VOID
        //write_fn_prototype_to_h_source(h_source, f_name);
        write_fn_prototype_void_def(h_source, f_name);
        // Baş funksiýanyň açylýan ýeri, kodly faýlda ýazylýar
        /// Make definition of function and open brace inside of C soure code for VOID
        write_fn_open_part_to_c_source(c_source, f_name);

        /// Create command that calls VOID function inside of MAIN
        wchar_t fn_prototype[MAX_FILE_LEN];
        make_fn_prototype(f_name, fn_prototype);
        wcsncpy(MAIN_FILE_INCLUDES[MAIN_FILE_INCLUDES_NUM-1][1], fn_prototype, wcslen(fn_prototype)+1);
        //printf(L"Funksiyanyn ady:%s\n", fn_prototype);
    }

	/// Add definition of local variables and arrays
	write_loc_var_def(c_source, main_file);
	write_loc_arr_def(c_source, main_file);

    /// Add algorithm. This is main body of program
    write_algor(c_source, main_file);

    write_loc_multi_free_call( c_source );

    /// Function is end
    if (main_file)
        write_main_func_close_part(c_source);
    else
        write_void_func_close_part(c_source);
    /// Add definitions of functions defined by user inside of TEPL source code.
    write_to_hsource_loc_fns_proto(h_source);
    write_to_csource_loc_fns(c_source);

    /** VOID functions inside of MAIN called from single place. So create opener of single place
        main () {
            ...
            call_voids();
        }
        ...
        call_voids() {      /// Define this in here
            name of void function1
            name of void function2
            ...
        }
    */
    if ( main_file )
    {
        trans_to_c_write_addtn_fn_multi_free( c_source );
        write_to_csource_call_another_files_fn_open(c_source);
    }

	write_h_source_ending(h_source);

	/// .C and .H source codes done!
	fclose(c_source);
	fclose(h_source);

	return 1;
}


/** puts prototypes of VOID function inside of .H file of void function.
 *
 * @fH - pointer to header
 * @fnName - function name
**/
int write_fn_prototype_to_h_source(FILE *f_h, wchar_t *fn_name)
{
    wchar_t fn_prototype[MAX_FILE_LEN+11] = {0};
    make_fn_prototype( fn_name, fn_prototype );

    wchar_t *l = NULL;
    int llen = 0;

    write_strings_to_file(f_h, &l, &llen , fn_prototype);

    return 1;
}


void write_fn_prototype_void_def(FILE *f_h, wchar_t *fn_name)
{
    wchar_t fn_prototype[MAX_FILE_LEN+11] = {0};
    wchar_t fn_prototype_void_def[MAX_FILE_LEN+11+5] = L"void ";
    make_fn_prototype( fn_name, fn_prototype );

    wcscat( fn_prototype_void_def, fn_prototype );

    wchar_t *l = NULL;
    int llen = 0;

    write_strings_to_file(f_h, &l, &llen , fn_prototype_void_def);

}


/** puts opener of VOID function inside of .C file of void function.
 *
 * @fH - pointer to header
 * @fnName - function name
**/
int write_fn_open_part_to_c_source(FILE *f_h, wchar_t *fn_name)
{
    wchar_t fn_prototype[MAX_FILE_LEN+11] = {0};
    make_fn_prototype( fn_name, fn_prototype );

    wchar_t fn_prototype_void_def[MAX_FILE_LEN+11+5] = L"void ";

    wcscat( fn_prototype_void_def, fn_prototype );

    str_change_char( fn_prototype_void_def, L';', L' ' );
    str_change_char( fn_prototype_void_def, L'\n', L' ' );

    wchar_t *l = NULL;
    int llen = 0;

    // void _fn_name_tepl();
    int line_len = 0;
    wchar_t *line = NULL;

    wcsadd_on_heap( &line, &line_len, fn_prototype_void_def );

    wchar_t *end = L"{ \n\
	wchar_t *_tpl_assign_right, *_tpl_assign_left;";
    wcsadd_on_heap( &line, &line_len, end );

    /// Write to source code
    write_strings_to_file(f_h, &l, &llen , line);

    free(line);

    return 1;
}


/// Adds algorithm commands to C source
int write_algor(FILE *f, int main_file)
{
    int i, len=0; wchar_t *l = NULL;
    wchar_t *putl = NULL;
    int putllen = 0;

    for (i=0; i<CUR_ALGOR_ITEMS_NUM; ++i)
    {
        CMD_GET_C_CODE[CUR_ALGOR[i].cmd_class][CUR_ALGOR[i].cmd_type]( &CUR_ALGOR[i], &l, &len );
        if (!is_open_block_cmd(&CUR_ALGOR[i]) && !is_close_block_cmd(&CUR_ALGOR[i]))
        {
            cmd_wrapper_c_code(&l, &len);
        }
        else
        {
            cmd_block_wrapper_c_code(&l, &len);
        }
        /// Add pre codes
        if ( CMD_C_CODE_PRE_LEN!=0 )
        {
            wchar_t *tmp =  NULL;
            int tmplen = 0;
            wcsadd_on_heap( &tmp, &tmplen, CMD_C_CODE_PRE );
            wcsadd_on_heap( &tmp, &tmplen, l );

            len = tmplen;
            l = realloc( l, tmplen );
            wcsncpy( l, tmp, wcslen( tmp )+1 );
            free ( tmp );
            free( CMD_C_CODE_PRE );
            CMD_C_CODE_PRE = NULL;
            CMD_C_CODE_PRE_LEN = 0;
        }
        else if ( CMD_ASSIGN_C_CODE_PRE_LEN!=0 )
        {
            wchar_t *tmp =  NULL;
            int tmplen = 0;
            wcsadd_on_heap( &tmp, &tmplen, CMD_ASSIGN_C_CODE_PRE );
            wcsadd_on_heap( &tmp, &tmplen, l );

            l = realloc( l, tmplen );
            wcsncpy( l, tmp, wcslen( tmp )+1 );
            free ( tmp );
            free( CMD_ASSIGN_C_CODE_PRE );
            CMD_ASSIGN_C_CODE_PRE = NULL;
            CMD_ASSIGN_C_CODE_PRE_LEN = 0;
        }
        /// Add after code
        if ( CMD_C_CODE_AFTER_LEN!=0 )
        {
            wcsadd_on_heap( &l, &len, CMD_C_CODE_AFTER );
            free( CMD_C_CODE_AFTER );
            CMD_C_CODE_AFTER = NULL;
            CMD_C_CODE_AFTER_LEN = 0;
        }
        else if ( CMD_ASSIGN_C_CODE_AFTER_LEN!=0 )
        {
            wcsadd_on_heap( &l, &len, CMD_ASSIGN_C_CODE_AFTER );
            free( CMD_ASSIGN_C_CODE_AFTER );
            CMD_ASSIGN_C_CODE_AFTER = NULL;
            CMD_ASSIGN_C_CODE_AFTER_LEN = 0;
        }

        write_strings_to_file(f, &putl, &putllen , l);

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
/** Makes prototype of function

    @fnName - function name
    @line   - string with prototype
*/
wchar_t *make_fn_prototype(wchar_t *fn_name, wchar_t *line)
{
    str_change_char( fn_name, L'/', L'_' );
    fn_name = remove_ext( fn_name, L".tepl" );
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

/** Checks whether TEPL command is block opener or not
*/
int is_open_block_cmd(command *cmd)
{
    if (cmd->is_compl)
    {
        if (cmd->cmd_class==CMD_CLASS_CTRL_STTMNT || cmd->cmd_class==CMD_CLASS_LOOP_STTMNT || (cmd->cmd_class==CMD_CLASS_FN_DEF &&
                                                                                              cmd->cmd_type ==CMD_FN_DEF_TYPE))
            return 1;
    }
    return 0;
}

/** Checks whether TEPL command is block closer or not */
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

/** Writes C code analog of TPL type names (both for TPL or user types). */
void write_type_c_code_analog(int type_class, int type_num, wchar_t **l, int *llen)
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


/** Writes C code of initial value of variables for user defined types */
void write_utype_init_val( wchar_t **l, int *llen, int type_num )
{
    wchar_t *brace_o    = L" { ",
            *brace_c    = L" } ",
            *comma      = L" , ";

    wcsadd_on_heap( l, llen, brace_o );

    int elem_num;
    for ( elem_num=0; elem_num<USER_DEF_TYPES[ type_num ].items_num; ++elem_num )
    {
        if ( -1!=USER_DEF_TYPE_ITEMS[ type_num ][ elem_num ].arr_items_addr )
        {
            wcsadd_on_heap( l, llen, brace_o );
        }

        if ( TOK_CLASS_DEF_TYPE!=USER_DEF_TYPE_ITEMS[ type_num ][ elem_num ].type_class )
            write_utype_init_val( l, llen, USER_DEF_TYPE_ITEMS[ type_num ][ elem_num ].type_num );
        else
            wcsadd_on_heap( l, llen, def_type_list[ USER_DEF_TYPE_ITEMS[ type_num ][ elem_num ].type_num ].init_val );

        if ( -1!=USER_DEF_TYPE_ITEMS[ type_num ][ elem_num ].arr_items_addr )
        {
            wcsadd_on_heap( l, llen, brace_c );
        }

        if ( elem_num+1!=USER_DEF_TYPES[ type_num ].items_num )
            wcsadd_on_heap( l, llen, comma );
    }

    wcsadd_on_heap( l, llen, brace_c );
}

/** Writes C code of initial value. Usually sets to variable or array definitions:

    @isArr      - write initial value for variable or array
    @typeClass  - type class of variable/array. Initial value are vary for each types
    @typeNUM    - type of variable/array.
    @l          - string that will hold C code of initial value code
    @llen       - length of @l string
*/
void write_type_init_val_c_code(wchar_t is_arr, int type_class, int type_num, wchar_t **l, int *llen)
{
    wchar_t *arr_o = L" = {",   // array open
            *arr_c = L"}",      // array close
            *o     = L" = ";    // open brace

    /// Add open brace
    if ( is_arr )
    {
        wcsadd_on_heap( l, llen, arr_o );
    }
    else
    {
        wcsadd_on_heap( l, llen, o );
    }


    if ( type_class!=TOK_CLASS_DEF_TYPE )
    {
        /** If variable in user defined type:
        write "{"
        Get address of elements
        for each element of variable
            call function that will write initial value of items
        write "}"

        */
        write_utype_init_val( l, llen, type_num  );
    }
    else
    {
        /// Add initial value between braces
        wcsadd_on_heap( l, llen, def_type_list[type_num].init_val );
    }

    /// Add close brace
    if ( is_arr )
    {
        wcsadd_on_heap( l, llen, arr_c );
    }
}

/// Writes `;` (command end) to C code line
void write_terminator(wchar_t **l, int *llen)
{
    wchar_t *end = L"; \n";
    wcsadd_on_heap( l, llen, end );
}

/** Writes C code analog for command items.
    Each command made by command items. Command items can be: nested command, token or parenthesis

    @ci     - command item
    @mem    - command item's C code analog will be hold in @mem string
    @memlen -  length of @mem string
*/
void write_cmd_item_c_code( command_item *ci, wchar_t **mem, int *memlen )
{
    if( ci->type==CMD_ITEM )
    {
        write_cmd_c_code(&ci->cmd, mem, memlen);
    }
    else if( ci->type==TOKEN_ITEM )
    {
        write_tok_c_code( &ci->tok, mem, memlen );
    }
    else if( ci->type==PAREN_ITEM )
    {
        paren_get_c_code( &ci->paren, mem, memlen );
    }
}
/** Writes C code analog for parenthesis items.
    Data that holded by parenthesis called parenthesis item.
    Parenthesis items can be: nested parenthesis, token or command

    @pe     - parenthesis item
    @mem    - parenthesis item's C code analog will be hold in @mem string
    @memlen -  length of @mem string
*/
void write_paren_item_c_code( parenthesis_elem *pe, wchar_t **mem, int *memlen )
{
    if( pe->type==CMD_ITEM )
    {
        write_cmd_c_code( &pe->cmd, mem, memlen );
    }
    else if( pe->type==TOKEN_ITEM )
    {
        write_tok_c_code( &pe->tok, mem, memlen );
    }
    else if( pe->type==PAREN_ITEM )
    {
        paren_get_c_code( &pe->paren, mem, memlen );
    }
}
/** Writes C code analog of token.
    Tokens are smallest item used in programming languages
    Most of tokens in TPL are same with tokens in C language

    @t      - token
    @mem    - token's C code analog will be hold in @mem string
    @memlen -  length of @mem string
*/
void write_tok_c_code( token *t, wchar_t **mem, int *memlen )
{
    TOK_GET_C_CODE[ get_token_type_class( t ) ][ get_token_type( t ) ]( t, mem, memlen );
}
/** Writes C code analog of command.

    @cmd    - command
    @mem    - command's C code analog will be hold in @mem string
    @memlen -  length of @mem string
*/
void write_cmd_c_code( command *cmd, wchar_t **mem, int *memlen )
{
    CMD_GET_C_CODE[ cmd->cmd_class ][ cmd->cmd_type ]( cmd, mem, memlen );
}

/** If source codes calls global variable/array/function/user type defined in another source codes.
    Make link in source code, so GCC can know where he can take information about global data.

    Also add additional source codes required in program.
*/
void async_source_codes()
{
    write_source_std_kabul_et_source_code();
    write_source_std_chap_et_source_code();

    /// Find in each source code calling the global variables and arrays that are defined in another source code.
    /// Prepare list of required #include preprocessors
    work_with_called_glob_vars();
    work_with_called_glob_arrs();

    /// Add #include with source code name where global data defined to source code where global data called
    if (INCLUDES_NUM)
    {
        translator_to_c_add_includes();
    }

    write_utype_structs( CUR_SOURCE, &CUR_SOURCE_POINTER, CUR_SOURCE_BYTES_NUM );


    /// Write closer to single place (Single place is function in C source code calls all VOID functions).
    write_to_csource_free_glob_strings(  );

    write_to_csource_call_another_files_fn_close( );
    trans_to_c_write_addtn_fn_glob_vars_free( );
}

void write_loc_multi_free_call( FILE *f )
{
    wchar_t *l = NULL, var_num[ 10 ] = { 0 };
    int llen = 0;

    if ( C_CODE_LOC_FREE_VARS_NUM==0 )
        return;

    wsprintfW(var_num, L"%d", C_CODE_LOC_FREE_VARS_NUM );

    write_strings_to_file(f, &l, &llen , L"_tpl_multi_free(" );
    write_strings_to_file(f, &l, &llen , var_num );
    write_strings_to_file(f, &l, &llen , L", " );

    int i;
    for ( i=0; i<C_CODE_LOC_FREE_VARS_NUM; ++i )
    {
        write_strings_to_file(f, &l, &llen , C_CODE_LOC_FREE_VARS[ i ] );
        if ( i+1<C_CODE_LOC_FREE_VARS_NUM )
            write_strings_to_file(f, &l, &llen , L", " );
    }

    write_strings_to_file(f, &l, &llen , L");\n" );
}

void  write_multi_free_call_in_def_fn( FILE *f, int func_num )
{
    wchar_t *l = NULL, var_num[ 10 ] = { 0 };
    int llen = 0;

    if ( C_CODE_DEF_FN_FREE_VARS_NUM[ func_num ]==0 )
        return;

    wsprintfW(var_num, L"%d", C_CODE_DEF_FN_FREE_VARS_NUM[ func_num ] );

    write_strings_to_file(f, &l, &llen , L"_tpl_multi_free(" );
    write_strings_to_file(f, &l, &llen , var_num );
    write_strings_to_file(f, &l, &llen , L", " );

    int i;
    for ( i=0; i<C_CODE_DEF_FN_FREE_VARS_NUM[ func_num ]; ++i )
    {
        write_strings_to_file(f, &l, &llen , C_CODE_DEF_FN_FREE_VARS[ func_num ][ i ] );
        if ( i+1<C_CODE_DEF_FN_FREE_VARS_NUM[ func_num ] )
            write_strings_to_file(f, &l, &llen , L", " );
    }

    write_strings_to_file(f, &l, &llen , L");\n" );
}

/** Adds string to list of strings */
void add_string_to_free_list ( wchar_t *add_me, int *list_items_num, wchar_t ***l )
{
    *list_items_num += 1;
    *l = realloc( *l, sizeof( **l )*( *list_items_num ) );

    (*l)[ *list_items_num-1 ] = malloc( ( wcslen( add_me )+1 )*sizeof( wchar_t ) );
    wcscpy( (*l)[ *list_items_num-1 ], add_me );
}

/** Add string elements of utype variable into `must free` list.

    @utypeNum       - number of utype, to check utype elements parameters
    @addMe          - variable name in utype
    @listItemsNum   - number of strings in list
    @l              - list of strings
*/
void add_utype_string_elems_to_free_list( int utype_num, wchar_t *add_me, int *list_items_num, wchar_t ***l )
{
    /// @str - is template for string element.
    wchar_t *str = malloc( ( wcslen( add_me )+1+1 )*sizeof( wchar_t ) );
    wcscpy( str, add_me );  //   name of utype variable
    wcscat( str, L"." );    //   dot.

    int i;
    for ( i=0; i<USER_DEF_TYPES[ utype_num ].items_num; ++i )
    {
        /// Simple element?
        if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].arr_items_addr==-1 )
        {
            /// Is string element of utype?
            if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class==TOK_CLASS_DEF_TYPE &&
                 USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num  ==3 )
            {
                wchar_t *str_sub = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                wcscpy( str_sub, str );
                wcscat( str_sub, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );
                /** Add to list string element */
                add_string_to_free_list( str_sub, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );

                free( str_sub );
            }
            /// Ya, utype element inside of utype?
            else if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class!=TOK_CLASS_DEF_TYPE )
            {
                wchar_t *str_sub = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                wcscpy( str_sub, str );
                wcscat( str_sub, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                /// Add them into free list.
                add_utype_string_elems_to_free_list( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num, str_sub, list_items_num, l );

                free( str_sub );
            }
        }
        /// Array element
        else
        {
            /// Is array of string in utype?
            if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class==TOK_CLASS_DEF_TYPE &&
                 USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num  ==3 )
            {
                wchar_t *sub_str = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                wcscpy( sub_str, str );
                wcscat( sub_str, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                int arr_addr = USER_DEF_TYPE_ITEMS[ utype_num ][ i ].arr_items_addr;

                add_arr_elems_to_free_list( sub_str, 0, USER_DEF_TYPES_ARRS[ utype_num ][ arr_addr ].incs , USER_DEF_TYPES_ARRS_ITEMS[ utype_num ][ arr_addr ] , list_items_num, l );

                free( sub_str );
            }
            /// Ya, array of utypes in utype?
            else if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class!=TOK_CLASS_CONST_DATA )
            {
                wchar_t *sub_str = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                wcscpy( sub_str, str );
                wcscat( sub_str, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                int arr_addr = USER_DEF_TYPE_ITEMS[ utype_num ][ i ].arr_items_addr;

                add_arr_utype_string_elems_to_free_list( 0,  USER_DEF_TYPES_ARRS[ utype_num ][ arr_addr ].incs, USER_DEF_TYPES_ARRS_ITEMS[ utype_num ][ arr_addr ],
                                                         USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num, sub_str, list_items_num, l );

                free( sub_str );
            }
        }
    }
    free( str );

}


void add_arr_utype_string_elems_to_free_list( int cur_nested, int nested, array_inc_item *elems_num, int utype_num, wchar_t *add_me, int *list_items_num, wchar_t ***l )
{
    /// @str - is template for string element.
    wchar_t *str = NULL;//malloc( ( wcslen( add_me )+1+1+2 )*sizeof( wchar_t ) );

    wchar_t *o = L"[",
            *c = L"]",
            *dot = L".",
             elem_num_string[20] = {0};
   int j, i, add_me_len = count_bytes( add_me )+count_bytes(o)+count_bytes(dot)+count_bytes(c)+(sizeof(wchar_t)*20);

    str = malloc( add_me_len );

    //wcscpy( str, add_me );  //   name of utype variable
    //wcscat( str, L"." );    //   dot.

    for ( j=0; j<elems_num[ cur_nested ].elem_num; ++j )
    {

        wcscpy( str, add_me );
        wcscat( str, o );

        wsprintfW(elem_num_string, L"%d", j );
        wcscat( str, elem_num_string );

        wcscat( str, c );
        if( cur_nested+1 < nested )
        {
            add_arr_utype_string_elems_to_free_list( cur_nested+1, nested, elems_num, utype_num, str, list_items_num, l );
        }
        else
        {
            wcscat( str, dot );
            for ( i=0; i<USER_DEF_TYPES[ utype_num ].items_num; ++i )
            {
                /// Simple element?
                if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].arr_items_addr==-1 )
                {
                    /// Is string element of utype?
                    if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class==TOK_CLASS_DEF_TYPE &&
                         USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num  ==3 )
                    {
                        wchar_t *str_sub = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                        wcscpy( str_sub, str );
                        wcscat( str_sub, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );
                        /** Add to list string element */
                        add_string_to_free_list( str_sub, &C_CODE_LOC_FREE_VARS_NUM, &C_CODE_LOC_FREE_VARS );

                        free( str_sub );
                    }
                    /// Ya, utype element inside of utype?
                    else if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class!=TOK_CLASS_DEF_TYPE )
                    {
                        wchar_t *str_sub = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                        wcscpy( str_sub, str );
                        wcscat( str_sub, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                        /// Add them into free list.
                        add_utype_string_elems_to_free_list( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num, str_sub, list_items_num, l );

                        free( str_sub );
                    }
                }
                /// Array element
                else
                {
                    /// Is array of string in utype?
                    if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class==TOK_CLASS_DEF_TYPE &&
                         USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num  ==3 )
                    {
                        wchar_t *sub_str = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                        wcscpy( sub_str, str );
                        wcscat( sub_str, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                        add_arr_elems_to_free_list( sub_str, 0, USER_DEF_TYPES_ARRS[ utype_num ][ i ].incs , USER_DEF_TYPES_ARRS_ITEMS[ utype_num ][ i ] , list_items_num, l );

                        free( sub_str );
                    }
                    /// Ya, array of utypes in utype?
                    else if ( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_class!=TOK_CLASS_CONST_DATA )
                    {
                        wchar_t *sub_str = malloc( ( wcslen( str )+1+wcslen( USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident ) )*sizeof( wchar_t ) );
                        wcscpy( sub_str, str );
                        wcscat( sub_str, USER_DEF_TYPE_ITEMS[ utype_num ][ i ].ident );

                        add_arr_utype_string_elems_to_free_list( 0, USER_DEF_TYPES_ARRS[ utype_num ][i].incs,
                                                                USER_DEF_TYPES_ARRS_ITEMS[ utype_num ][ i ],
                                                                USER_DEF_TYPE_ITEMS[ utype_num ][ i ].type_num,
                                                                sub_str, list_items_num, l );

                        free( sub_str );
                    }
                }
            }
        }
    }
    free( str );

}

void add_arr_elems_to_free_list( wchar_t *base, int cur_nested, int nested, array_inc_item *elems_num, int *list_items_num, wchar_t ***l )
{
   wchar_t *o = L"[",
           *c = L"]",
           *add_me = NULL,
           elem_num_string[20] = {0};
   int i, add_me_len = count_bytes( base )+count_bytes(o)+count_bytes(c)+(sizeof(wchar_t)*20);

    add_me = malloc( add_me_len );

   for ( i=0; i<elems_num[ cur_nested ].elem_num; ++i )
   {
       wcscpy( add_me, base );
       wcscat( add_me, o );

       wsprintfW(elem_num_string, L"%d", i );

       wcscat( add_me, elem_num_string );

       wcscat( add_me, c );

       if ( cur_nested+1 >= nested )
       {
           add_string_to_free_list( add_me, list_items_num, l );
       }
       else
       {
           add_arr_elems_to_free_list( add_me, cur_nested+1, nested, elems_num, list_items_num, l );
       }
    }
    free( add_me );
}


void write_substr_to_file( int i, int finish, FILE *f, wchar_t *str )
{
    for ( ; i<=finish; ++i )
    {
        fputwc( str[ i ], f );
    }
}
