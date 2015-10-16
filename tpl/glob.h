/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler
**/
#ifndef TPL_GLOB_H
#define TPL_GLOB_H

#include "files.h"
#include "../cmds.h"
#include "../algor.h"

extern char            CMD_END;

extern char            CUR_FILE_NAME[CONST_MAX_FILE_LEN];
extern int             CUR_FILE_NUM;
extern char            CUR_CHAR;
extern unsigned int    CUR_LINE;
extern int             CUR_PART;


// Häzirki ýasalyp duran komanda
extern command cmd;





extern long            GLOB_VAR_DEFS_SIZE;
extern int             GLOB_VAR_DEFS_NUM;
extern global_def_var *GLOB_VAR_DEFS;


extern long            UNKNOWN_USED_VARS_SIZE;
extern int             UNKNOWN_USED_VARS_NUM;
extern global_def_var *UNKNOWN_USED_VARS;

extern long            LOCAL_VAR_DEFS_SIZE;
extern int             LOCAL_VAR_DEFS_NUM;
extern local_def_var * LOCAL_VAR_DEFS;


extern command        *CUR_ALGOR;
extern long            CUR_ALGOR_SIZE;
extern int             CUR_ALGOR_ITEMS_NUM;


// Komandanyň içinde bolýan komandalaryň birliklerine, ýatda emeli usulda ýer berilýär.
command_item         **GLOB_SUBCMD_ITEMS_LIST;
unsigned int           GLOB_SUBCMDS_NUM;


extern file_item *FILES;

// TPL'den cykmazdan öňürti, programmanyň eýelän ýatdaky ýerlerini şu iki funksiýa arkaly boşadylýar.
void free_globs(void);
void free_locals(void);

#endif // TPL_GLOB_H
