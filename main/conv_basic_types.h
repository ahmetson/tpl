#ifndef MAIN_CONV_BASIC_TYPES_H
#define MAIN_CONV_BASIC_TYPES_H

extern char             *CONV_DEF_TYPES_SOURCE_NAME_NO_H;
extern char             *CONV_DEF_TYPES_SOURCE_NAME;
extern char             *CONV_DEF_TYPES_C_SOURCE_NAME;

void add_conv_basic_type_c_code_file();

void add_conv_basic_free_atexit(FILE *f);

void add_conv_basic_prepere_atexit(FILE *f);

#endif // MAIN_CONV_BASIC_TYPES_H
