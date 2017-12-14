#include <sys/timeb.h>
#include <limits.h>

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
	Solution * currentSolution = tabu_solutionInit(instance);
	printf("Initial solution:\n");
	solution_print(currentSolution);
#ifdef DEV_LOG_SCORE
	FILE * logScoreFile = fopen("./logScores.csv", "w");
	fprintf(logScoreFile, "%s;%s\n", "C_BestEver", "C_BestIter");
	FILE * logScoreCompactFile = fopen("./logScoresCompact.csv", "w");
	fprintf(logScoreCompactFile, "%s;%s\n", "C_BestIter", "C_BestEver");
	FILE * logScoreFullFile = fopen("./logScoresFull.csv", "w");
	fprintf(logScoreFullFile, "%s;%s;%s\n", "C_Iter", "C_Current", "C_Score");
	fprintf(logScoreFullFile, "0;");
	solution_printCSV(currentSolution, logScoreFullFile);
	fprintf(logScoreFullFile, ";%u\n", solution_eval(currentSolution)->score);
#endif
	Solution * bestSolution = solution_copy(currentSolution);
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
				if(TABU_PROPAGATE)
				{
					solution_destroy(currentSolution);
					currentSolution = solution_copy(bestMethodResult->solution);
				}
			}
			else
				searchResult_destroy(iterSolutionSwap);
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
				if(TABU_PROPAGATE)
				{
					solution_destroy(currentSolution);
					currentSolution = solution_copy(bestMethodResult->solution);
				}
			}
			else
				searchResult_destroy(iterSolutionEBSR);
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
				if(TABU_PROPAGATE)
				{
					solution_destroy(currentSolution);
					currentSolution = solution_copy(bestMethodResult->solution);
				}
			}
			else
				searchResult_destroy(iterSolutionEFSR);
		}
		if(diversification == True)
		{
			printf("DIV\n");
			diversification = False;
			nbNoBetterIterations = 0;
		}
		
		if(bestMethodResult != NULL)
		{
			if(bestMethodResult->tabuItem != NULL)
			{
				tabuList_addItem(tabuList, tabuItem_copy(bestMethodResult->tabuItem));
			}
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
				fprintf(logScoreCompactFile, "%u;%u\n", nbIterations + 1, solution_eval(bestSolution)->score);
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
		else if(TABU_DIVERSIFICATION)
		{
			diversification = True;
			tabuList_clear(tabuList);
			nbNoBetterIterations++;
		}
		printf("%d\n",solution_eval(currentSolution)->score);
		
		if(TABU_DIVERSIFICATION && nbNoBetterIterations > TABU_ITERATIONS_NOIMPROVE)
		{
			diversification = True;
			tabuList_clear(tabuList);
		}
#ifdef DEV_LOG_SCORE
		fprintf(logScoreFile, "%u;%u\n", (bestSolution == NULL) ? UINT_MAX : solution_eval(bestSolution)->score, (bestMethodResult == NULL || bestMethodResult->solution == NULL) ? UINT_MAX : solution_eval(bestMethodResult->solution)->score);
		fprintf(logScoreFullFile, "%u;", nbIterations + 1);
		solution_printCSV(currentSolution, logScoreFullFile);
		fprintf(logScoreFullFile, ";%u\n", solution_eval(currentSolution)->score);
#endif
		searchResult_destroy(bestMethodResult);
		printf("%d: ", nbIterations);
		tabuList_print(tabuList);
		printf("\n");
		nbIterations++;
		ftime(&timeNow);
	}

#ifdef DEV_LOG_SCORE
	fclose(logScoreFile);
	fclose(logScoreCompactFile);
	fclose(logScoreFullFile);
#endif
	debugPrint("Iterated %d times, with best solution %p\n", nbIterations, bestSolution);
	tabuList_destroy(tabuList);
	if(currentSolution != bestSolution)
		solution_destroy(currentSolution);
	return tabuSolution_create(bestSolution, nbIterations, currentTime);
}

