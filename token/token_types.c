/**
 * Tokenlerin tiplerinin bilen bagly ahli maglumatlar.
 * Eyyam bolup biljek tiplerin hemmesi yglan edilen son doredilmeli
**/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "token_types/assign.h"
#include "token_types/glob.h"
#include "token_types/def_types.h"
#include "token_types/const_data.h"
#include "token_types/arif.h"
#include "token_types/cmp.h"
#include "token_types/logic.h"
#include "token_types/ctrl_sttmnt.h"
#include "token_types/block.h"
#include "token_types/loop_sttmnt.h"
#include "token_types/triangle_block.h"
#include "token_types/utype.h"
#include "token_types/utype_con.h"
#include "token_types/void.h"
#include "token_types/comment.h"
#include "token_types/block_inside.h"
#include "keywords.h"
#include "token_types.h"
#include "../main/tpl_esc_keys.h"
#include "../main/user_def_type.h"
#include "../fns.h"
#include "../tokens.h"
#include "../main/glob.h"

const int TOK_CLASS_UNDEFINED       = -1;
const int TOK_CLASS_UNKNOWN         = 0;
const int TOK_CLASS_DEF_TYPE 	    = 1;
const int TOK_CLASS_GLOB 		    = 2;
const int TOK_CLASS_IDENT           = 3;
const int TOK_CLASS_ASSIGN		    = 4;
const int TOK_CLASS_CONST_DATA      = 5;
const int TOK_CLASS_ARIF            = 6;
const int TOK_CLASS_CMP             = 7;
const int TOK_CLASS_LOGIC           = 8;
const int TOK_CLASS_CTRL_STTMNT     = 9;
const int TOK_CLASS_BLOCK           = 10;
const int TOK_CLASS_LOOP_STTMNT     = 11;
const int TOK_CLASS_TRIANGLE_BLOCK  = 12;
const int TOK_CLASS_UTYPE           = 13;
const int TOK_CLASS_UTYPE_CON       = -2;
const int TOK_CLASS_VOID            = 15;
const int TOK_CLASS_COMMENT         = 16;
const int TOK_CLASS_BLOCK_INSIDE    = 17;

/// DEV_DEBUG.c arkaly debug'lamak üçin, token tipleriniň klaslarynyň atlary
char *dev_type_class_names[] = {
    "unknown",
	"def simple type",
	"global",
	"ident",
	"assign",
	"const data",
	"arifmetic",
	"compare",
	"logic",
	"control sttmnt",
	"block",
	"loop sttmnt",
	"triangle block",
	"user type",
	"user type connection",
	"empty type",
	"comment",
	"function and repeat block helpers"
};

