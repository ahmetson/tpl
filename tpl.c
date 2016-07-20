/** Programma:  TPL (tepe'el diýip okalýar) ( Turkmen|turkic|testable programming language )

   Awtory:     Medet Atabayev[milayter@milayt.com]
   Wersýesi:  0.0.1
 **/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

#include "tpl.h"
#include "main/init.h"
#include "work_with_source.h"
#include "translator_to_c.h"
#include "dev_debug.h"
#include "error.h"
#include "semantic.h"
#include "main/create_exe.h"

void tpl(int argc, const char **args)
{
    /// Set UTF-8 as default encode for I/O.
    system("chcp 65001 > nul");

	/// TPL runs if there is source codes.
	source_codes_exist(argc);

	/// Prepare TPL:
	/// Initialize global variables.
	init();

	/// Create algorithm tree by TEPL source code
	work_with_sources(argc, args);

    /// After parsing TEPL source codes and building node tree, check interconnections between TEPL source codes
    work_with_semantic();

    /// Add additional C codes to C source codes
    async_source_codes();

    /// TPL program automatically calls GCC and makes .EXE file of project
    create_exe();

	/// Success!
	printf("OK!\n");
}

