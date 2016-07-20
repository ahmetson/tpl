#ifndef PARSER_H
#define PARSER_H

extern wchar_t PARSER_DEFAULT_MODE;
extern wchar_t STRING_PREPARE_MODE;
extern wchar_t STRING_MODE;
extern wchar_t PARENTHESIS_MODE;

int parse();
int is_valid_wchar_t();

void prevent_from_parsing_file();

void process_char( char check_char, int *source_pointer, wchar_t *source );

int parser_mode_end_cmd();

#endif
