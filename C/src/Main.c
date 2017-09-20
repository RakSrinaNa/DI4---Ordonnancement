#include <string.h>
#include <stdlib.h>

#include "unit/headers/MainUnit.h"

int main(int argc, char * argv[])
{
	if(argc == 2)
	{
		if(strcmp(argv[1], "test") == 0) // Used to start unit tests
		{
			mainUnit();
		}
		else if(strcmp(argv[1], "temp") == 0) // For internal testing
		{
			
		}
	}

	return 0;
}
