#ifndef CMDS_H
#define CMDS_H

#define CONST_CMD_MAX_ITEMS 4
/*
COMMAND TYPES AND CLASSES ARE:
	1. var class
		1. decl
*/


// Komandanyn birliginin nomeri
extern const int UNKNOWN_ITEM;
extern const int TOKEN_ITEM;
extern const int CMD_ITEM;
extern const int PAREN_ITEM;

extern const int DEF_VAR_MAX_TOKENS;
extern const int ASSIGN_MAX_ITEMS;
extern const int FN_CALL_MAX_ITEMS;

extern int CMD_CLASS_UNKNOWN;
extern int CMD_CLASS_DEF_VAR;
extern int CMD_CLASS_ASSIGN;
extern int CMD_CLASS_FN;

extern int GLOB;
extern int LOCAL;

typedef struct command_item command_item;
typedef struct command command;


// Bashga komandalar ya tokenler bolup bilyan komanda
struct command{
	command_item *items;
	unsigned int items_num;
	int cmd_type;
	int cmd_class;
	int value_class;
	int value_type;
	int is_compl;
	int ns;
	char parenthesis;
	char inf_file_name[];
};

#include "parenthesis.h"
#include "tokens.h"
struct command_item{
	char type;				// Ya token (1), ya komanda (2) ýa skobka (3) bolup bilyar
	token tok;
	command cmd;
	parenthesis paren;
};

extern void init_cmd(command *cmd, char free_items);

extern int recognize_cmd(command *cmd);

// Her komanda gornushini barlayan komanda
typedef struct{
	int (*is_cmd) (command *cmd);
}is_cmd_item;

extern is_cmd_item cmd_types[];

typedef struct{
    command *cmd;
    int cmd_class;
    int cmd_type;
    int waited_class;
    int waited_type;
}unknown_cmd;

// Komandalaryn sany
#ifndef CMDS_TYPES_NUM
#define CMDS_TYPES_NUM 3
#endif

#ifndef MAX_CLASS_TYPES
#define MAX_CLASS_TYPES 2
#endif

extern char *cmd_classes[];

#define CONST_MAX_CLASS_TYPES 2
extern char *cmd_class_types[][CONST_MAX_CLASS_TYPES];

// Komanda-da bolup biljek tokenlerin sanawynda ulanylyan tip
typedef struct{
	int token_class;
	int token_type;
	int is_required;
}cmd_token;


int parse_cmd(command *cmd);


int is_tok_in_list(cmd_token *list_tok, token *tok);

//command *cmd, token *tok, int tok_num);

int add_to_cmd(command *cmd, int type, token tok, parenthesis paren, command subcmd);

// Komandalar bilen ishleyan bolum
int work_with_cmd();

int cmd_add_subcmd(command *cmd, command addcmd);
int cmd_add_subparen(command *cmd, parenthesis paren);

int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type);
int empty_cmd_return_type(command *cmd, int *cmd_class, int *cmd_type);

// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd);
int empty_cmd_checking_semantic(command *cmd);

void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, char **l, int *len);
void empty_cmd_c_code(command *cmd, char **l, int *len);

void unknown_cmd_add(command *cmd, int cmd_class, int cmd_type, int waited_class, int waited_type);
#endif
