/** 1. Kodly fayllary harplar boyuncha parsing edyan bolum*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tpl.h"
#include "main/glob.h"
#include "main/user_def_type.h"
#include "main/files.h"
#include "fns.h"
#include "cmds.h"
#include "parser.h"
#include "algor.h"
#include "translator_to_c.h"
#include "work_with_source.h"
#include "translator_to_c/includes.h"
#include "error.h"


/** TPL kodly faýllary ýekäm ýekäm açyp, parsere berýär.
    Parser işläp bolan soň bolsa, faýldan galan zibili arassalaýar
 * @n - kodly faýllaryň sany
 * @file_paths - kodly faýllaryň salgylarynyň sanawy
**/
void work_with_sources( int n, const char **file_paths )
{
    int i;

    for ( i=1; i<n; ++i )
	{
		/// TPL programda wide character bilen işlenilýär
		wchar_t source[MAX_FILE_LEN] = {0};
        mbstowcs( source, file_paths[ i ], strlen( file_paths[i]) );

        /// Faýlyň maglumatlary we şolara görä C kodynda talap ediljek faýl atlary taýýarlanylýar.
        add_file_info( source );
        /// Soňky kodlardaky yglan edilen maglumatlary ulanmak üçin.
		includes_add_new();

        wcscpys( CUR_FILE_NAME, source );

        check_for_reserved_file_name( source );

		work_with_source( file_paths[ i ] );
	}

}


/** TPL kodly faýly açyp, parsere berýär.
    Parser işläp bolan soň bolsa, faýldan galan zibili arassalaýar
    @parseFileName - parserlenmeli faýlyň salgysy
**/
void work_with_source( const char *file_path )
{
	FILE *source = fopen( file_path, "r, ccs=UTF-8");

	if ( source==NULL )
	{
		CUR_PART = 1;
		print_err(CODE1_FILE_CANT_OPEN, &inf_tok);
	}
	// 2-nji TPL-in bolumine gechildi
	parse( source );

	fclose( source );
}


/** Eger TPL üçin ýasalan faýlyň ady, TPL programyň özi üçin ulanýan faýlynyň ady bolsa
    programdan çykmaly
*/
void check_for_reserved_file_name( wchar_t *file_path )
{
    if ( is_reserved_source_name( file_path ) )
    {
        CUR_PART = 1;
        print_err( CODE1_FILE_NAME_RESERVED, &inf_tok );
    }
}

