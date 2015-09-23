#ifndef TPL_H
#define TPL_H

#include "algor.h"
#include "cmds.h"

#define MAX_FILE_LEN 255
#define MAX_TOKEN_LEN 32

extern char cur_parse_file_name[MAX_FILE_LEN];	
extern char cur_parse_char;
extern unsigned int cur_parse_line_num;

// Hazir TPL-in haysy bolumi kod bilen ishleyar
extern int CUR_PART;

// Translyator arkaly yasalan programmadaky esasy fayl
extern char main_file_name[MAX_FILE_LEN];

//char *tpl(int argc, const char **args);

/*typedef struct{
	token lvalue;
	token op;
	token rvalue;
	char cmd_type;
}cmd;*/

extern char cmd_end;

// Parser edilyan komanda
extern command cmd;
extern command prev_cmd;

// Yglan edilen global ulnilerin sany
extern long glob_def_vars_size;
extern int glob_def_vars_cmds;
extern global_def_var *glob_def_vars;

// Hazirki maglumatlar uchin
extern long loc_def_vars_size;
extern int loc_def_vars_num;
extern local_def_var *loc_def_vars; 

// Parsing edilyan faylyn algoritmini yatda saklamak uchin
extern command *cur_file_algor;
extern long cur_file_algor_size;
extern int cur_file_algor_cmds;

// Programmadan chykmazdan onurti chagyrylmaly,
// Hemme programmanyn ulanyan yadyny boshadyar.
void free_globs(void);
void free_locals(void);


#endif
