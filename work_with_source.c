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


/** Opens all TEPL source code files and give them to parser.
    After parser job, clears memory
    @n         - number of TEPL sources
    @filePaths - Addresses of TEPL sources
*/
void work_with_sources( int n, const char **file_paths )
{
    int i;
    for ( i=1; i<n; ++i )
	{
		/// TPL Program works with Wide characters
		wchar_t source[ MAX_FILE_LEN ] = {0};
        mbstowcs( source, file_paths[ i ], strlen( file_paths[ i ] ) );

        /// Prepare field for file parameters and file paths that will need for GCC compiler.
        add_file_info( source );
        /// Soňky kodlardaky yglan edilen maglumatlary ulanmak üçin.
        /// If TEPL source code called global information from another source code.
        /// To add includes
        /// DELETE-ME
		includes_add_new();

        wcsncpys( CUR_FILE_NAME, source );

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
	FILE *source = fopen( file_path, "r" );
    if ( source==NULL )
	{
		CUR_PART = 1;
		print_err(CODE1_FILE_CANT_OPEN, &inf_tok);
	}
	move_file_to_tmp_var( source, &CUR_SOURCE, &CUR_SOURCE_BYTES_NUM );
	fclose( source );   /// TEPL source code copied to temporary variable. No need file

    /* +.get file as bytes to temporary variable.
       +.set global variable for storing wide character version.
       +.add to initializing function the global variable
       +.add to exit function the global variable
       +.create converters from multibyte to wide characters.
       +.convert temporary variable to wide character and store it in global variable
       +.delete temporary variable.

       .Delete getting or setting FILE * as arguments from parser function.
       .Add global integer that will represent global position of Character seeker in global variable of source code.
       .Init global integer in initializing function.
       .Change parsing method from file = getchar, to moving Character seeker position in parser.
       .Change parsing method from file = getchar, to moving Character seeker position in pragma
       .Change parsing method from file = getchar, to moving Character seeker position in parenthesis inside.
       .At the end of work_with_source free and prepare for next source code the global variabe of wide characters.
       .At the end of work_with_source prepare for next source code the global varaible of character seeker
    */


	/// Move to 2nd part of TPL, which is PARSER.
	parse( );

    /// Clear local data. And init again. Might be used for next TEPL source code.
    if ( CUR_SOURCE_BYTES_NUM )
    {
        free( CUR_SOURCE );
        CUR_SOURCE = NULL;
    }
    CUR_SOURCE_BYTES_NUM = 0;
    CUR_SOURCE_POINTER   = 0;

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

void move_file_to_tmp_var( FILE *source, wchar_t **source_holder, int *source_holder_bytes_num )
{

    char *tmp = NULL;
    int   tmp_len = 0,
          tmp_c;

    int   sizeof_char = sizeof( char    ),
          sizeof_wchar= sizeof( wchar_t );

	while ( ( tmp_c=fgetc( source ) )!= EOF )
    {
		tmp_len += sizeof_char;
		tmp = realloc( tmp, tmp_len );
        tmp[ tmp_len-1 ] = tmp_c;
	}
	tmp_len += sizeof_char;
    tmp = realloc( tmp, tmp_len );
	tmp[ tmp_len-1 ] = '\0';

    /// Delete BOM
    if ( strlen( tmp )>=3 && tmp[0]==-17 && tmp[1]==-69 && tmp[2]==-65 )
    {
        tmp[0] = ' ';
        tmp[1] = ' ';
        tmp[2] = ' ';
    }

    *source_holder_bytes_num = count_mb_bytes( tmp )*sizeof_wchar;
    *source_holder = malloc( *source_holder_bytes_num );
    mb2wc( tmp, *source_holder, *source_holder_bytes_num );

	free( tmp );        /// TEPL source code converted and stored in global wide character variable. No need temporary
}


