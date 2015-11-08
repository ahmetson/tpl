#ifndef PARSER_H
#define PARSER_H

extern char PARSER_DEFAULT_MODE;
extern char STRING_PREPARE_MODE;
extern char STRING_MODE;
extern char PARENTHESIS_MODE;

int parse(FILE *source);

#endif
