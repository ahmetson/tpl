/*
 * Harplardan yasalyan geljekki tokenin teksti
**/
#ifndef TOKEN_WORD_H
#define TOKEN_WORD_H

// Token types fayly, token uchin yasalan token strukturalaryny hem chagyryar
#include "token_types.h"

extern void init_token_word(token_word *t);

extern void set_token_word_value(token_word *tok_word, char c);

#endif
