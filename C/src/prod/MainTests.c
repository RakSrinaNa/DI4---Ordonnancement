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
		instance_print(instance);
		Solution * sol = solution_create(instance);
		
		solution_moveTaskPack(sol, 8, 0);
		solution_moveTaskPack(sol, 12, 0);
		solution_moveTaskPack(sol, 16, 1);
		solution_moveTaskPack(sol, 11, 1);
		solution_moveTaskPack(sol, 19, 2);
		solution_moveTaskPack(sol, 6, 2);
		solution_moveTaskPack(sol, 7, 3);
		solution_moveTaskPack(sol, 5, 3);
		solution_moveTaskPack(sol, 10, 4);
		solution_moveTaskPack(sol, 15, 4);
		solution_moveTaskPack(sol, 0, 5);
		solution_moveTaskPack(sol, 14, 5);
		solution_moveTaskPack(sol, 9, 6);
		solution_moveTaskPack(sol, 17, 6);
		solution_moveTaskPack(sol, 3, 7);
		solution_moveTaskPack(sol, 18, 7);
		solution_moveTaskPack(sol, 4, 8);
		solution_moveTaskPack(sol, 2, 8);
		solution_moveTaskPack(sol, 1, 9);
		solution_moveTaskPack(sol, 13, 9);
		
		solution_eval(sol);
		solution_print(sol);
		solution_destroy(sol);
		instance_destroy(instance);
		exit(EXIT_SUCCESS);
		TabuSolution * solution = tabu_search(instance);
		printf("Tabu found solution in %Lfs (%d iterations) : \n", solution->time, solution->iterations);
		solution_print(solution->solution);
		tabuSolution_destroy(solution);
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
