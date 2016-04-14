#ifndef TPL_H
#define TPL_H

/// wchar_t tipi, TPL funksiýanyň çagyrýan funksiýalarynda belli bolar ýaly
#include <stdio.h>

#include "main/glob.h"
#include "main/init.h"
#include "main/main_file.h"
#include "fns.h"

#ifndef CHECK_VALID_CHAR
#define CHECK_VALID_CHAR 1
#define UNCHECK_VALID_CHAR 0
#endif

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _wgetcwd
#else
    #include <unistd.h>
    #define GetCurrentDir wgetcwd
 #endif

#include <stdlib.h>
#include <windows.h>
#define TMP_FOLDER_NAME     "tmp_c_code"
#define GCC_PATH            "gcc\\gcc.exe"


void tpl(int argc, const char **args);

#endif
