/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "inf.h"
#include "glob.h"
#include "../translator_to_c.h"

/** Programma-da ulanylýan faýllaryň sanawyna ýene täze faýl goşulýar.*/
int add_to_file_list_file()
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
int add_to_file_list_source(const char *source)
{
    strncpy(FILES[CUR_FILE_NUM-1].source, source, strlen(source)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ady, ekstensiýesiz.
**/
int add_to_file_list_name(char *name)
{
    strncpy(FILES[CUR_FILE_NUM-1].name, name, strlen(name)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ýasalan kodly faýlynyň Headeriniň salgysy.
**/
int add_to_file_list_h_source(char *source)
{
    strncpy(FILES[CUR_FILE_NUM-1].h_source, source, strlen(source)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ýasalan kodly faýlynyň Kodynyň salgysy.
**/
int add_to_file_list_c_source(char *source)
{
    strncpy(FILES[CUR_FILE_NUM-1].c_source, source, strlen(source)+1);
    return 1;
}


/** Faýlyň ady boýunça ähli maglumatlynyň adresini gaýtarýar*/
file_item *get_file_by_name(char *name)
{
    int i, len;
    for(i=0; i<CUR_FILE_NUM; ++i)
    {
        len = strlen(FILES[i].name)>strlen(name)?strlen(FILES[i].name):strlen(name);
        if (strncmp(FILES[i].name, name, len)==0)
            return &FILES[i];
    }
    return &FILES[0];
}

/** Faýlyň TPL kodynyň ady boýunça boýunça ähli maglumatlynyň adresini gaýtarýar*/
file_item *get_file_by_tpl_source_name(char *name)
{
    int i, len = strlen(name);
    for(i=0; i<CUR_FILE_NUM; ++i)
    {
        int flen = strlen(FILES[i].source);
        if (flen==len && strncmp(FILES[i].name, name, len)==0)
            return &FILES[i];
    }
    return &FILES[0];
}



int is_glob_defs_file_exist(char *fn)
{
    int i;
    for(i=0; i<GLOB_DECS_FILES_NUM; ++i)
    {
        if(strlen(fn)==strlen(GLOB_DECS_FILES[i]) && strncmp(fn, GLOB_DECS_FILES[i], strlen(fn))==0)
           return 1;
    }
    return 0;
}


void glob_defs_file_add(char fn[MAX_FILE_LEN])
{
    ++GLOB_DECS_FILES_NUM;
    //printf("%d\n", strlen(*fn));
    GLOB_DECS_FILES = realloc(GLOB_DECS_FILES, sizeof(*GLOB_DECS_FILES)*GLOB_DECS_FILES_NUM);
    GLOB_DECS_FILES[GLOB_DECS_FILES_NUM-1] = NULL;
    GLOB_DECS_FILES[GLOB_DECS_FILES_NUM-1] = realloc(GLOB_DECS_FILES[GLOB_DECS_FILES_NUM-1], MAX_FILE_LEN);
    strncpy(GLOB_DECS_FILES[GLOB_DECS_FILES_NUM-1], fn, strlen(fn)+1);
        //printf("HAWAWAWA\n");
  /*  ++GLOB_DECS_FILES_NUM;
    //printf("%d\n", strlen(*fn));
    GLOB_DECS_FILES = malloc(300);*/
    //strncpy(GLOB_DECS_FILES[GLOB_DECS_FILES_NUM-1], fn, strlen(fn)+1);
}

void add_file_info(char *file_name)
{
    char f_name[MAX_FILE_LEN] = {0};// = CUR_FILE_NAME;
	strncpy(f_name, file_name, strlen(file_name)+1);

    add_to_file_list_file();                // Ýasalmaly programmanyň faýllarynyň sanawyna,
    add_to_file_list_source(f_name);

	remove_dirnames(f_name);
	remove_ext(f_name, ".tepl");
    add_to_file_list_name(f_name);

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
	add_to_file_list_h_source(h_path);
	add_to_file_list_c_source(c_path);
}
