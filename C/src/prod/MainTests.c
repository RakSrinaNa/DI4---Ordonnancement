#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "headers/Instance.h"
#include "../unit/headers/MainUnit.h"
#include "headers/Parser.h"
#include "headers/Solution.h"
#include "headers/Tabu.h"
#include "FLAGS.h"

Bool DEBUG = False;

int main(int argc, char * argv[])
{
	srand((unsigned long) SEED);
	char * filepath = "./unitResources/Instance1.txt";
	if(argc >= 2) //Testings and stuff.
	{
		Bool test = False;
		for(int i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "--debug") == 0)
				DEBUG = True;
			else if(strcmp(argv[i], "--test") == 0)
				test = True;
			else
				filepath = argv[i];
		}
		if(test)
		{
			mainUnit();
			exit(EXIT_SUCCESS);
		}
	}
	
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	if(instance != NULL)
	{
		TabuSolution * solution = tabu_search(instance);
		printf("Tabu found best in %Lfs (%d iterations) : \n", solution->time, solution->iterations);
		solution_print(solution->solution);
		tabuSolution_destroy(solution);
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
	fprintf(stdout, "WARNING: ");
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
