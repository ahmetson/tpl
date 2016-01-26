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


void tpl(int argc, const char **args);

#endif
