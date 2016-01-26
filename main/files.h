/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#ifndef TPL_FILES_H
#define TPL_FILES_H

#ifndef MAX_FILE_LEN
#include <limits.h>
#define MAX_FILE_LEN  PATH_MAX
#define MAX_TOKEN_LEN 32
#endif

// Faýllaryň sanawy
typedef struct{
    wchar_t source  [MAX_FILE_LEN];         // Original TEPL kodly saýl nirede ýerleşýär
    wchar_t h_source[MAX_FILE_LEN];         // Faýlyň Ç dilindäki kodynyň headeri nirede ýerleşýär
    wchar_t c_source[MAX_FILE_LEN];         // Faýlyň Ç dilindäki kody nirede ýerleşýär
    wchar_t name    [MAX_FILE_LEN];         // Faýlyň ady, öňündäki papkalary we faýl ekstensiesi hasap edilmesiz
    int  num;
}file_item;

int files_list_add_new();
int add_last_file_tpl_source_inf(const wchar_t *source);
int add_to_file_list_name(wchar_t *name);
int add_last_file_h_source_inf(wchar_t *source);
int add_last_file_c_source_inf(wchar_t *source);

file_item *get_file_by_name(wchar_t *name);
file_item *get_file_by_tpl_source_name(wchar_t *name);

int is_dec_file_exist(wchar_t *fn);
void dec_files_add_new(wchar_t fn[MAX_FILE_LEN]);

void add_file_info(wchar_t *file_name);

#endif // TPL_FILES_H
