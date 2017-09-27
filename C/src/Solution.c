#include "headers/Solution.h"

#include "headers/Utils.h"
#include "headers/Pack.h"

Solution* solution_create(Instance *instance)
{
	Solution *solution = NULL;
	MMALLOC(solution, Solution, 1, "solution_create");
	solution->instance = instance;
	solution->packCount = 0;
	solution->packList = NULL;
	solution->processOrder = NULL;
	
	MMALLOC(solution->processOrder, unsigned int, solution->instance->taskCount, "solution_create");
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		solution->processOrder[i] = i;
	
	return solution;
}

void solution_delete(Solution* solution)
{
	free(solution->packList);
	free(solution->processOrder);
	free(solution);
}

void solution_setProcessIndex(Solution *solution, unsigned int task, unsigned int position)
{
	
}


