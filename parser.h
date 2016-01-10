#ifndef PARSER_H
#define PARSER_H

extern wchar_t PARSER_DEFAULT_MODE;
extern wchar_t STRING_PREPARE_MODE;
extern wchar_t STRING_MODE;
extern wchar_t PARENTHESIS_MODE;

int parse(FILE *source);
int is_valid_wchar_t();

#endif
