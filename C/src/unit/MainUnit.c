#include <stdio.h>
#include "headers/ParserUnit.h"
#include "headers/InstanceUnit.h"

int mainUnit()
{
	setbuf(stdout, NULL);
	printf("Executing Instance tests...");
	instanceUnit();
	printf("OK\n");
	printf("Executing Parser tests...");
    parserUnit();
	printf("OK\n");
	return 0;
}
