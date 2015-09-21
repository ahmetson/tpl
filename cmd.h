#ifndef FNS_H
#define FNS_H
/*
COMMAND TYPES AND CLASSES ARE:
	1. var class
		1. decl
*/
#include "tokens.h"

// Commands can contain maximum 3 tokens
#define CONST_CMD_MAX_TOKENS 3
extern const int CMD_MAX_TOKENS;

typedef struct{
	token tokens[CONST_CMD_MAX_TOKENS];
	int tokens_num;
	int cmd_type;
	int cmd_class;
	int is_compl;
	int ns;
	int max_tokens_num;						// Komandanyn tipine we klasyna gora maximum nache tokenden ybarat bolup bilyar
} command;

extern void init_cmd(command *cmd);

extern int recognize_cmd(command *cmd);

// Her komanda gornushini barlayan komanda
typedef struct{
	int (*is_cmd) (command *cmd);
}is_cmd_item;

extern is_cmd_item cmd_types[];

// Komandalaryn sany
extern int CMDS_TYPES_NUM;

extern char *cmd_classes[];

#define CONST_MAX_CLASS_TYPES 1
extern int MAX_CLASS_TYPES;
extern char *cmd_class_types[][CONST_MAX_CLASS_TYPES];

// Komandanyn bashynda bolup bilyan tokenlerin sany
extern int CMD_FIRST_TOKENS_NUM;

// Komanda-da bolup biljek tokenlerin sanawynda ulanylyan tip
typedef struct{
	int token_class;
	int token_type;
	int is_required;
}cmd_token;

extern int cmd_first_tokens_classes[];

int cmd_add_token(command *to, token tok);

int parse_cmd(command *cmd);

int is_cmd_def_var(command *cmd);

int is_tok_in_list(cmd_token *list_tok, token *tok);

int def_var_cmd_mod();//command *cmd, token *tok, int tok_num);

int add_to_cmd(command *cmd, token *tok);

// Komandanyn gornushinin global ulnidini yglan etmedigini barlayar
int is_glob_def_var_cmd(command *cmd);

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_var_cmd(command *cmd);

// Komandalar bilen ishleyan bolum
int work_with_cmd();
#endif
