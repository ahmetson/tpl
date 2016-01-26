/** Tokenler uchin ulanylyan 'struct' instruksiyasy arkaly yasalan maglumat tipleri
*/
#ifndef TOKEN_STRUCTS_H
#define TOKEN_STRUCTS_H

#include <wchar.h>

#ifndef TOKEN_CLASSES_NUM
#define TOKEN_CLASSES_NUM 17
#endif

#define MAXLEN 255

// Harpl tokeni hasaplananok.
#ifndef TOKEN_TYPES_NUM
#define TOKEN_TYPES_NUM 18
#endif


#ifndef TOKEN_MAX_TYPES_NUM
#define TOKEN_MAX_TYPES_NUM 5
#endif

#define MAX_VAR_LEN 32


// Token tipi
typedef struct{
	int                     type_num;
	int                     type_class;
	wchar_t                    need_value;	        // token must contain value
	wchar_t                    value[MAXLEN];
	wchar_t                    tok_value_type;     // Default - wchar_t*
	wchar_t                    is_compl;		    // Is token completed
	wchar_t                    type_must_check;
	wchar_t                    parenthesis;
	int                    string_value;
}token_type;

// Token uchin
typedef struct{
	int  ns;													// Namespace of token: 0-global, 1-local_file
	//wchar_t *klas;												// Class of token type
	token_type potentional_types[TOKEN_TYPES_NUM];		// Potentional types of token
	int potentional_types_num;									// Number of recognized types for token
	//wchar_t source_file[MAXLEN];									// source file of token
	wchar_t value[MAXLEN];
	int is_compl;
	int type_class;
    int required_type_num;
	int required_type_class;
	int inf_line_num;
	int inf_file_num;
	int inf_wchar_t_num;
	wchar_t parenthesis;
	wchar_t inf_wchar_t;
}token;

typedef struct{
    int type_class;
    int type_num;
    wchar_t name[MAXLEN];
    int inf_file_num;
    int inf_wchar_t_num;
    wchar_t inf_wchar_t;
    int inf_line_num;
}glob_ident;



#endif
