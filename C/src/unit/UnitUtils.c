#include "headers/UnitUtils.h"
#include "../prod/headers/Utils.h"

Bool UNIT_FAILED = False;

extern Bool DEBUG;

void unit_error(char * str, ...)
{
	va_list args;
	va_start(args, str);
	fprintf(stderr, "\nFATAL UNIT: ");
	vfprintf(stderr, str, args);
	fprintf(stderr, "\n");
	va_end(args);
	unit_breakpoint();
	UNIT_FAILED = True;
	//exit(EXIT_FAILURE);
}

void unit_breakpoint()
{
	raise(SIGINT);
}

Bool unit_intArrayEquals(const int * base, const int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return False;
	return True;
}

Bool unit_uintArrayEquals(const unsigned int * base, const unsigned int * compare, int length)
{
	for(int i = 0; i < length; i++)
		if(base[i] != compare[i])
			return False;
	return True;
}
