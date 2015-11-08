/**
 * Ýasaljak programmanyň faýllary bilen işleýär
**/
#ifndef TPL_FILES_H
#define TPL_FILES_H

#define CONST_MAX_FILE_LEN  255
#define CONST_MAX_TOKEN_LEN 32

// Faýllaryň sanawy
typedef struct{
    char source  [CONST_MAX_FILE_LEN];         // Original TEPL kodly saýl nirede ýerleşýär
    char h_source[CONST_MAX_FILE_LEN];         // Faýlyň Ç dilindäki kodynyň headeri nirede ýerleşýär
    char c_source[CONST_MAX_FILE_LEN];         // Faýlyň Ç dilindäki kody nirede ýerleşýär
    char name    [CONST_MAX_FILE_LEN];         // Faýlyň ady, öňündäki papkalary we faýl ekstensiesi hasap edilmesiz
    int  num;
}file_item;

int add_to_file_list_file();
int add_to_file_list_source(const char *source);
int add_to_file_list_name(char *name);
int add_to_file_list_h_source(char *source);
int add_to_file_list_c_source(char *source);

file_item *get_file_by_name(char *name);

#endif // TPL_FILES_H
