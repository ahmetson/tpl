/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler
**/
#ifndef TPL_GLOB_H
#define TPL_GLOB_H

#include "files.h"
#include "../cmds.h"
#include "../algor.h"

typedef struct{
    char inf_char;                          // Komandanyň birinji harpy.
    int  inf_char_pos;                      // Komandanyň birinji harpynyň setir boýunça nomeri
    int  inf_line;                          // Komandanyň faýldaky setiriniň nomeri
    char inf_file_name[CONST_MAX_FILE_LEN]; // Komandanyň duşýan faýly

    command_item left;                      // Komandanyň çepki maglumat birligi
    token_type   right;                     // Komandanyň sagky maglumat birliginiň görnüşi

    int cmd_class;
    int cmd_type;
}right_data_cmd_item;

extern char            CMD_END;

extern char            CUR_FILE_NAME[CONST_MAX_FILE_LEN];
extern int             CUR_FILE_NUM;
extern char            CUR_CHAR;
extern unsigned int    CUR_LINE;
extern int             CUR_PART;


// Häzirki ýasalyp duran komanda
extern command cmd;

// Ýasaljak faýl boýunça yglan edilen ülňileriň sanawy.
extern int             USER_VAR_DEFS_NUM;
extern var_def_item   *USER_VAR_DEFS;

extern long            UNKNOWN_USED_VARS_SIZE;
extern int             UNKNOWN_USED_VARS_NUM;
extern var_def_item   *UNKNOWN_USED_VARS;


extern command        *CUR_ALGOR;
extern long            CUR_ALGOR_SIZE;
extern int             CUR_ALGOR_ITEMS_NUM;


// Komandanyň içinde bolýan komandalaryň birliklerine, ýatda emeli usulda ýer berilýär.
extern command_item  **GLOB_SUBCMD_ITEMS_LIST;
extern unsigned int    GLOB_SUBCMDS_NUM;

// Iki sany maglumat birliklerden we ikinji birligi konstanta maglumat bolan komandany
// barlamak üçin, şeýle komandalaryň sanawy.
right_data_cmd_item    *GLOB_RIGHT_DATA_CMDS_LIST;
unsigned int            GLOB_RIGHT_DATA_CMDS_NUM;

extern file_item *FILES;

// TPL'den cykmazdan öňürti, programmanyň eýelän ýatdaky ýerlerini şu iki funksiýa arkaly boşadylýar.
void free_globs(void);
void free_locals(void);

#endif // TPL_GLOB_H
