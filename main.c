#include <stdlib.h>
#include "tpl.h"

#define EXIT return 0
//#define EXIT system("pause"); return 0


//int        argc - Argumentlerin sanawynda elemntleriň sany. Programmanyň öz adyndan başlanýar
//const char argv - Argumentleriň sanawy
int main(int argc, const char **args, const char **argv)
{
    system("chcp 65001 > nul");

	tpl(argc, args);

	EXIT;
}
