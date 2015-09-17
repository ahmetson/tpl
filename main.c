#include "tpl.h"

#define EXIT return 0

//void free_glob_fns(void);

// Declareted global data in Programming Language
//token *glob_vars;// = (token*) malloc( sizeof(token) );
//token *glob_fns;// = (token*) malloc( sizeof(token) );
//token *glob_types;// = (token*) malloc( sizeof(token) );
//token *glob_sanaw;// = (token*) malloc( sizeof(token) );

// List of files that called global data
//token *glob_vars_from; // = (token*) malloc( sizeof(token) );
//token *glob_fns_from;  // = (token*) malloc( sizeof(token) );
//token *glob_types_from;// = (token*) malloc( sizeof(token) );
//token *glob_sanaw_from;// = (token*) malloc( sizeof(token) );

// Parse file list
// Parse file
// After parsing each file and creating algorithms, check fns and vars for
// existing
// make current_file algoritms
// make all_soft_file for linking
// 

/*
	int argc - argument count, number ofaccepted arguments
	const char argv - passed arguments array
**/
int main(int argc, const char **args, const char **argv)
{
	tpl(argc, args);
	
	EXIT;
}

/*void free_glob_fns()
{
	free(glob_vars);
	free(glob_fns);
	free(glob_types);
	free(glob_sanaw);
	
	free(glob_vars_from);
	free(glob_fns_from);
	free(glob_types_from);
	free(glob_sanaw_from);	
}*/
