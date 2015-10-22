#ifndef TOKENS_H
#define TOKENS_H

// TOKEN TIPLER: defined types, global, assign
#include "token/token_types/def_types.h"
#include "token/token_types/glob.h"
#include "token/token_types/assign.h"
#include "token/token_types/const_data.h"
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
int move_to_cmd(token *tok, char *tok_word);                        // 4)Komanda salynýar

#endif
