/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "glob.h"

/**
 * Programma-da ulanylýan faýllaryň sanwyna ýene täze faýl goşulýar.
**/
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
    FILES[CUR_FILE_NUM-1] = f;

    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, häzirki işlenilýän faýlyň ady goşulýar.
**/
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
add_to_file_list_h_source(char *source)
{
    strncpy(FILES[CUR_FILE_NUM-1].h_source, source, strlen(source)+1);
    return 1;
}


/**
 * Programma-da ulanylýan faýllaryň sanawynda, faýlyň ýasalan kodly faýlynyň Kodynyň salgysy.
**/
add_to_file_list_c_source(char *source)
{
    strncpy(FILES[CUR_FILE_NUM-1].c_source, source, strlen(source)+1);
    return 1;
}
