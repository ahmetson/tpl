#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "create_exe.h"
#include "glob.h"
#include "../error.h"
#include "../fns.h"

void create_exe()
{
    wchar_t *GCC_C_ARG = L" -std=c11 -c ";
    wchar_t *GCC_O_ARG = L"-o ";

    int  OBJ_LIST_LEN = 0;
    wchar_t *OBJ_LIST = NULL,
            *line = NULL;

    OBJ_LIST_LEN += ( wcslen( L" " )+1 )*sizeof( wchar_t ) ;
    OBJ_LIST = realloc( OBJ_LIST, OBJ_LIST_LEN );
    wcsncpys( OBJ_LIST, L" " );

    // Making OBJ files from C
    int i, line_len;

    for ( i=0; i<CUR_FILE_NUM; ++i )
    {
        line_len = ( wcslen( FILE_GCC_EXE ) + wcslen( GCC_C_ARG )*2 + wcslen( FILES[ i ].c_source )*2 + wcslen( GCC_C_TO_O_ADDS ) + 5 )*sizeof( wchar_t );
        line = malloc( line_len );
        wcsncpys( line, FILE_GCC_EXE );

        // Add -c command
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_C_ARG, wcslen( GCC_C_ARG ) );

        // Add .C file source
        wcsncat( line, FILES[ i ].c_source, wcslen( FILES[ i ].c_source ) );

        // Add -o command
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_O_ARG, wcslen( GCC_O_ARG ) );

        // Add .O file source
        wchar_t obj_source[ MAX_FILE_LEN ] = { 0 };
        wcsncpys( obj_source, FILES[ i ].c_source );
        obj_source[ wcslen( obj_source )-1 ] = L'o';              // change .c extension to .o extension
        wcsncat( line, obj_source, wcslen( obj_source ) );

        // Save .O name for making .EXE file
        OBJ_LIST_LEN += ( wcslen( obj_source ) + wcslen( L" " ) )*sizeof( wchar_t ) ;
        OBJ_LIST = realloc( OBJ_LIST, OBJ_LIST_LEN );
        wcsncat( OBJ_LIST, obj_source, wcslen(  obj_source ) );
        wcsncat( OBJ_LIST, L" ", wcslen( L" " ) );

        // Add additional arguments
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_C_TO_O_ADDS, wcslen( GCC_C_TO_O_ADDS ) );

        // Call GCC
        _wsystem( line );
        free( line );
    }
    /// Add additional files: for example source codes of kabul_et
    wchar_t std_kabul_et[ MAX_FILE_LEN ] = { 0 };
    wcsncpys( std_kabul_et, DIR_C_FOLDER );
    wcscat( std_kabul_et, L"_tpl_std_kabul_et.c" );
    {
        line_len = ( wcslen( FILE_GCC_EXE ) + wcslen( GCC_C_ARG )*2 + wcslen( std_kabul_et )*2 + wcslen( GCC_C_TO_O_ADDS ) + 5 )*sizeof( wchar_t );
        line = malloc( line_len );
        wcsncpys( line, FILE_GCC_EXE );

        // Add -c command
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_C_ARG, wcslen( GCC_C_ARG ) );

        // Add .C file source
        wcscat( line, std_kabul_et );

        // Add -o command
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_O_ARG, wcslen( GCC_O_ARG ) );

        // Add .O file source
        wchar_t obj_source[ MAX_FILE_LEN ] = { 0 };
        wcsncpys( obj_source, std_kabul_et );
        obj_source[ wcslen( obj_source )-1 ] = L'o';              // change .c extension to .o extension
        wcsncat( line, obj_source, wcslen( obj_source ) );

        // Save .O name for making .EXE file
        OBJ_LIST_LEN += ( wcslen( obj_source ) + wcslen( L" " ) )*sizeof( wchar_t ) ;
        OBJ_LIST = realloc( OBJ_LIST, OBJ_LIST_LEN );
        wcsncat( OBJ_LIST, obj_source, wcslen(  obj_source ) );
        wcsncat( OBJ_LIST, L" ", wcslen( L" " ) );

        // Add additional arguments
        wcsncat( line, L" ", wcslen( L" " ) );
        wcsncat( line, GCC_C_TO_O_ADDS, wcslen( GCC_C_TO_O_ADDS ) );

        // Call GCC
        _wsystem( line );
        free( line );
    }

    // Make EXE from O files
    line_len = ( wcslen( FILE_GCC_EXE ) + wcslen( OBJ_LIST ) + wcslen( GCC_O_ARG ) + wcslen( FILE_OUT_EXE ) + wcslen( GCC_O_TO_EXE_ADDS ) + 3)*sizeof( wchar_t );
    line = malloc( line_len );

    wcsncpys( line, FILE_GCC_EXE );
    wcsncat( line, OBJ_LIST, wcslen( OBJ_LIST ) );
    wcsncat( line, L" ", wcslen( L" " ) );
    wcsncat( line, GCC_O_ARG, wcslen( GCC_O_ARG ) );
    wcsncat( line, FILE_OUT_EXE, wcslen( FILE_OUT_EXE ) );
    // Add additional arguments
    wcsncat( line, L" ", wcslen( L" " ) );
    wcsncat( line, GCC_O_TO_EXE_ADDS, wcslen( GCC_O_TO_EXE_ADDS ) );

    free ( OBJ_LIST );

    //printf("%ls", line);

    // Create .EXE file
    if ( _wsystem( line ) )
    {
        // Can not create .EXE file
        free( line );
        sys_rmdir( DIR_C_FOLDER );
        CUR_PART = 0;
        print_err( CODE0_CANT_CREATE_EXE, &inf_tok );
    }
    else
    {
        free( line );
        sys_rmdir( DIR_C_FOLDER );
    }
}
