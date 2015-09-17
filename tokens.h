#ifndef TOKENS_H
#define TOKENS_H


// Token tiplerinin her bir gornushinin dine ozune gerekli maglumatlar
#include "token/def_types.h"
#include "token/glob.h"
// Umumy we ahli token tiplerine degishli funksiyalar
#include "token/token_types.h"
// Tokenler uchin strukturalar arkaly yasalan tipler
#include "token/token_structs.h"

#include "token/token_word.h"

// Initialized the token before using
// Sets default parameters
extern void init_token(token *tok);

extern void empty_token(token *tok);

extern is_token_item tok_types[];

// FNS
int recognize_token(token *tok, token_word *val);


int add_potentional_token_type(token *tok, token_type tok_type);

int finishize_token(token *tok);

int is_token_empty(token *tok);

int move_to_cmd(token *tok, token_word *tok_word);


#endif
