#include "tpl.h"

#define TPL_DEBUG_MODE

#ifdef TPL_DEBUG_MODE
    #include <stdlib.h>
    #define EXIT system("pause"); return 0
#else
    #define EXIT return 0
#endif

int main(int argc, const char **args)
{
    /// Main function of TPL. It's a start point.
    /// Arguments from CMD and their number are required.
	tpl(argc, args);

	EXIT;
}
