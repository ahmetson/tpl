/*
 * Tokenler uchin ulanylyan 'struct' instruksiyasy arkaly yasalan maglumat tipleri
**/
#ifndef TOKEN_STRUCTS_H
#define TOKEN_STRUCTS_H

#define MAXLEN 255
#define CONST_TOKEN_TYPES_NUM 8

#define MAX_VAR_LEN 32


// Token tipi
typedef struct{
	int                     type_num;
	int                     type_class;
	char                    need_value;	        // token must contain value
	char                    value[MAXLEN];
	char                    tok_value_type;     // Default - char*
	char                    is_compl;		    // Is token completed
	char                    type_must_check;
	char                   **string_value;
}token_type;

// Token uchin
typedef struct{
	int  ns;													// Namespace of token: 0-global, 1-local_file
	//char *klas;												// Class of token type
	token_type potentional_types[CONST_TOKEN_TYPES_NUM];		// Potentional types of token
	int potentional_types_num;									// Number of recognized types for token
	//char source_file[MAXLEN];									// source file of token
	char value[MAXLEN];
	int is_compl;
	int type_class;
    int required_type_num;
	int required_type_class;
	int inf_line_num;
	int inf_file_num;
	int inf_char_num;
	char inf_char;
}token;

// Her tokenin tipi uchin funksiyalar
typedef struct{
	int (*is_token) (token *tok, char*token_val);
}is_token_item;


#endif
