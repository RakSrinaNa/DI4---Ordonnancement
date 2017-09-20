#include "headers/UnitUtils.h"

void unit_error(char * str)
{
	perror(str);
	raise(SIGINT);
	exit(EXIT_FAILURE);
}

int unit_intArrayEquals(int * base, int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return 0;
	return 1;
}

int unit_uintArrayEquals(unsigned int * base, unsigned int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return 0;
	return 1;
}
