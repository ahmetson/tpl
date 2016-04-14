#include "tpl.h"

//#define TPL_DEBUG_MODE

#ifdef TPL_DEBUG_MODE
#include <stdlib.h>
#define EXIT system("pause"); return 0
#else
#define EXIT return 0
#endif

/*void create_EXE()
{
    char GCC_EXE = "bin/gcc.exe ";
    char GCC_C_ARG = "-c ";
    char GCC_O_ARG = "-o ";
    // For creating object files
    char GCC_ADD_1 = "-I\"mingw\\include\\\" ";
    char GCC_ADD_2 = "-I\"mingw\\x86_64-w64-mingw32\\include\\\" ";
    char GCC_ADD_3 = "-I\"mingw/lib/gcc/x86_64-w64-mingw32/4.9.2/include ";
    // For creating .exe files
    char GCC_ADD_4 = "-L\"mingw/lib\" ";
    char GCC_ADD_5 = "-L\"mingw/x86_64-w64-mingw32/lib\" ";
    char GCC_ADD_STATIC = " -static-libgcc";



    // The making output file formula
    char C_TO_OBJ = "gcc.exe -c D:/tpl/example.c -o D:/tpl/example.o -I\"D:/Downloads/Dev-Cpp/MinGW64/include\" -I\"D:/Downloads/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" -I\"D:/Downloads/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" ";

    int     OBJ_LIST_LEN = 1;
    char *OBJ_LIST = malloc( OBJ_LIST_LEN );
    OBJ_LIST[0] = '\0';

    // Making OBJ files from C
    for ( i=0; i<FILES_NUM; ++i )
    {
        int line_len = strlen( GCC_PREVIOUS ) + strlen( GCC_C_ARG )*2 + strlen( FILES[ i ].c_name )*2 + strlen( GCC_C_TO_O_ADD_ARGS ) + 3;
        char *line = malloc( line_len );
        strncpy( line, GCC_FOLDER, strlen( GCC_FOLDER )+1 );
        strncpy( line, GCC_EXE, strlen( GCC_EXE )+1 );

        // Add -c command
        strncat( line, GCC_C_ARG, strlen( GCC_C_ARG ) );

        // Add .C file source
        strncat( line, FILES[ i ].c_name, strlen( FILES[ i ].c_name ) );

        // Add -o command
        strncat( line, GCC_O_ARG, strlen( GCC_O_ARG ) );

        // Add .O file source
        char obj_source[ MAX_FILE_LEN ] = { 0 };
        strncpy( obj_source, FILES[ i ].c_name, strlen( FILES[ i ].c_name ) );
        obj_source[ strlen( FILES[ i ].c_name )-1 ] = 'o';              // change .c extension to .o extension

        strncat( line, obj_source, strlen( obj_source ) );

        // Save .O name for making .EXE file
        OBJ_LIST_LEN += strlen( obj_source );
        OBJ_LIST = realloc( OBJ_LIST, OBJ_LIST_LEN );
        strncat( OBJ_LIST, obj_source, strlen( obj_source ) );

        // Add additional arguments
        char *adds = get_additional_arguments();
        strncat( line, adds, strlen( adds ) );
        free( adds );

        // Call GCC
        system( line );
        free( line );
    }

    // Make EXE from O files
    int line_len = strlen( GCC_FOLDER ) + strlen( GCC_EXE ) + strlen( OBJ_LIST ) + strlen( GCC_O_ARG ) + strlen( TPL_CALLED_DIR ) + strlen( MAIN_FILE_NAME ) + 1;
    char *line = malloc( line_len );

    strncpy( line, GCC_FOLDER, strlen( GCC_FOLDER )+1 );
    strncat( line, GCC_EXE, strlen( GCC_EXE ) );
    strncat( line, OBJ_LIST, strlen( OBJ_LIST ) );
    strncat( line, GCC_O_ARG, strlen( GCC_O_ARG ) );
    strncat( line, TPL_CALLED_DIR, strlen( TPL_CALLED_DIR ) );
    strncat( line, MAIN_FILE_NAME, strlen( MAIN_FILE_NAME ) );

    free ( OBJ_LIST );

    // Create .EXE file
    if ( !system( line ) )
    {
        // Can not create .EXE file
        free( line );
        delete_c_folder();
        print_err( 0, &inf_tok );
    }
    else
    {
        free( line );
        delete_tmp_c_folder();
    }

}


char *get_additional_arguments()
{
    char *GCC_ADD_CMD = " -I";
    char *GCC_ADD_1 = "include";
    char *GCC_ADD_2 = "x86_64-w64-mingw32/include/";
    // We must add to commands

    int i = strlen( GCC_FOLDER )*2 + strlen(GCC_ADD_1) + strlen( GCC_ADD_2 )+strlen( GCC_ADD_CMD )+/* D.quotes near of arguments *//*2+1 ;

    char *str = malloc( i );
    strncpy( str, GCC_ADD_CMD, strlen( GCC_ADD_CMD )+1 );
    strncat( str, "\"", strlen( "\"" ) );
    strncat( str, GCC_FOLDER, strlen( GCC_FOLDER ) );
    strncat( str, GCC_ADD_1, strlen( GCC_ADD_1 ) );
    strncat( str, "\"", strlen( "\"" ) );
    strncat( str, GCC_ADD_CMD, strlen( GCC_ADD_CMD ) );
    strncat( str, "\"", strlen( "\"" ) );
    strncat( str, GCC_FOLDER, strlen( GCC_FOLDER ) );
    strncat( str, GCC_ADD_2, strlen( GCC_ADD_2 ) );
    strncat( str, "\"", strlen( "\"" ) );

    return str;
}*/

int main(int argc, const char **args)
{



    // After preparing dir and file names
    // Check for existing of Template directory
    // If so
    //      Delete it
    //      If it can't be deleted
    //          Show error: Can't modify temporary data.
                        //  Please, insert your TPL program's folder to accessable place
    //  Create temporary folder for c codes
    //  If it can't be created
    //      Show error: Can't modify temporary data.
                    //  Please, insert your TPL program's folder to accessable place
    //   Check for existing of C compiler
    //   If compiler doesn't exists
    //      Show error: Missing C compiler.

    // After translator to C.
    // Create compiler to C.
    // It allocates string, for creating Compiling Executable file

    // Add path to C compiler
    //  Add prepared c compiler options (-optimize -STD=C11)
    // Add each .C code+C folder path from Files list
    // Add output directory which is current path where TPL called from + name of folder. Which is main TPL files name + ".exe"

    // Call System function with given text.
    // Free memory.
    // If System function returns SUCCESS
    //      Delete C folder
    //      Show OK
    //  Else
    //      Delete C folder
    //      Show error: Can't compile program from .TEPL to .C


    // All helper functions and variables must be added to the main .c and .h pages

    /// TPl baş funksiýasy hökmän programma gelen argumentleri we olaryň sanyny kabul etmeli.
	tpl(argc, args);


	EXIT;
}
