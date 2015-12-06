/**
 * Global yglan edilen, bütin faýl boýunça ulanylýan üllňiler
**/
#ifndef TPL_GLOB_H
#define TPL_GLOB_H

#include "files.h"
#include "../cmds.h"
#include "../algor.h"
#include "../parenthesis.h"
#include "../fns/fn.h"
#include "../semantic.h"

extern char CHAR_UNDERSCORE;


typedef struct{
    char  (*inc)[MAX_FILE_LEN];
    int    num;
}file_incs;

typedef struct{
    char  (*ident)[MAXLEN];
    int    num;
}called_var;

extern int             TEST;

extern char            CMD_END;

extern char            CUR_FILE_NAME[MAX_FILE_LEN];
extern int             CUR_FILE_NUM;
extern char            CUR_CHAR;
extern int             CUR_CHAR_POS;
extern unsigned int    CUR_LINE;
extern int             CUR_PART;


// Häzirki ýasalyp duran komanda
extern command cmd;
extern token   inf_tok;


extern command        *CUR_ALGOR;
extern long            CUR_ALGOR_SIZE;
extern int             CUR_ALGOR_ITEMS_NUM;


// Komandanyň içinde bolýan komandalaryň birliklerine, ýatda emeli usulda ýer berilýär.
extern command_item  **GLOB_SUBCMD_ITEMS_LIST;
extern unsigned int    GLOB_SUBCMDS_NUM;

// Global harpl ülňileriniň sanawy. Olaryň uzynlyklary näbelli bolup durýar.
extern char                  **GLOB_STRINGS;
extern unsigned long           GLOB_STRINGS_NUM;


/** Kodlarda ulanylýan skobkalarda (parenthesis), näçe sany element boljagy näbelli.
    Şonuň üçin olar kuça-da (heap) ýerleşdirilýär.
**/
extern parenthesis_elem      **GLOB_PARENTHS;
extern int                     GLOB_PARENTHS_NUM;


/**
 * Programmadaky kodlaryň setirleri.
**/
extern char          ***GLOB_SOURCE_CODES;

extern file_item           *FILES;

/** Programmadaky goldanylýan funksiýalar
**/
extern func                *FUNCS;
extern int                  FUNCS_NUM;

/** Programmadaky goldanylýan funksiýalaryň argumentleri
**/
extern func_arg           **FUNC_ARGS;

/// Goşmaly faýllaryň sanawy
extern file_incs           *INCLUDES;
extern int                  INCLUDES_NUM;

/// 'A' identifikatoryň 'B', we 'B' identifikatoryň 'A' baglanmagyny saklaýar
/*extern compare_ident       *COMPARE_IDENTS;
extern int                  COMPARE_IDENTS_NUM;*/

/// Ýasaljak programma boýunça global yglan edilen ülňileriň sanawy
extern glob_ident          *GLOBAL_VAR_DEFS;
extern int                  GLOBAL_VAR_DEFS_NUMS;

/// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen ülňileriň maglumatlarynyň sanawy
extern glob_ident          *GLOBAL_VAR_DECS;
extern int                  GLOBAL_VAR_DECS_NUMS;

/// Global yglan etmeli faýllaryň sanawy
extern char               **GLOB_DECS_FILES;
extern int                  GLOB_DECS_FILES_NUM;

/// Ýasaljak kodly faýlda yglan edilen ülňileriň maglumatlarynyň sanawy
extern glob_ident          *LOCAL_VAR_DEFS;
extern int                  LOCAL_VAR_DEFS_NUMS;

/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
extern called_var          *GLOBAL_CALLED_VARS;
extern int                  GLOBAL_CALLED_VARS_NUM;

/// Häzirki içine parsing edilip goşulýan birlikli komanda
command                    *CUR_CMD;

command_item               *MAIN_CMD_ITEMS_LIST;
command_item               *TMP_CMD_ITEMS_LIST;

/// Bloklaryň içindeligini görkezýän ülňi
extern int                  GLOB_BLOCK_INCLUDES;

/// Bloklaryň içindeliginiň sanawy
extern block_inc           *GLOB_BLOCKS;
extern int                  GLOB_BLOCKS_NUM;

// TPL'den cykmazdan öňürti, programmanyň eýelän ýatdaky ýerlerini şu iki funksiýa arkaly boşadylýar.
void free_globs(void);
void free_locals(void);

#endif // TPL_GLOB_H
