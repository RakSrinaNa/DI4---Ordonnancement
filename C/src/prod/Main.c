#include <string.h>
#include <stdlib.h>

#include "headers/Utils.h"
#include "headers/Parser.h"
#include "headers/Solution.h"

Bool DEBUG = True;

int main(int argc, char * argv[])
{
	char * filepath = "./Input.txt";
	//Main, read file and compute a solution.
	Instance * instance = parser_readInstanceFromFile(filepath);
	//instance_print(instance);
	Solution * solution = solution_create(instance);
	solution_eval(solution);
	solution_print(solution);
	solution_save(solution, "../Inputs/output1.txt");
	instance_destroy(instance);
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
