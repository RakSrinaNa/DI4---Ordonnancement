#include <sys/timeb.h>
#include "headers/Instance.h"
#include "headers/Solution.h"
#include "FLAGS.h"

Solution * tabu_solutionInit(Instance * instance)
{
	debugPrint("Creating initial solution for instance %p\n", instance);
	Solution * bestSolution = NULL;
	task_t * sortedDueDates = instance_sortByDueDate(instance);
	unsigned int maxTasksPerPack = 1;
	while(maxTasksPerPack <= instance->taskCount / 2) //Try every size of packs.
	{
		debugPrint("\tCreating packs of size %d\n", maxTasksPerPack);
		Solution * solution = solution_create(instance);
		for(unsigned int i = 0; i < instance->taskCount; i++) //Move tasks to do packs of max maxTasksPerPack tasks per pack.
			solution_moveTaskPack(solution, sortedDueDates[i], i / maxTasksPerPack);
		if(bestSolution == NULL || solution_eval(solution) < solution_eval(bestSolution))
		{
			solution_destroy(bestSolution);
			bestSolution = solution;
		}
		else
			solution_destroy(solution);
		maxTasksPerPack++;
	}
	free(sortedDueDates);
	debugPrint("Initial solution is %p\n", bestSolution);
	return bestSolution;
}

long double tabu_getTimeDiff(struct timeb start, struct timeb end)
{
	return end.time - start.time + (end.millitm - start.millitm) / 1000.0f;
}

Solution * tabu_search(Instance * instance)
{
	Solution *initSolution = tabu_solutionInit(instance);
	Solution *bestSolution = initSolution;
	Solution *currentSolution = initSolution;
	Solution *bestNeighbor = NULL;
	unsigned int nbIterations = 0;
	unsigned int nbNoBetterIterations = 0;
	Bool diversification = False;
	struct timeb timeStart;
	ftime(&timeStart);
	struct timeb timeNow;
	ftime(&timeNow);
	unsigned int timeLimit = instance->taskCount * instance->machineCount / 4;
	
	while(tabu_getTimeDiff(timeStart, timeNow) < timeLimit && nbIterations < NB_ITERATION_MAX)
	{
		if(TABU_SEARCH_SWAP)
		{
			tabu_searchSwap(&currentSolution, &bestSolution, &bestNeighbor);
		}
		if(TABU_SEARCH_EBSR)
		{
			tabu_searchEBSR(&currentSolution, &bestSolution, &bestNeighbor);
		}
		if(TABU_SEARCH_EFSR)
		{
			tabu_searchEFSR(&currentSolution, &bestSolution, &bestNeighbor);
		}
		ftime(&timeNow);
	}
	
	return NULL;
}

//TODO for these TODOs : cf python
void tabu_searchSwap(Solution **currentSolution, Solution **bestSolution, Solution **bestNeighbor)
{
	unsigned int bestVal = 0xFFFFFFFF;
	Solution *best
	for(unsigned int packIndex = 0; packIndex < currentSolution->packCount; packIndex++)
	{
		for(unsigned int taskIndex = 0; taskIndex < currentSolution->packList[packIndex]->taskCount; taskIndex++)
		{
			task_t task1 = currentSolution->packList[packIndex]->deliveries[taskIndex];
			for(unsigned int packIndex2 = packIndex; packIndex2 < currentSolution->packCount; packIndex2++)
			{
				for(unsigned int taskIndex2 = 0; taskIndex2 < currentSolution->packList[packIndex2]->taskCount; taskIndex2++)
				{
					task_t task2 = currentSolution->packList[packIndex2]->deliveries[taskIndex2];
					//TODO DELTA
					Solution * newSolution = sort_swapDeliveries(currentSolution, task1, task2);
					SolutionInfo *newInfo = solution_eval(newSolution);
					if(newInfo->score < bestVal) // TODO && notTabu(swap)
					{
						bestVal = newInfo->score;
						//TODO HERE
					}
				}
			}
		}
	}
}

void tabu_searchEBSR(Solution **currentSolution, Solution **bestSolution, Solution **bestNeighbor)
{
	
}

void tabu_searchEFSR(Solution **currentSolution, Solution **bestSolution, Solution **bestNeighbor)
{
	
}


