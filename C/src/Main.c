#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "unit/headers/MainUnit.h"
#include "headers/Instance.h"
#include "headers/Parser.h"
#include "headers/Solution.h"

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
			filepath = "./unitResources/Instance2.txt";
		}
	}
	
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	instance_print(instance);
	Solution * solution = solution_create(instance);
	solution_print(solution);
	solution_sortByDD(solution);
	solution_print(solution);
	//TODO: Compute solution
	instance_destroy(instance);
	return 0;
}

void warn(char * format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "\n");
	vfprintf(stderr, format, args);
	va_end(args);
}

void fatal_error(char * format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "FATAL ERROR!\n |-\t");
	vfprintf(stderr, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
