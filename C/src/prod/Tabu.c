#include <sys/timeb.h>
#include <limits.h>

#include "headers/Tabu.h"
#include "FLAGS.h"
#include "headers/Sort.h"
#include "headers/SearchResult.h"

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
	FILE * logScoreCompactFile = fopen("./logScoresCompact.csv", "w");
	fprintf(logScoreCompactFile, "%s;%s\n", "C_BestIter", "C_BestEver");
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
	while((currentTime = tabu_getTimeDiff(timeStart, timeNow)) < timeLimit && nbIterations < TABU_ITERATIONS)
	{
		debugPrint("----- Tabu iteration %d on solution %p\n", nbIterations, currentSolution);
		SearchResult * bestMethodResult = NULL;
		if(TABU_SEARCH_SWAP)
		{
			SearchResult * iterSolutionSwap = tabu_searchSwap(currentSolution, tabuList, diversification);
			if(iterSolutionSwap != NULL && iterSolutionSwap->tabuItem != NULL)
			{
				bestMethodResult = iterSolutionSwap;
			}
		}
		if(TABU_SEARCH_EBSR)
		{
			SearchResult * iterSolutionEBSR = tabu_searchEBSR(currentSolution, tabuList, diversification);
			if(iterSolutionEBSR != NULL && iterSolutionEBSR->tabuItem != NULL)
			{
				if(bestMethodResult != NULL)
				{
					if(solution_eval(bestMethodResult->solution)->score > solution_eval(iterSolutionEBSR->solution)->score)
					{
						searchResult_destroy(bestMethodResult);
						bestMethodResult = iterSolutionEBSR;
					}
					else
						searchResult_destroy(iterSolutionEBSR);
				}
				else
					bestMethodResult = iterSolutionEBSR;
			}
		}
		if(TABU_SEARCH_EFSR)
		{
			SearchResult * iterSolutionEFSR = tabu_searchEFSR(currentSolution, tabuList, diversification);
			if(iterSolutionEFSR != NULL)
			{
				if(bestMethodResult != NULL)
				{
					if(solution_eval(bestMethodResult->solution)->score > solution_eval(iterSolutionEFSR->solution)->score)
					{
						searchResult_destroy(bestMethodResult);
						bestMethodResult = iterSolutionEFSR;
					}
					else
						searchResult_destroy(iterSolutionEFSR);
				}
				else
					bestMethodResult = iterSolutionEFSR;
			}
		}
		if(diversification == True)
		{
			printf("DIV\n");
			diversification = False;
			nbNoBetterIterations = 0;
		}
		
		if(bestMethodResult != NULL)
		{
			if(solution_eval(bestMethodResult->solution)->score < solution_eval(bestSolution)->score)
			{
				debugPrint("Found better solution %p, replacing %p\n", bestMethodResult, bestSolution);
				solution_print(bestMethodResult->solution);
				if(bestSolution != currentSolution)
				{
					solution_destroy(bestSolution);
				}
				solution_destroy(currentSolution);
				bestSolution = solution_copy(bestMethodResult->solution);
				nbNoBetterIterations = 0;
#ifdef DEV_LOG_SCORE
				fprintf(logScoreCompactFile, "%u;%u\n", nbIterations + 1, bestSolution->info->score);
#endif
			}
			else
			{
				if(currentSolution != bestSolution)
					solution_destroy(currentSolution);
				nbNoBetterIterations++;
			}
			currentSolution = solution_copy(bestMethodResult->solution);
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
		fprintf(logScoreFile, "%u;%u\n", (bestSolution == NULL) ? 0xFFFFFFFF : bestSolution->info->score, (bestMethodResult == NULL && bestMethodResult->solution != NULL) ? 0xFFFFFFFF : solution_eval(bestMethodResult->solution)->score);
#endif
		searchResult_destroy(bestMethodResult);
		nbIterations++;
		ftime(&timeNow);
	}
#ifdef DEV_LOG_SCORE
	fclose(logScoreFile);
	fclose(logScoreCompactFile);
#endif
	debugPrint("Iterated %d times, with best solution %p\n", nbIterations, bestSolution);
	tabuList_destroy(tabuList);
	if(currentSolution != bestSolution)
		solution_destroy(currentSolution);
	return tabuSolution_create(bestSolution, nbIterations, currentTime);
}

