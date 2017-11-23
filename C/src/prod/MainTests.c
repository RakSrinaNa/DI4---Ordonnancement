#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "headers/Instance.h"
#include "../unit/headers/MainUnit.h"
#include "headers/Parser.h"
#include "headers/Solution.h"
#include "FLAGS.h"

Bool DEBUG = False;

int main(int argc, char * argv[])
{
	srand((unsigned long) SEED);
	char * filepath = "./Input.txt";
	if(argc >= 2) //Testings and stuff.
	{
		char * method = NULL;
		for(int i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "--debug") == 0)
				DEBUG = True;
			else
				method = argv[i];
		}
		if(method != NULL)
		{
			if(strcmp(method, "test") == 0) // Used to start unit tests.
			{
				mainUnit();
				exit(EXIT_SUCCESS);
			}
			else if(strcmp(method, "temp") == 0) // For internal testing.
			{
				filepath = "../Inputs/input.txt";
			}
		}
	}
	
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	if(instance != NULL)
	{
		//instance_print(instance);
		Solution * solution = solution_create(instance);
		solution_eval(solution);
		solution_print(solution);
		solution_save(solution, "../Inputs/output1.txt", 14.25);
		instance_destroy(instance);
	}
	return 0;
}

void debugPrint(char * format, ...)
{
	if(DEBUG)
	{
		va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
}

void warn(char * format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stdout, "\nWARNING: ");
	vfprintf(stdout, format, args);
	va_end(args);
}

void fatalError(char * format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stdout, "FATAL ERROR!\n |-\t");
	vfprintf(stdout, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
