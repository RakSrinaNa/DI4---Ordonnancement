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


int solution_getPackIndex(Solution *solution, unsigned int pack)
{
	// TODO : faire ça
}

int solution_getProcessIndex(Solution *solution, unsigned int task)
{
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
	{
		if(solution->processOrder[i] == task)
			return (int) i;
	}
	return -1;
}

void solution_setProcessIndex(Solution *solution, unsigned int task, unsigned int position)
{
	if(task < solution->packCount)
	{
		unsigned int index = solution_getProcessIndex(solution, task);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < solution->packCount; i += direction)
		{
			solution->processOrder[i] = solution->processOrder[i + direction];
		}
		solution->processOrder[MMIN(position, solution->packCount - 1)] = task;
	}
	else
		warn("WARNING : solution_setProcessIndex : given process does not exist (%d)\n", task);
}




