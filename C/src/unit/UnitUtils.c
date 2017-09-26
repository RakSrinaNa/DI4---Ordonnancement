#include "headers/UnitUtils.h"

void unit_error(char * str)
{
	fprintf(stderr, str);
	raise(SIGINT);
	exit(EXIT_FAILURE);
}

int unit_intArrayEquals(const int * base, const int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return 0;
	return 1;
}

int unit_uintArrayEquals(const unsigned int * base, const unsigned int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return 0;
	return 1;
}
