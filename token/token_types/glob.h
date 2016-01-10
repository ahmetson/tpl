/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#ifndef GLOB_H
#define GLOB_H
#include <wchar.h>

/* token NAMESPACES
 *
 * glob
 * loc
**/

extern wchar_t GLOB_IDENT_OPENER;

// Global maglumatlar, onunde durant yorite token arkaly tanalyarlar.
// Token bolsa programma dilde rezerw edilen sozlerin biri
extern wchar_t *glob_def_string;

#endif
