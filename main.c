#include "tpl.h"

//#define TPL_DEBUG_MODE

#ifdef TPL_DEBUG_MODE
#include <stdlib.h>
#define EXIT system("pause"); return 0
#else
#define EXIT return 0
#endif


int main(int argc, const char **args)
{
    /// TPl baş funksiýasy hökmän programma gelen argumentleri we olaryň sanyny kabul etmeli.
	tpl(argc, args);

	EXIT;
}