SearchResult * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	if(TABU_RANDOM)
	{
		task_t t1 = 0;//TODO
		task_t t2 = 0;//TODO
		Solution * newSolution = sort_swapDeliveries(currentSolution, t1, t2);
		return searchResult_create(newSolution, tabuItem_create(t1, t2, SWAP), SWAP);
	}
	if(currentSolution == NULL)
		return searchResult_create(NULL, NULL, SWAP);
	Solution * bestSolution = solution_copy(currentSolution);
	TabuItem * bestItem = NULL;
	Bool first_done = False;
	for(unsigned int packIndex1 = 0; packIndex1 < currentSolution->packCount; packIndex1++)
	{
		for(unsigned int taskIndex1 = 0; taskIndex1 < currentSolution->packList[packIndex1]->taskCount; taskIndex1++)
		{
			task_t task1 = currentSolution->packList[packIndex1]->tasks[taskIndex1];
			for(unsigned int packIndex2 = (TABU_LOGIC ? packIndex1 : 0); packIndex2 < currentSolution->packCount; packIndex2++)
			{
				for(unsigned int taskIndex2 = 0; taskIndex2 < currentSolution->packList[packIndex2]->taskCount; taskIndex2++)
				{
					task_t task2 = currentSolution->packList[packIndex2]->tasks[taskIndex2];
					if(task1 == task2)
						continue;
					if((taskIndex2 > taskIndex1 ? taskIndex2 - taskIndex1 + 1 : taskIndex1 - taskIndex2 + 1) > TABU_DELTA)
						continue;
					Solution * newSolution = sort_swapDeliveries(currentSolution, task1, task2);
					TabuItem * item = tabuItem_create(task1, task2, SWAP);
					if(solutionCompare(bestSolution, newSolution, diversification) < 0 && !tabuList_contains(tabuList, item))
					{
						debugPrint("Solution %p is better than %p with swapped values %d and %d\n", currentSolution, newSolution, task1, task2);
						if(bestSolution != NULL)
							solution_destroy(bestSolution);
						tabuItem_destroy(bestItem);
						bestItem = item;
						bestSolution = newSolution;
						if(TABU_FIRST_IMPROVE)
							first_done = True;
					}
					else
					{
						solution_destroy(newSolution);
						tabuItem_destroy(item);
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
		if(TABU_FIRST_IMPROVE && first_done == True)
			break;
	}
	return searchResult_create(bestSolution, bestItem, SWAP);
}

SearchResult * tabu_searchEBSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	if(currentSolution == NULL)
		return searchResult_create(NULL, NULL, EBSR);
	Bool stop = False;
	unsigned int packIndex1 = 0;
	Solution * bestSol = solution_copy(currentSolution);
	TabuItem * bestTabuItem = NULL;
	while(packIndex1 < currentSolution->packCount - 1 && !stop)
	{
		unsigned int packIndex2 = packIndex1 + 1;
		while(packIndex2 < currentSolution->packCount && packIndex2 <= packIndex1 + TABU_DELTA_BATCH && !stop)
		{
			for(unsigned int packElemIndex = 0; packElemIndex < currentSolution->packList[packIndex2]->taskCount; packElemIndex++)
			{
				if(!stop)
				{
					task_t taskID = currentSolution->packList[packIndex2]->tasks[packElemIndex];
					Solution * neighbor = sort_moveDeliveriesEBSR(currentSolution, taskID, packIndex2 - packIndex1);
					TabuItem * tabuItem = tabuItem_create(packIndex1, taskID, EBSR);
					if(!tabuList_contains(tabuList, tabuItem) && solutionCompare(bestSol, neighbor, diversification) < 0)
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
			packIndex2++;
		}
		packIndex1++;
	}
	return searchResult_create(bestSol, bestTabuItem, EBSR);
}

SearchResult * tabu_searchEFSR(Solution * currentSolution, TabuList * tabuList, Bool diversification)
{
	if(currentSolution == NULL)
		return searchResult_create(NULL, NULL, EFSR);
	Bool stop = False;
	unsigned int packIndex1 = 0;
	Solution * bestSol = solution_copy(currentSolution);
	TabuItem * bestTabuItem = NULL;
	while(packIndex1 < currentSolution->packCount - 1 && !stop)
	{
		for(unsigned int packElemIndex = 0; packElemIndex < currentSolution->packList[packIndex1]->taskCount; packElemIndex++)
		{
			task_t taskID = currentSolution->packList[packIndex1]->tasks[packElemIndex];
			unsigned int packIndex2 = packIndex1 + 1;
			while(packIndex2 < currentSolution->packCount && packIndex2 <= packIndex1 + TABU_DELTA_BATCH && !stop)
			{
				Solution * neighbor = sort_moveDeliveriesEFSR(currentSolution, taskID, packIndex2 - packIndex1);
				TabuItem * tabuItem = tabuItem_create(taskID, packIndex2, EFSR);
				if(!tabuList_contains(tabuList, tabuItem) && solutionCompare(bestSol, neighbor, diversification) < 0)
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
				packIndex2++;
			}
		}
		packIndex1++;
	}
	printf("%d\n", bestSol->info->score);
	return searchResult_create(bestSol, bestTabuItem, EFSR);
}
