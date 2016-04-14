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
    /// Kompýuterdäki ähli zatlar türkmen dilini harplaryny tanar ýaly.
    char *tk = setlocale(LC_ALL, ".1250");
    /// Türkmen harplaryny PROMPT'a çykarar ýaly.
    system("chcp 65001 > nul");

	/// TPL kodly faýllar hökman argumentler bilen berilmeli.
	source_codes_exist(argc);

	/// TPL programma işe girişmäne taýynlanýar.
	/// Global ülňiler ötürdilýär.
	init();

	/// HER BERLEN KODLY FAÝLYŇ ALGORITMLERI ÝASALÝAR
	work_with_sources(argc, args);

    /// Hemme TPL kodly faýllar barlanan soň, biri-biriniň arasyndaky näsazlyklara barlanylýar.
    work_with_semantic();

    /// Ähli TPL proýektine degişli bolan goşmaça C dilindäki kodlary goşýar
    work_with_translator_whole_project();

    /// TPL program automatically calls GCC and makes .EXE file of project
    create_exe();

	/// TPL PROGRAMMADAN ÇYKMALY
	printf("OK!\n");
}



