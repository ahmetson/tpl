/**
 * 1. Kodly fayllary harplar boyuncha parsing edyan bolum
**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tpl.h"
#include "main/glob.h"
#include "main/user_def_type.h"
#include "cmds.h"
#include "parser.h"
#include "algor.h"
#include "translator_to_c.h"
#include "work_with_source.h"
#include "translator_to_c/includes.h"
#include "error.h"


/**
 * Kodly faýllary parserleýär.
 *
 * @argc - kodly faýllaryň sany
 * @args - kodly faýllaryň salgylarynyň sanawy
**/
void work_with_sources(int argc, const char **args)
{
    int i;

    for (i=1; i<argc; ++i)
	{
		//printf("Parsing etmeli: '%s'\n", args[i]);
		// Häzirki parserlenýän kodly faýlyň ady
        add_file_info(args[i]);

		if (is_reserved_source_name((char *)args[i]))
        {
            CUR_PART = 1;
            print_err(CODE1_FILE_NAME_RESERVED, &inf_tok);
        }
		includes_add_new();
        strncpy(CUR_FILE_NAME, args[i], strlen(args[i])+1);

		work_with_source(args[i]);
	}

}


/**
 * Faýly parserleýär.
 *
 * @parse_file_name - parserlenmeli faýlyň salgysy
**/
int work_with_source(const char *parse_file_name)
{
	FILE *source = fopen(parse_file_name, "r");

	if (source==NULL)
	{
		CUR_PART = 1;
		print_err(CODE1_FILE_CANT_OPEN, &inf_tok);
	}

	// 2-nji TPL-in bolumine gechildi
	parse(source);

	fclose(source);
	return 1;
}


