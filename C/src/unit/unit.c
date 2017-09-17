#include "headers/unit.h"

void unit_error(char * str)
{
	perror(str);
	raise(SIGINT);
	exit(EXIT_FAILURE);
}

int unit_arrayEquals(int * base, int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return 0;
	return 1;
}
