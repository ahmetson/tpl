#ifndef TPL_H
#define TPL_H

#include "algor.h"
#include "cmds.h"

#define CONST_MAX_FILE_LEN  255
#define CONST_MAX_TOKEN_LEN 32

extern char            CMD_END;

extern char            CUR_FILE_NAME[CONST_MAX_FILE_LEN];
extern char            CUR_CHAR;
extern unsigned int    CUR_LINE;
extern int             CUR_PART;

extern char            MAIN_FILE_NAME[CONST_MAX_FILE_LEN];

// Häzirki ýasalyp duran komanda
extern command cmd;

extern long            GLOB_VAR_DEFS_SIZE;
extern int             GLOB_VAR_DEFS_NUM;
extern global_def_var *GLOB_VAR_DEFS;

extern long            LOCAL_VAR_DEFS_SIZE;
extern int             LOCAL_VAR_DEFS_NUM;
extern local_def_var * LOCAL_VAR_DEFS;

extern command        *CUR_ALGOR;
extern long            CUR_ALGOR_SIZE;
extern int             CUR_ALGOR_ITEMS_NUM;

// Komandanyň içinde bolýan komandalaryň birliklerine, ýatda emeli usulda ýer berilýär.
command_item         **GLOB_SUBCMD_ITEMS_LIST;
unsigned int           GLOB_SUBCMDS_NUM;

// TPL'den cykmazdan öňürti, programmanyň eýelän ýatdaky ýerlerini şu iki funksiýa arkaly boşadylýar.
void free_globs(void);
void free_locals(void);
#endif
