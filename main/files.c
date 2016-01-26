/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "../fns.h"
#include "inf.h"
#include "glob.h"
#include "../translator_to_c.h"

/** Programma-da ulanylýan faýllaryň sanawyna ýene täze faýl goşulýar.*/
int files_list_add_new()
{
    ++CUR_FILE_NUM;

    long size;
    if (CUR_FILE_NUM>1)
    {
        size = sizeof(file_item)*CUR_FILE_NUM;
        FILES = realloc(FILES, size);
    }
    else
    {
        size = sizeof(file_item);
        FILES = malloc(size);
    }

    // Ýer berildi. Indi
    // Ýere täze faýl üçin bölüm açylmaly
    file_item f;
    f.num = CUR_FILE_NUM-1;
    FILES[CUR_FILE_NUM-1] = f;

    inf_add_source_code_last_file();

    return 1;
}


/** Programma-da ulanylýan faýllaryň sanawynda, häzirki işlenilýän faýlyň ady goşulýar.*/
int add_last_file_tpl_source_inf(const wchar_t *source)
{
    wcsncpys( FILES[ CUR_FILE_NUM-1 ].source, (wchar_t *)source );
    return 1;
}


/** Programma-da ulanylýan faýllaryň sanawynda, faýlyň ady, ekstensiýesiz.*/
int add_to_file_list_name(wchar_t *name)
{
    wcsncpy(FILES[CUR_FILE_NUM-1].name, name, wcslen(name)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ýasalan kodly faýlynyň Headeriniň salgysy.
**/
int add_last_file_h_source_inf(wchar_t *source)
{
    wcsncpy(FILES[CUR_FILE_NUM-1].h_source, source, wcslen(source)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ýasalan kodly faýlynyň Kodynyň salgysy.
**/
int add_last_file_c_source_inf(wchar_t *source)
{
    wcsncpy(FILES[CUR_FILE_NUM-1].c_source, source, wcslen(source)+1);
    return 1;
}


/** Faýlyň ady boýunça ähli maglumatlynyň adresini gaýtarýar*/
file_item *get_file_by_name(wchar_t *name)
{
    int i, len;
    for(i=0; i<CUR_FILE_NUM; ++i)
    {
        len = wcslen(FILES[i].name)>wcslen(name)?wcslen(FILES[i].name):wcslen(name);
        if (wcsncmp(FILES[i].name, name, len)==0)
            return &FILES[i];
    }
    return &FILES[0];
}

/** Faýlyň TPL kodynyň ady boýunça boýunça ähli maglumatlynyň adresini gaýtarýar*/
file_item *get_file_by_tpl_source_name(wchar_t *name)
{
    int i, len = wcslen(name);
    for(i=0; i<CUR_FILE_NUM; ++i)
    {
        int flen = wcslen(FILES[i].source);
        if (flen==len && wcsncmp(FILES[i].name, name, len)==0)
            return &FILES[i];
    }
    return &FILES[0];
}



int is_dec_file_exist(wchar_t *fn)
{
    int i;
    for(i=0; i<GLOB_DECS_FILES_NUM; ++i)
    {
        if(wcslen(fn)==wcslen(GLOB_DECS_FILES[i]) && wcsncmp(fn, GLOB_DECS_FILES[i], wcslen(fn))==0)
           return 1;
    }
    return 0;
}


void dec_files_add_new( wchar_t fn[ MAX_FILE_LEN ] )
{
    ++GLOB_DECS_FILES_NUM;

    #define LAST ( GLOB_DECS_FILES_NUM-1 )
    GLOB_DECS_FILES = realloc( GLOB_DECS_FILES, sizeof( *GLOB_DECS_FILES )*GLOB_DECS_FILES_NUM );
    GLOB_DECS_FILES[ LAST ] = NULL;
    GLOB_DECS_FILES[ LAST ] = realloc( GLOB_DECS_FILES[ LAST ], MAX_FILE_LEN );
    wcsncpys( GLOB_DECS_FILES[ LAST ], fn );

    #undef LAST
}

void add_file_info(wchar_t *file_name)
{
    wchar_t *dquote = L"\\", *dot_h = L".h", *dot_c = L".c", *dot_tpl = L".tepl";

    wchar_t f_name[ MAX_FILE_LEN ] = {0};// = CUR_FILE_NAME;
	wcsncpys( f_name, file_name );

    files_list_add_new();                // Ýasalmaly programmanyň faýllarynyň sanawyna,
    add_last_file_tpl_source_inf(f_name);

	remove_dirnames(f_name);
	remove_ext(f_name, dot_tpl);
    add_to_file_list_name(f_name);

	wchar_t h_path[MAX_FILE_LEN] = {0};
	wcsncat(h_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(h_path, dquote, wcslen(dquote));
	wcsncat(h_path, f_name, wcslen(f_name));
	wcsncat(h_path, dot_h, wcslen(dot_h));

	// .c faylyn boljak adresi
	wchar_t c_path[MAX_FILE_LEN] = {0};
	wcsncat(c_path, C_SOURCE_FOLDER, wcslen(C_SOURCE_FOLDER));
	wcsncat(c_path, dquote, wcslen(dquote));
	wcsncat(c_path, f_name, wcslen(f_name));
	wcsncat(c_path, dot_c, wcslen(dot_c));

	// Fayllar achylyar:
	add_last_file_h_source_inf(h_path);
	add_last_file_c_source_inf(c_path);
}
