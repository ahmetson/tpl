/** Programmadaky awchar_t sozler */
#include "token_types/ctrl_sttmnt.h"
#include "token_types/loop_sttmnt.h"
#include "token_types/utype.h"
#include "keywords.h"
#include "token_types/block_inside.h"

int MAX_KEYWORDS_NUM = 13; /// 13 sany açar söz ulanylýar

wchar_t *keywords[] = {
	L"san",					/// tipler
	L"harp",
	L"harpl",
	L"drob",
	L"umumy",				/// global
	IF_WORD,    			/// ctrl_sttmnt
	IFELSE_WORD,
	ELSE_WORD,
	CTRL_STTMNT_CLOSE_BLOCK,
	UNTIL_WORD,             /// loop_sttmnt
	UNTIL_CLOSE_WORD,
	UTYPE_DEF_SIGNIFIER_WORD,/// user_type
	TOK_CLASS_BLOCK_INSIDE_FN_RETURN_CHAR /// block_inside: return
};
