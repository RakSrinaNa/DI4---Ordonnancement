#include "headers/Tabu.h"

Solution * tabu_solutionInit(Instance * instance)
{
	Solution * solution = solution_create(instance);
	Solution * bestSolution = NULL;
	solution_sortByDD(solution);
	unsigned int maxTasksPerPack = 1;
	while(maxTasksPerPack <= instance->taskCount / 2) //Try every size of packs.
	{
		Solution * solutionCopy = solution_copy(solution);
		for(unsigned int i = 0; i < instance->taskCount; i++) //Move tasks to do packs of max maxTasksPerPack tasks per pack.
			solution_moveTaskPack(solution, solution->productionOrder[i], i / maxTasksPerPack);
		if(bestSolution == NULL || solution_eval(solutionCopy) > solution_eval(bestSolution))
		{
			solution_destroy(bestSolution);
			bestSolution = solutionCopy;
		}
		else
			solution_destroy(solutionCopy);
		maxTasksPerPack++;
	}
	solution_destroy(solution);
	return bestSolution;
}
