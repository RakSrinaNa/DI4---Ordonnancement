#include <string.h>
#include <stdlib.h>

#ifdef __WIN32__
#include <io.h>
#else
#include <sys/stat.h>
#endif

#include "headers/Utils.h"
#include "headers/Parser.h"
#include "headers/Solution.h"
#include "FLAGS.h"
#include "headers/Tabu.h"
#include "../unit/headers/MainUnit.h"

#ifdef __MINGW32__
#define printf __mingw_printf
#endif

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
#ifdef __WIN32__
		mkdir("log");
#else
		mkdir("log", 0777);
#endif
		//instance_print(instance);		
		TabuSolution * solution = tabu_search(instance);
		printf("Tabu found solution in %Lfs (%d iterations) : \n", solution->time, solution->iterations);
		solution_print(solution->solution);
		char filenameSolution[512];
		sprintf(filenameSolution, "./log/solution_%s_%d.txt", instance->origin, tabu_flagsFingerprint());
		FILE * file = fopen(filenameSolution, "w");
		solutionInfo_printForVerification(file, solution->solution, solution->solution->info);
		fclose(file);
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
	fprintf(stdout, "FATAL ERROR!\n\t");
	vfprintf(stdout, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}
