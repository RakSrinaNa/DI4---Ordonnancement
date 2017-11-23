#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "headers/Parser.h"
#include "headers/Solution.h"
#include "FLAGS.h"
#include "headers/Tabu.h"

Bool DEBUG = False;

int main(int argc, char * argv[])
{
	char * filepath = "./Input.txt";
	if(argc >= 2)
	{
		for(int i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "--debug") == 0)
				DEBUG = True;
			else
				filepath = argv[i];
		}
	}
	
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	if(instance != NULL)
	{
		instance_print(instance);
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
	fprintf(stderr, "\nWARNING: ");
	vfprintf(stderr, format, args);
	va_end(args);
}

void fatalError(char * format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "FATAL ERROR!\n |-\t");
	vfprintf(stderr, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
