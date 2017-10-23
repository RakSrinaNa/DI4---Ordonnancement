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
#include "headers/SolutionInfoUnit.h"

extern Bool UNIT_FAILED;
extern Bool DEBUG;

int mainUnit()
{
	DEBUG = False;
	
	srand((unsigned int) time(NULL));
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	printf("UNIT: Executing Task tests...\n");
	taskUnit();
	printf("UNIT: OK\n");
	printf("UNIT: Executing Instance tests...\n");
	instanceUnit();
	printf("UNIT: OK\n");
	printf("UNIT: Executing Parser tests...\n");
	parserUnit();
	printf("UNIT: OK\n");
	
	printf("UNIT: Executing Pack tests...\n");
	packUnit();
	printf("UNIT: OK\n");
	printf("UNIT: Executing SolutionInfo tests...\n");
	solutionInfoUnit();
	printf("UNIT: OK\n");
	printf("UNIT: Executing Solution tests...\n");
	solutionUnit();
	printf("UNIT: OK\n");
	
	printf("UNIT: Executing Sequencer tests...\n");
	sequencerUnit();
	printf("UNIT: OK\n");
	
	if(UNIT_FAILED)
		exit(EXIT_FAILURE);
	else
		printf("UNIT: TESTS OK - CONGRATULATIONS\n");
	return 0;
}
