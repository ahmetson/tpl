#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
// TOKEN TIPLER: defined types, global, assign
#include "token/token_types/def_types.h"
#include "token/token_types/glob.h"
#include "token/token_types/assign.h"
#include "token/token_types/const_data.h"
#include "token/token_types/arif.h"
#include "token/token_types/cmp.h"
#include "token/token_types/logic.h"
#include "token/token_types/ctrl_sttmnt.h"
#include "token/token_types/block.h"
#include "token/token_types/loop_sttmnt.h"
#include "token/token_types/triangle_block.h"
#include "token/token_types/utype.h"
#include "token/token_types/utype_con.h"
#include "token/token_types/void.h"
#include "token/token_types/comment.h"
#include "fns/fn.h"
// Ähli token tiplerine degişli funksiýalar
#include "token/token_types.h"

// Tokenler bilen ulanylýan çylşyrymly, emeli tipleriň yglan edilmeleri
#include "token/token_structs.h"

extern is_token_item tok_types[];
                                                                    // TOKEN BILEN BAŞDA
void init_token(token *tok);                                        // 1) taýynlaýar
void empty_token(token *tok);                                       // 2) boşadýar
int  is_token_empty(token *tok);                                    // 3) boşdygyny barlaýar

                                                                    // TOKEN:
int recognize_token(token *tok, char *val);                         // 1)tanalýar,
int add_potentional_token_type(token *tok, token_type tok_type);    // 2)içine bolup biljek tipleri goshulyar
int finishize_token(token *tok);                                    // 3)komanda salmana taýynlanýar
#include "cmds.h"
int cmd_add_tok(token tok, command *cmd);                          // 4)Komanda salynýar

int work_with_token(token *tok, command *cmd);
token parse_token(FILE *s);

int (*TOK_RETURN_TYPE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, int *tok_class, int *tok_num);
int empty_tok_return_type(token *tok, int *tok_class, int *tok_num);


void (*TOK_GET_C_CODE[TOKEN_CLASSES_NUM][TOKEN_MAX_TYPES_NUM])(token *tok, char **l, int *llen);
void empty_tok_c_code(token *tok, char **l, int *llen);

token get_empty_tok();

int return_tok_type(token *tok, int *ret_class, int *ret_type);

#endif
