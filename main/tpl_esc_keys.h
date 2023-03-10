/**
 * TPL programma-da ulanylýan Speşl açarlar.
**/
#ifndef MAIN_TPL_ESC_KEYS_H
#define MAIN_TPL_ESC_KEYS_H

#define CONST_TPL_ESC_KEYS 5

#ifndef ESCAPE_IN_CHR
#define ESCAPE_IN_CHR 1
#define ESCAPE_IN_STR 0
#endif

extern wchar_t TPL_ESC_KEYS_LIST[CONST_TPL_ESC_KEYS][2];

extern short int TPL_ESC_KEY_MAX_LEN;
extern wchar_t      TPL_ESC_KEY_OPENER;
extern wchar_t      TPL_ESC_KEY_C_OPENER;

int is_tpl_ESC_key(wchar_t ESC_key, wchar_t type);
int tpl_ESC_key_to_c_ESC_key(wchar_t *val);

#endif // MAIN_TPL_ESC_KEYS_H
