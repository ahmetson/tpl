/*
 * Programma dilde global maglumatlar uchin ulanylyan maglumatlar
**/
#include <wchar.h>
#include "glob.h"

/* token NAMESPACES
 *
 * glob
 * loc
**/

// Global maglumatlar, onunde durant yorite token arkaly tanalyarlar.
// Token bolsa programma dilde rezerw edilen sozlerin biri
wchar_t *glob_def_string = L"@";

wchar_t GLOB_IDENT_OPENER = L'@';
