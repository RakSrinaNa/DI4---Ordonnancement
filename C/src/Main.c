#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "unit/headers/MainUnit.h"
#include "headers/Instance.h"
#include "headers/Parser.h"

static int UNIT = 0;

int main(int argc, char * argv[])
{
	char * filepath = "./Input.txt";
	if(argc == 2) //Testings and stuff.
	{
		if(strcmp(argv[1], "test") == 0) // Used to start unit tests.
		{
			UNIT = 1;
			mainUnit();
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(argv[1], "temp") == 0) // For internal testing.
		{
			filepath = "./../Inputs/input.txt";
		}
	}
	
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	instance_print(instance);
	//TODO: Compute solution
	instance_destroy(instance);
	return 0;
}

void warn(char * format, ...)
{
	if(UNIT == 0)
	{
		va_list args;
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
	}
}
