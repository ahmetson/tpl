#include "tpl.h"

#define EXIT return 0

/*
	int argc - argumentlering sany. Faylyng oz adyndan bashlaar
	const char argv - passed arguments array
**/
int main(int argc, const char **args, const char **argv)
{
	tpl(argc, args);
	
	EXIT;
}
