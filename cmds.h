#ifndef CMDS_H
#define CMDS_H

#define CONST_CMD_MAX_ITEMS 4
/*
COMMAND TYPES AND CLASSES ARE:
	1. var class
		1. decl
*/
#include "tokens.h"

// Komandanyn birliginin nomeri
extern const int TOKEN_ITEM;
extern const int CMD_ITEM;

extern const int DEF_VAR_MAX_TOKENS;
extern const int ASSIGN_MAX_ITEMS;

extern int ASSIGN_CLASS_NUM;

typedef struct command_item command_item;
typedef struct command command;

// Bashga komandalar ya tokenler bolup bilyan komanda
struct command{
	command_item *items;
	unsigned int items_num;
	int cmd_type;
	int cmd_class;
	int is_compl;
	int ns;
};

struct command_item{
	char type;				// Ya token (1), ya komanda (2) bolup bilyar
	token tok;
	command cmd;
};


extern void init_cmd(command *cmd, char free_items);

extern int recognize_cmd(command *cmd);

// Her komanda gornushini barlayan komanda
typedef struct{
	int (*is_cmd) (command *cmd);
}is_cmd_item;

extern is_cmd_item cmd_types[];

// Komandalaryn sany
extern int CMDS_TYPES_NUM;

extern char *cmd_classes[];

#define CONST_MAX_CLASS_TYPES 2
extern char *cmd_class_types[][CONST_MAX_CLASS_TYPES];

// Komandanyn bashynda bolup bilyan tokenlerin sany
extern int CMD_FIRST_ITEMS_NUM;

// Komanda-da bolup biljek tokenlerin sanawynda ulanylyan tip
typedef struct{
	int token_class;
	int token_type;
	int is_required;
}cmd_token;


// Komanda-da bolup biljek birlikleri tanamak uchin ulanylyan tip
typedef struct{
	char type;
	int  item_class;
	int  item_type;
}command_item_recog;

extern command_item_recog cmd_first_items_classes[];

int parse_cmd(command *cmd);

int is_cmd_def_var(command *cmd);
int is_cmd_assign(command  *cmd);

int is_tok_in_list(cmd_token *list_tok, token *tok);

int def_var_cmd_mod(command *cmd, token *tok, int tok_num);//command *cmd, token *tok, int tok_num);
int assign_cmd_mod(command *cmd, int tok_num);

int add_to_cmd(command *cmd, token *tok);

// Komandanyn gornushinin global ulnidini yglan etmedigini barlayar
int is_glob_def_var_cmd(command *cmd);

// Komandanyn gornushinin ulni yglan etmedigini barlayar
int is_def_var_cmd(command *cmd);

// Komandalar bilen ishleyan bolum
int work_with_cmd();


#endif
