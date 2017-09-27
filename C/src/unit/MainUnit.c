#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "headers/ParserUnit.h"
#include "headers/InstanceUnit.h"
#include "headers/TaskUnit.h"
#include "headers/PackUnit.h"
#include "headers/SolutionUnit.h"

int mainUnit()
{
#define UNIT 1
	srand((unsigned int) time(NULL));
	setbuf(stdout, NULL);
	printf("Executing Task tests...");
	taskUnit();
	printf("OK\n");
	printf("Executing Instance tests...");
	instanceUnit();
	printf("OK\n");
	printf("Executing Parser tests...");
	parserUnit();
	printf("OK\n");
	
	printf("Executing Pack tests...");
	packUnit();
	printf("OK\n");
	printf("Executing Solution tests...");
	solutionUnit();
	printf("OK\n");
	return 0;
}
