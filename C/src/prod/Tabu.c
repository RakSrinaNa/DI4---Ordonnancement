#include <sys/timeb.h>

#include "headers/Tabu.h"
#include "FLAGS.h"
#include "headers/Sort.h"

TabuSolution * tabuSolution_create(Solution * solution, unsigned int iterations, long double time)
{
	TabuSolution * tabuSolution = NULL;
	MMALLOC(tabuSolution, TabuSolution, 1, "tabu_search");
	tabuSolution->solution = solution;
	tabuSolution->iterations = iterations;
	tabuSolution->time = time;
	return tabuSolution;
}

void tabuSolution_destroy(TabuSolution * solution)
{
	solution_destroy(solution->solution);
	free(solution);
}

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
		if(bestSolution == NULL || solution_eval(solution)->score < solution_eval(bestSolution)->score)
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

TabuSolution * tabu_search(Instance * instance)
{
#ifdef DEV_LOG_SCORE
	FILE * logScoreFile = fopen("./logScores.csv", "w");
	fprintf(logScoreFile, "%s;%s\n", "C_BestEver", "C_BestIter");
#endif
	Solution * currentSolution = tabu_solutionInit(instance);
	Solution * bestSolution = currentSolution;
	solution_eval(currentSolution);
	unsigned int nbIterations = 0;
	unsigned int nbNoBetterIterations = 0;
	Bool diversification = False;
	TabuList * tabuList = tabuList_create(TABU_LIST_SIZE);
	
	struct timeb timeStart;
	ftime(&timeStart);
	struct timeb timeNow;
	ftime(&timeNow);
	double timeLimit = (instance->taskCount * instance->machineCount) / 4.0;
	long double currentTime = 0;
	while((currentTime = tabu_getTimeDiff(timeStart, timeNow)) < timeLimit || nbIterations < TABU_ITERATIONS)
	{
		debugPrint("----- Tabu iteration %d on solution %p\n", nbIterations, currentSolution);
		Solution * bestMethodSolution = NULL;
		if(TABU_SEARCH_SWAP)
		{
			Solution * iterSolutionSwap = tabu_searchSwap(currentSolution, tabuList, diversification);
			if(iterSolutionSwap != NULL)
			{
				solution_eval(iterSolutionSwap);
				bestMethodSolution = iterSolutionSwap;
			}
		}
		if(TABU_SEARCH_EBSR)
		{
			Solution * iterSolutionEBSR = tabu_searchEBSR(currentSolution, tabuList, diversification);
			if(iterSolutionEBSR != NULL)
			{
				solution_eval(iterSolutionEBSR);
				if(bestMethodSolution != NULL)
				{
					if(solution_eval(bestMethodSolution)->score > solution_eval(iterSolutionEBSR)->score)
					{
						solution_destroy(bestMethodSolution);
						bestMethodSolution = iterSolutionEBSR;
					}
					else
						solution_destroy(iterSolutionEBSR);
				}
				else
					bestMethodSolution = iterSolutionEBSR;
			}
		}
		if(TABU_SEARCH_EFSR)
		{
			Solution * iterSolutionEFSR = tabu_searchEFSR(currentSolution, tabuList, diversification);
			if(iterSolutionEFSR != NULL)
			{
				solution_eval(iterSolutionEFSR);
				if(bestMethodSolution != NULL)
				{
					if(solution_eval(bestMethodSolution)->score > solution_eval(iterSolutionEFSR)->score)
					{
						solution_destroy(bestMethodSolution);
						bestMethodSolution = iterSolutionEFSR;
					}
					else
						solution_destroy(iterSolutionEFSR);
				}
				else
					bestMethodSolution = iterSolutionEFSR;
			}
		}
		if(diversification == True)
		{
			diversification = False;
			nbNoBetterIterations = 0;
		}
		
		if(bestMethodSolution != NULL)
		{
			if(solution_eval(bestMethodSolution)->score < solution_eval(bestSolution)->score)
			{
				debugPrint("Found better solution %p, replacing %p\n", bestMethodSolution, bestSolution);
				solution_print(bestMethodSolution);
				if(bestSolution != currentSolution)
				{
					solution_destroy(bestSolution);
				}
				solution_destroy(currentSolution);
				bestSolution = bestMethodSolution;
				nbNoBetterIterations = 0;
			}
			else
			{
				if(currentSolution != bestSolution)
					solution_destroy(currentSolution);
				nbNoBetterIterations++;
			}
			currentSolution = bestMethodSolution;
		}
		else
		{
			diversification = True;
			nbNoBetterIterations++;
			warn("Found NULL solution\n");
		}
		
		if(nbNoBetterIterations > TABU_ITERATIONS_NOIMPROVE)
			diversification = True;
		#ifdef DEV_LOG_SCORE
			fprintf(logScoreFile, "%u;%u\n", (bestSolution == NULL) ? 0xFFFFFFFF : bestSolution->info->score, (bestMethodSolution == NULL) ? 0xFFFFFFFF : bestMethodSolution->info->score);
		#endif
		nbIterations++;
		ftime(&timeNow);
	}
	#ifdef DEV_LOG_SCORE
		fclose(logScoreFile);
	#endif
	debugPrint("Iterated %d times, with best solution %p\n", nbIterations, bestSolution);
	tabuList_destroy(tabuList);
	return tabuSolution_create(bestSolution, nbIterations, currentTime);
}

Solution * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	if(currentSolution == NULL)
		return NULL;
	unsigned int bestVal = (diversification ? 0 : 0xFFFFFFFF);
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
					/*if(taskIndex2 - taskIndex <= TABU_DELTA)
						continue;*/
					Solution * newSolution = sort_swapDeliveries(currentSolution, task1, task2);
					SolutionInfo * newInfo = solution_eval(newSolution);
					TabuItem item;
					item.source = task1;
					item.destination = task2;
					if((diversification == True ? (newInfo->score > bestVal) : (newInfo->score < bestVal)) && !tabuList_contains(tabuList, &item))
					{
						debugPrint("Solution %p is better than %p with swapped values %d and %d\n", currentSolution, newSolution, task1, task2);
						bestVal = newInfo->score;
						if(bestSolution != NULL)
							solution_destroy(bestSolution);
						bestSolution = newSolution;
						bestSwap1 = task1;
						bestSwap2 = task2;
					}
					else
						solution_destroy(newSolution);
				}
			}
		}
	}
	TabuItem * item = tabuItem_create(bestSwap1, bestSwap2);
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


