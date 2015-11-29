#ifndef CMDS_H
#define CMDS_H

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
extern int CMD_CLASS_CALL_GLOB_VAR;
extern int CMD_CLASS_ARIF;
extern int CMD_CLASS_CMP;
extern int CMD_CLASS_LOGIC;

extern int GLOB;
extern int LOCAL;

typedef struct command_item command_item;
typedef struct command command;

#ifndef TPL_MAX_ITEMS
#define TPL_MAX_ITEMS 4
#define MAX_THREE_ITEMS 3
#define MAX_TWO_ITEMS 2
#define MAX_ONE_ITEMS 1
#define MAX_ZERO_ITEMS 0
#define MAX_NO_ITEMS -1
#endif

// Bashga komandalar ya tokenler bolup bilyan komanda
struct command{
	int items;         /// Komandadaku birlikleri saklaýan global sanawdaky komandanyň nomeri
	unsigned int items_num;
	int cmd_type;
	int cmd_class;
	int value_class;
	int value_type;
	int is_compl;
	int ns;
	char parenthesis;
	char inf_file_name[];
};// Bashga komandalar ya tokenler bolup bilyan komanda


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


// Komandalaryn sany
#ifndef CMDS_TYPES_NUM
#define CMDS_TYPES_NUM 7
#endif

#ifndef MAX_CLASS_TYPES
#define MAX_CLASS_TYPES 2
#endif

extern char *cmd_classes[];

extern char *cmd_class_types[][MAX_CLASS_TYPES];

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

int cmd_add_item(command *cmd, int item_type, parenthesis p, command c, token t);

int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type);
int empty_cmd_return_type(command *cmd, int *cmd_class, int *cmd_type);

// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd);
int empty_cmd_checking_semantic(command *cmd);

int CMD_MAX_ITEMS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES];

void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, char **l, int *len);
void empty_cmd_c_code(command *cmd, char **l, int *len);

command get_empty_cmd();

int is_cmd_item_compl(command_item *ci);
int is_paren_compl(parenthesis *p);

int subcmd_items_add(unsigned int items_num);


int is_glob_decl_support_cmd(command *cmd);
void glob_vars_decl_add(command *cmd);
int is_glob_var_dec_exist(char *ident);
void get_glob_var_dec_value_type(char *ident, int *c, int *t);
void cmd_wrapper_c_code(char **line, int *llen);

command_item **main_cmd_add(unsigned int items_num);
command_item *get_cmd_item(int cmd_num, int item_num);
int change_cmd_items_num(int cmd_num, int new_num);
void put_cmd_item(int cmd_num, int item_num, command_item ci);

int is_cmd_not_compl_item_exist(command *cmd, char rec);

int is_cmd_item_can_be_needed(command *cmd);

#endif
