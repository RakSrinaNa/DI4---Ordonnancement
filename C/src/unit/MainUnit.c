#include <stdio.h>
#include "ParserUnit.h"

int mainUnit()
{
	setbuf(stdout, NULL);
	printf("Executing Parser tests...");
    parserUnit();
	printf("OK\n");
	return 0;
}
