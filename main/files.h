/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#ifndef TPL_FILES_H
#define TPL_FILES_H

#ifndef MAX_FILE_LEN
#define MAX_FILE_LEN  255
#define MAX_TOKEN_LEN 32
#endif

// Faýllaryň sanawy
typedef struct{
    char source  [MAX_FILE_LEN];         // Original TEPL kodly saýl nirede ýerleşýär
    char h_source[MAX_FILE_LEN];         // Faýlyň Ç dilindäki kodynyň headeri nirede ýerleşýär
    char c_source[MAX_FILE_LEN];         // Faýlyň Ç dilindäki kody nirede ýerleşýär
    char name    [MAX_FILE_LEN];         // Faýlyň ady, öňündäki papkalary we faýl ekstensiesi hasap edilmesiz
    int  num;
}file_item;

int add_to_file_list_file();
int add_to_file_list_source(const char *source);
int add_to_file_list_name(char *name);
int add_to_file_list_h_source(char *source);
int add_to_file_list_c_source(char *source);

file_item *get_file_by_name(char *name);

int is_glob_defs_file_exist(char *fn);
void glob_defs_file_add(char fn[MAX_FILE_LEN]);

#endif // TPL_FILES_H
