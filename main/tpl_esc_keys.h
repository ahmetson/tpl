/**
 * TPL programma-da ulanylýan Speşl açarlar.
**/
#ifndef MAIN_TPL_ESC_KEYS_H
#define MAIN_TPL_ESC_KEYS_H

#define CONST_TPL_ESC_KEYS 5

extern char TPL_ESC_KEYS_LIST[CONST_TPL_ESC_KEYS][2];

extern short int TPL_ESC_KEY_MAX_LEN;
extern char      TPL_ESC_KEY_OPENER;
extern char      TPL_ESC_KEY_C_OPENER;

int is_tpl_ESC_key(char ESC_key, char type);
int tpl_ESC_key_to_c_ESC_key(char *val);

#endif // MAIN_TPL_ESC_KEYS_H
