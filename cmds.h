#ifndef CMDS_H
#define CMDS_H
#include <wchar.h>
#include <wctype.h>

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
extern int CMD_CLASS_FN_CALL;
extern int CMD_CLASS_CALL_GLOB_VAR;
extern int CMD_CLASS_ARIF;
extern int CMD_CLASS_CMP;
extern int CMD_CLASS_LOGIC;
extern int CMD_CLASS_CTRL_STTMNT;
extern int CMD_CLASS_BLOCK;
extern int CMD_CLASS_LOOP_STTMNT;
extern int CMD_CLASS_ARR;
extern int CMD_CLASS_UTYPE;
extern int CMD_CLASS_FN_DEF;
extern int CMD_CLASS_CONV_BASIC_TYPES;

#ifndef CONST_CMD_CLASS_UTYPE
#define CONST_CMD_CLASS_UTYPE  12
#define CONST_CMD_CLASS_FN_DEF 13
#endif // CONST_CMD_CLASS_UTYPE

extern int GLOB;
extern int LOCAL;

typedef struct command_item command_item;
typedef struct command command;

#ifndef TPL_MAX_ITEMS
#define TPL_MAX_ITEMS 5
#define MAX_FOUR_ITEMS 4
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
	wchar_t parenthesis;
	wchar_t inf_file_name[];
};// Bashga komandalar ya tokenler bolup bilyan komanda


#include "parenthesis.h"
#include "tokens.h"
struct command_item{
	wchar_t type;				// Ya token (1), ya komanda (2) ýa skobka (3) bolup bilyar
	token tok;
	command cmd;
	parenthesis paren;
};

extern void init_cmd(command *cmd, wchar_t free_items);

extern int recognize_cmd(command *cmd);

// Her komanda gornushini barlayan komanda
typedef struct{
	int (*is_cmd) (command *cmd);
}is_cmd_item;

extern is_cmd_item cmd_types[];


// Komandalaryn sany
#ifndef CMDS_TYPES_NUM
#define CMDS_TYPES_NUM 14
#endif

#ifndef MAX_CLASS_TYPES
#define MAX_CLASS_TYPES 3
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
int work_with_cmd( command *cmd );


int (*CMD_RETURN_TYPE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, int *cmd_class, int *cmd_type);
int empty_cmd_return_type(command *cmd, int *cmd_class, int *cmd_type);

// Komandanyň klasy we tipi boýunça semantikasyny barlaýan funksiýalar
int (*CMD_CHECK_SEMANTICS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd);
int empty_cmd_checking_semantic(command *cmd);

int CMD_MAX_ITEMS[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES];

void (*CMD_GET_C_CODE[CMDS_TYPES_NUM+1][MAX_CLASS_TYPES])(command *cmd, wchar_t **l, int *len);
void empty_cmd_c_code(command *cmd, wchar_t **l, int *len);

command get_empty_cmd();

int is_cmd_item_compl(command_item *ci);
int is_paren_compl(parenthesis *p);

int subcmd_items_add(unsigned int items_num);


int is_glob_decl_support_cmd(command *cmd);
void glob_vars_decl_add(command *cmd);
void glob_arrs_decl_add(command *cmd);
int is_glob_var_dec_exist(wchar_t *ident);
void get_glob_var_dec_value_type(wchar_t *ident, int *c, int *t);
void cmd_wrapper_c_code(wchar_t **line, int *llen);
void cmd_block_wrapper_c_code(wchar_t **line, int *llen);

command_item *get_cmd_item(int cmd_num, int item_num);
int change_cmd_items_num(int cmd_num, int new_num);
void put_cmd_item(int cmd_num, int item_num, command_item ci);

int is_cmd_not_compl_item_exist(command *cmd, wchar_t rec);

int is_cmd_item_can_be_needed(command *cmd);

int is_glob_arr_dec_exist(wchar_t *ident);

void make_cmd_copy(command *cmd, command *cmd_out);

void glob_fns_decl_add(command *cmd);

void  work_with_glob_var_decs();
void  work_with_glob_arr_decs();
void work_with_glob_fn_decs();


int    *get_op_positions(command *cmd);
int     get_op_prior(int cmd_items, int op_position);

int    get_mem_for_cmd_items();

void    move_cmd_items ( int orig_pos, int start_pos, command *cmd );

void find_most_prior_op( command *cmd, int *op_positions, int *pos, int *lvl );
int is_op_require_left_data( int pos );
int is_op_require_right_data( int pos );

void    minimize_operands( command *cmd, int *op_positions );
void    move_cmd_items ( int orig_pos, int start_pos, command *cmd );
int     make_subcmd_from_op(command *cmd, int *op_position);
int is_cmd_item_op( command_item *ci );

int     get_prev_op_pos( int *pos, int cur_pos );
int     get_next_op_pos( int *pos, int cur_pos );

#endif
