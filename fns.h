#ifndef FNS_H
#define FNS_H

// Concatinate string with char and put them to @to
void strcat_ch(token_word *to, token_word *from, char c);

// Checks first occurance of substring in string started from offset
int strstr_by_offset(const char *string, const char*sub, unsigned int offset);

// Soze soz bilen harpy goshyar
void strcat_ch2(char **to, char **from, char c);

// SISTEMA BILEN ISHLEYAN ULNILER
// Taze papka yasayar
// Eger ikinji argument dogry bolsa,
// 	Onki papkany pozyar.
int sys_mkdir(char *folder_name, char rm_prev_dir);
// Onki papkany pozyar
int sys_rmdir(char *folder_name)


char *remove_ext(char *f, char *e);
#endif
