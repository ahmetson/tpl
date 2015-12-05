/*
 * Programmadaky achar sozler
**/
#include "token_types/ctrl_sttmnt.h"
#include "keywords.h"

int MAX_KEYWORDS_NUM = (4) + 1 + (4);
char *keywords[] = {
	"san",					// tipler
	"harp",
	"harpl",
	"drob",
	"umumy",				// global
	IF_WORD,    			// ctrl_sttmnt
	IFELSE_WORD,
	ELSE_WORD,
	CTRL_STTMNT_CLOSE_BLOCK
};
