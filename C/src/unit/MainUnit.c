#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "headers/UnitUtils.h"
#include "headers/ParserUnit.h"
#include "headers/InstanceUnit.h"
#include "headers/TaskUnit.h"
#include "headers/PackUnit.h"
#include "headers/SolutionUnit.h"
#include "headers/SequencerUnit.h"

extern Bool UNIT_FAILED;

int mainUnit()
{
	srand((unsigned int) time(NULL));
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
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
	
	printf("Executing Sequencer tests...");
	sequencerUnit();
	printf("OK\n");
	
	if(UNIT_FAILED)
		exit(EXIT_FAILURE);
	else
		printf("TESTS OK - CONGRATULATIONS\n");
	return 0;
}
