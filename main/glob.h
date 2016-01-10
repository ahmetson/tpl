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
#include "../cmd/array.h"

extern wchar_t CHAR_UNDERSCORE;
extern wchar_t TMP_CHAR;

typedef struct{
    wchar_t  (*inc)[MAX_FILE_LEN];
    int    num;
}file_incs;

typedef struct{
    wchar_t  (*ident)[MAXLEN];
    int    num;
}called_var;

extern int             TEST;

extern wchar_t            CMD_END;

extern wchar_t            CUR_FILE_NAME[MAX_FILE_LEN];
extern int             CUR_FILE_NUM;
extern wchar_t            CUR_CHAR;
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
extern wchar_t                  **GLOB_STRINGS;
extern unsigned long           GLOB_STRINGS_NUM;


/** Kodlarda ulanylýan skobkalarda (parenthesis), näçe sany element boljagy näbelli.
    Şonuň üçin olar kuça-da (heap) ýerleşdirilýär.
**/
extern parenthesis_elem      **GLOB_PARENTHS;
extern int                     GLOB_PARENTHS_NUM;


/**
 * Programmadaky kodlaryň setirleri.
**/
extern wchar_t          ***GLOB_SOURCE_CODES;

extern file_item           *FILES;

/** Programmadaky goldanylýan funksiýalar
**/
extern func                *FUNCS;
extern int                  FUNCS_NUM;

/** Programmadaky goldanylýan funksiýalaryň argumentleri
**/
extern func_arg           **FUNC_ARGS;

/** Funksiýalar TPL derejede mümkin. Şonuň üçin kuçada ýerleşdirilýär **/
extern func                   TMP_FUNC;
extern int                    TMP_FUNC_NUM;
extern command               *TMP_FUNC_PARAMS;
extern command               *TMP_FUNC_ALGOR;
extern int                    TMP_FUNC_ALGOR_NUM;
extern glob_ident            *TMP_FUNC_VARS;
extern int                    TMP_FUNC_VARS_NUM;
extern array_item            *TMP_FUNC_ARRS;
extern int                    TMP_FUNC_ARRS_NUM;
extern array_inc_item       **TMP_FUNC_ARRS_ITEMS;


/** Diňe parsing edilip duran faýlyň döwründe işleýär.
    Diňe parsing edilen faýlyň içinde yglan edilen funksiýalaryň parametrleri **/
extern int                    LOC_FUNCS_NUM;
extern func                  *LOC_FUNCS;
extern command              **LOC_FUNCS_PARAMS;
extern command              **LOC_FUNCS_ALGOR;
extern int                   *LOC_FUNCS_ALGOR_NUM;
extern glob_ident           **LOC_FUNCS_VARS;
extern int                   *LOC_FUNCS_VARS_NUM;
extern array_item           **LOC_FUNCS_ARRS;
extern int                   *LOC_FUNCS_ARRS_NUM;
extern array_inc_item      ***LOC_FUNCS_ARRS_ITEMS;


/** Beýan edilýän faýlda yglan edilen funksiýalaryň maglumatlary **/
extern func                  *DEC_FUNCS;
extern int                    DEC_FUNCS_NUM;

extern func_arg             **DEC_FUNC_ARGS;

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
extern wchar_t               **GLOB_DECS_FILES;
extern int                  GLOB_DECS_FILES_NUM;

/// Ýasaljak kodly faýlda yglan edilen ülňileriň maglumatlarynyň sanawy
extern glob_ident          *LOCAL_VAR_DEFS;
extern int                  LOCAL_VAR_DEFS_NUMS;

/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
extern called_var          *GLOBAL_CALLED_VARS;
extern int                  GLOBAL_CALLED_VARS_NUM;

/// Ýasaljak programma boýunça global yglan edilen birsyhly sanawlaryň sanawy
extern array_item          *GLOBAL_ARR_DEFS;
extern int                  GLOBAL_ARR_DEFS_NUMS;

/// Ýasaljak programma boýunça soň çagyrylyp bilinjek global yglan edilen birsyhly sanawlaryň maglumatlarynyň sanawy
extern array_inc_item     **GLOBAL_ARR_DEFS_ITEMS;

extern array_item          *GLOBAL_ARR_DECS;
extern int                  GLOBAL_ARR_DECS_NUMS;

extern array_inc_item     **GLOBAL_ARR_DECS_ITEMS;

/// Ýasaljak programma boýunça global yglan edilen birsyhly sanawlaryň sanawy
extern array_item          *LOCAL_ARR_DEFS;
extern int                  LOCAL_ARR_DEFS_NUMS;

extern array_inc_item     **LOCAL_ARR_DEFS_ITEMS;


/// Ýasaljak kodlarda çagyrylan global ülňileriň sanawy.
/// C translator global ülňileriň çagyrylan faýlyna, çagyrylan global ülňiniň yglan edilen .h faýlyny goşmaly.
extern called_var         *GLOBAL_CALLED_ARRS;
extern int                 GLOBAL_CALLED_ARRS_NUM;


/// Häzirki içine parsing edilip goşulýan birlikli komanda
command                    *CUR_CMD;

command_item               *MAIN_CMD_ITEMS_LIST;
command_item               *TMP_CMD_ITEMS_LIST;

/// Bloklaryň içindeligini görkezýän ülňi
extern int                  GLOB_BLOCK_INCLUDES;

/// Bloklaryň içindeliginiň sanawy
extern block_inc           *GLOB_BLOCKS;
extern int                  GLOB_BLOCKS_NUM;

extern char                **PRINTED_STRINGS;
extern int                 PRINTED_STRINGS_NUM;

// TPL'den cykmazdan öňürti, programmanyň eýelän ýatdaky ýerlerini şu iki funksiýa arkaly boşadylýar.
void free_globs(void);
void free_locals(void);
void free_tmp_fn();
void free_local_fns();


#endif // TPL_GLOB_H