Solution * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	if(TABU_RANDOM)
	{
		task_t t1 = 0;//TODO
		Solution * newSolution = sort_swapDeliveries(currentSolution, t1, 0);
		return newSolution;
	}
	if(currentSolution == NULL)
		return NULL;
	Solution * bestSolution = solution_copy(currentSolution);
	task_t bestSwap1 = 0, bestSwap2 = 0;
	Bool first_done = False;
	for(unsigned int packIndex1 = 0; packIndex1 < currentSolution->packCount; packIndex1++)
	{
		for(unsigned int taskIndex1 = 0; taskIndex1 < currentSolution->packList[packIndex1]->taskCount; taskIndex1++)
		{
			task_t task1 = currentSolution->packList[packIndex1]->deliveries[taskIndex1];
			for(unsigned int packIndex2 = packIndex1; packIndex2 < currentSolution->packCount; packIndex2++)
			{
				for(unsigned int taskIndex2 = 0; taskIndex2 < currentSolution->packList[packIndex2]->taskCount; taskIndex2++)
				{
					task_t task2 = currentSolution->packList[packIndex2]->deliveries[taskIndex2];
					if((taskIndex2 > taskIndex1 ? taskIndex2 - taskIndex1 + 1 : taskIndex1 - taskIndex2 + 1) <= TABU_DELTA)
						continue;
					Solution * newSolution = sort_swapDeliveries(currentSolution, task1, task2);
					TabuItem item;
					item.source = task1;
					item.destination = task2;
					if((diversification == True ? (solution_eval(newSolution)->score > solution_eval(bestSolution)->score) : (solution_eval(newSolution)->score < solution_eval(bestSolution)->score)) && !tabuList_contains(tabuList, &item))
					{
						debugPrint("Solution %p is better than %p with swapped values %d and %d\n", currentSolution, newSolution, task1, task2);
						if(bestSolution != NULL)
							solution_destroy(bestSolution);
						bestSolution = newSolution;
						bestSwap1 = task1;
						bestSwap2 = task2;
						if(TABU_FIRST_IMPROVE)
							first_done = True;
					}
					else
						solution_destroy(newSolution);
					if(TABU_FIRST_IMPROVE && first_done == True)
						break;
				}
				if(TABU_FIRST_IMPROVE && first_done == True)
					break;
			}
			if(TABU_FIRST_IMPROVE && first_done == True)
				break;
		}
		if(TABU_FIRST_IMPROVE && first_done == True)
			break;
	}
	TabuItem * item = tabuItem_create(bestSwap1, bestSwap2);
	tabuList_addItem(tabuList, item);
	return bestSolution;
}

SearchResult * tabu_searchEBSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	Bool stop = False;
	unsigned int packI = 0;
	Solution * bestSol = solution_copy(currentSolution);
	TabuItem * bestTabuItem = NULL;
	while(packI < currentSolution->packCount - 1 && !stop)
	{
		unsigned int packJ = packI + 1;
		while(packJ < currentSolution->packCount && packJ <= packI + TABU_DELTA_BATCH && !stop)
		{
			for(unsigned int packElemIndex = 0; packElemIndex < currentSolution->packList[packJ]->taskCount; packElemIndex++)
			{
				if(!stop)
				{
					task_t jobJ = currentSolution->packList[packJ]->deliveries[packElemIndex];
					Solution * neighbor = sort_moveDeliveriesEBSR(currentSolution, jobJ, packJ - packI);
					TabuItem * tabuItem = tabuItem_create(packI, jobJ);
					if(tabuList_contains(tabuList, tabuItem) && solutionCompare(bestSol, neighbor, diversification) > 0)
					{
						solution_destroy(bestSol);
						tabuItem_destroy(bestTabuItem);
						bestSol = neighbor;
						bestTabuItem = tabuItem;
#if TABU_FIRST_IMPROVE
						stop = True;
#endif
					}
					else
					{
						solution_destroy(neighbor);
						tabuItem_destroy(tabuItem);
					}
				}
			}
			packJ++;
		}
		packI++;
	}
	return searchResult_create(bestSol, bestTabuItem, EBSR);
}

Solution * tabu_searchEFSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	UNUSED(currentSolution);
	UNUSED(tabuList);
	UNUSED(diversification);
	return NULL;
}


