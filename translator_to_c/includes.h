/** Ýasaljak kodda #inçlude preprosessor komandasy bilen işleýän ähli funksiýalar we maglumatlar
 *
**/
#ifndef TRANSLATOR_TO_C_INCLUDES_H
#define TRANSLATOR_TO_C_INCLUDES_H
#include "../main/glob.h"

// Içine inklud etmeli preprosessor komandasyny ulanmaly faýllaryň sanyny ýene birine köpledýär
file_incs *includes_add_new();

// Içinde inklud edýän preprosessor komandasyny ulanmaly faýla, inklud etmeli faýly goşýar
void includes_file_add_include(file_incs *fi, char *h_source);

// Ýasalan kodlara, #include preprosessory goşulýar
void translator_to_c_add_includes();

#endif // TRANSLATOR_TO_C_INCLUDES_H
