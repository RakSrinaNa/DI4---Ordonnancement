#include <sys/timeb.h>

#include "headers/Tabu.h"
#include "headers/Instance.h"
#include "headers/Solution.h"
#include "FLAGS.h"
#include "headers/Sort.h"
#include "headers/TabuList.h"

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
	Solution * currentSolution = tabu_solutionInit(instance);
	Solution * bestSolution = currentSolution;
	solution_eval(currentSolution);
	unsigned int nbIterations = 0;
	unsigned int nbNoBetterIterations = 0;
	Bool diversification = False;
	TabuList *tabuList = tabuList_create(TABU_LIST_SIZE);
	
	struct timeb timeStart;
	ftime(&timeStart);
	struct timeb timeNow;
	ftime(&timeNow);
	double timeLimit = (instance->taskCount * instance->machineCount) / 4.0;
	debugPrint("Time : start at %d, now %d\n", timeStart.time, timeNow.time);
	while(tabu_getTimeDiff(timeStart, timeNow) < timeLimit && nbIterations < TABU_ITERATIONS)
	{
		debugPrint("Tabu iteration %d on solution %p\n", nbIterations, currentSolution);
		Solution * iterSolutionSwap = NULL;
		Solution * iterSolutionEBSR = NULL;
		Solution * iterSolutionEFSR = NULL;
		if(TABU_SEARCH_SWAP)
			iterSolutionSwap = tabu_searchSwap(currentSolution, tabuList, diversification);
		if(TABU_SEARCH_EBSR)
			iterSolutionEBSR = tabu_searchEBSR(currentSolution, tabuList, diversification);
		if(TABU_SEARCH_EFSR)
			iterSolutionEFSR = tabu_searchEFSR(currentSolution, tabuList, diversification);
		if(diversification == True)
		{
			diversification = False;
			nbNoBetterIterations = 0;
		}
		
		if(iterSolutionSwap != NULL && iterSolutionEBSR != NULL && iterSolutionEFSR != NULL)
		{
			solution_eval(iterSolutionSwap);
			solution_eval(iterSolutionEBSR);
			solution_eval(iterSolutionEFSR);
			Solution *bestMethodSolution = iterSolutionSwap;
			if(iterSolutionSwap->info->score > iterSolutionEBSR->info->score)
				bestMethodSolution = iterSolutionSwap;
			if(bestMethodSolution->info->score > iterSolutionEFSR->info->score)
				bestMethodSolution = iterSolutionEFSR;
			
			if(bestMethodSolution->info->score < bestSolution->info->score)
			{
				debugPrint("Found better solution %p, replacing %p\n", bestMethodSolution, bestSolution);
				solution_destroy(bestSolution);
				bestSolution = bestMethodSolution;
				if(bestMethodSolution != iterSolutionSwap)
					solution_destroy(iterSolutionSwap);
				if(bestMethodSolution != iterSolutionEBSR)
					solution_destroy(iterSolutionEBSR);
				if(bestMethodSolution != iterSolutionEFSR)
					solution_destroy(iterSolutionEFSR);
				nbNoBetterIterations = 0;
			}
			else
				nbNoBetterIterations++;
		}
		else
		{
			diversification = True;
			nbNoBetterIterations++;
			warn("Found NULL solution\n");
		}
		
		if(nbNoBetterIterations > TABU_ITERATIONS_NOIMPROVE)
			diversification = True;
		
		nbIterations++;
		ftime(&timeNow);
	}
	debugPrint("Iterated %d times, with best solution %p\n", nbIterations, bestSolution);
	return bestSolution;
}

Solution * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	unsigned int bestVal = 0xFFFFFFFF;
	Solution * bestSolution = NULL;
	task_t bestSwap1 = 0, bestSwap2 = 0;
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
					if(taskIndex2 - taskIndex < TABU_DELTA)
						continue;
					Solution * newSolution = sort_swapDeliveries(currentSolution, task1, task2);
					SolutionInfo * newInfo = solution_eval(newSolution);
					TabuItem item; 
					item.source = task1; item.destination = task2;
					if((diversification == True ? (newInfo->score < bestVal) : (newInfo->score > bestVal)) && tabuList_contains(tabuList, &item))
					{
						debugPrint("Solution %p is better than %p with swapped values %d and %d\n", currentSolution, newSolution, task1, task2);
						bestVal = newInfo->score;
						if(bestSolution != NULL)
							solution_destroy(bestSolution);
						bestSolution = newSolution;
						bestSwap1 = task1;
						bestSwap2 = task2;
					}
				}
			}
		}
	}
	TabuItem *item = tabuItem_create(bestSwap1, bestSwap2);
	tabuList_addItem(tabuList, item);
	return bestSolution;
}

Solution * tabu_searchEBSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	UNUSED(currentSolution);
	UNUSED(tabuList);
	UNUSED(diversification);
	return NULL;
}

Solution * tabu_searchEFSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	UNUSED(currentSolution);
	UNUSED(tabuList);
	UNUSED(diversification);
	return NULL;
}


