#ifndef FNS_H
#define FNS_H
#include <stdio.h>
#include "main/files.h"

// Concatinate string with wchar_t and put them to @to
void wcsstrchcat( wchar_t *to, wchar_t *from, wchar_t c );

void str_add_char( wchar_t *to, wchar_t c );

// Checks first occurance of substring in string started from offset
int wcsstr_by_offset(const wchar_t *string, const wchar_t*sub, unsigned int offset);

void str_add_char( wchar_t *to, wchar_t c );


// SISTEMA BILEN ISHLEYAN ULNILER
// Taze papka yasayar
// Eger ikinji argument dogry bolsa,
// 	Onki papkany pozyar.
int sys_mkdir(wchar_t *folder_name, wchar_t rm_prev_dir);
// Onki papkany pozyar
int sys_rmdir(wchar_t *folder_name);

/**
 * Berlen sozun bolmaly uzynlygyndaky,
 * ichindaki hemme harplaryn deregine \0 bilen dolduryar.
 *
 * Eger-de bolmaly uzynlygy tanalmasy, yagny argument -1 bolsa,
 * ol wcslen() funksiyasy arkaly tanalyar.
 *   Shonda dine sozde birinji \0 dushyancha hemme harplaryn deregine \0 goyular
 */
wchar_t *empty_string(wchar_t *f, int len);

wchar_t *remove_dirnames(wchar_t *f);
wchar_t *remove_ext(wchar_t *f, wchar_t *e);

void divide_string(wchar_t *source, wchar_t d, wchar_t ***out, int *items);

/** Harplyň daşynda duran goşa dyrnaklary aýyryp, yzyna gaýtarýar.

    @withKuotes - daşy goşa dyrnakly tekst,
    @unKutoed   - goşa dyrnaklar aýrylan görnüşi (Harplaryň sanawynyň uzynlygy @withKuotes sanawynyňky ýaly bolmaly)*/
void string_helper_remove_dquotes(wchar_t *unquoted, wchar_t *with_quotes);

int count_bytes( wchar_t *wstr );
int get_null_size();
int add_wchar_to_heap( wchar_t **mem, int *memlen, wchar_t *add );
int wcsncat_on_heap( wchar_t **mem, int *memlen, wchar_t *add );
int wcsncpy_on_heap( wchar_t **mem, int *memlen, wchar_t *add );
int wcsadd_on_heap( wchar_t **mem, int *memlen, wchar_t *add );
int realloc_wchar_heap( wchar_t **mem, int *memlen, wchar_t *add );
int realloc_wchar_null_heap( wchar_t **mem, int *memlen );


int wcsncpys( wchar_t *d, wchar_t *s );
int is_wcseq( wchar_t *s1, wchar_t *s2 );

int sys_is_dir_exists( wchar_t folder_name[ MAX_FILE_LEN ] );

void    str_change_char( wchar_t *s, wchar_t c, wchar_t with );

#endif
