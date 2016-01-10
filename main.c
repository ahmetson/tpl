#include <stdlib.h>
#include <locale.h>
#include "tpl.h"

#define TPL_DEBUG_MODE
#define EXIT return 0
//#define EXIT system("pause"); return 0


/** int        argc - Argumentlerin sanawynda elemntleriň sany. Programmanyň öz adyndan başlanýar
    const wchar_t argv - Argumentleriň sanawy*/
int main(int argc, const char **args)
{
    char *tk = setlocale(LC_ALL, ".1250");
    system("chcp 65001 > nul");

	tpl(argc, args);

	EXIT;
}
