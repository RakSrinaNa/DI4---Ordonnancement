#include <string.h>

#include "FLAGS.h"
#include "headers/Solution.h"

Solution * solution_create(Instance * instance)
{
	Solution * solution = NULL;
	MMALLOC(solution, Solution, 1, "solution_create");
	solution->instance = instance;
	solution->packCount = 0;
	solution->packList = NULL;
	solution->info = NULL;
	
	//Create a pack and put every tasks inside it.
	MMALLOC(solution->packList, Pack *, 1, "solution_create");
	solution->packList[0] = pack_create(instance);
	for(task_t i = 0; i < solution->instance->taskCount; i++)
		pack_addTask(solution->packList[0], i);
	solution->packCount++;
	
	debugPrint("Solution created : %p\n", solution);
	return solution;
}

void solution_destroy(Solution * solution)
{
	if(solution == NULL)
		return;
	//Destroy every pack.
	for(unsigned int i = 0; i < solution->packCount; i++)
		pack_destroy(solution->packList[i]);
	
	free(solution->packList);
	solutionInfo_destroy(solution, solution->info);
	free(solution);
	debugPrint("Solution destroyed : %p\n", solution);
}

Solution * solution_copy(Solution * solution)
{
	if(solution == NULL)
		return NULL;
	Solution * copy = solution_create(solution->instance);
	
	//Copy packs
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			solution_moveTaskPack(copy, solution->packList[i]->tasks[j], i+1);
	debugPrint("Solution %p copied to %p\n", solution, copy);
	return copy;
}

int solution_getTaskPack(Solution * solution, task_t task)
{
	if(task >= solution->instance->taskCount)
	{
		warn("solution_getTaskPack : Task %d does not exist.\n", task);
		return -1;
	}
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			if(solution->packList[i]->tasks[j] == task)
				return i;
	fatalError("solution_getTask : Task %d exists, but is not in any pack.\n", task);
	return -1;
}

void solution_moveTaskPack(Solution * solution, task_t task, unsigned int pack)
{
	debugPrint("Moving task %d to pack %d of solution %p\n", task, pack, solution);
	int temp = solution_getTaskPack(solution, task);
	if(temp == -1)
	{
		warn("solution_moveTaskPack : Task %d does not exist.\n");
		return;
	}
	unsigned int index = (unsigned int) temp;
	if(index == pack) //The task is already in the desired pack.
		return;
	if(pack == solution->packCount) //If we need to put it in a new pack, create it.
	{
		solution->packCount++;
		RREALLOC(solution->packList, Pack*, solution->packCount, "solution_moveTaskPack");
		solution->packList[solution->packCount - 1] = pack_create(solution->instance);
	}
	if(pack < solution->packCount)
	{
		solutionInfo_destroy(solution, solution->info);
		solution->info = NULL;
		pack_addTask(solution->packList[pack], task);
		if(pack_removeTask(solution->packList[index], task)) //If it leaves an empty pack, remove it.
		{
			pack_destroy(solution->packList[index]);
			
			//Fill the hole left by the deletion.
			for(unsigned int i = index; i < solution->packCount - 1; i++)
				solution->packList[i] = solution->packList[i + 1];
			solution->packCount--;
			RREALLOC(solution->packList, Pack*, solution->packCount, "solution_moveTaskPack");
		}
	}
	else
		warn("solution_moveTaskPack : Pack %d does not exist.\n", pack);
}

void solution_switchTaskPack(Solution * solution, task_t task1, task_t task2)
{
	debugPrint("Switching tasks %d and %d in solution %p\n", task1, task2, solution);
	int pack1 = solution_getTaskPack(solution, task1);
	int pack2 = solution_getTaskPack(solution, task2);
	if(pack1 < 0 || pack2 < 0)
	{
		warn("solution_switchTaskPack : Missing task (%d / %d).\n", task1, task2);
		return;
	}
	if(pack1 == pack2)
		return;
	solutionInfo_destroy(solution, solution->info);
	solution->info = NULL;
	solution_moveTaskPack(solution, task1, (unsigned int) pack2);
	solution_moveTaskPack(solution, task2, (unsigned int) pack1);
}

SolutionInfo * solution_eval(Solution * solution)
{
	debugPrint("Evaluation solution %p\n", solution);
#ifdef CACHED_SCORE
	if(solution->info != NULL)
		return solution->info;
#endif
	
	if(solution->info != NULL)
		solutionInfo_destroy(solution, solution->info);
	solution->info = solutionInfo_productionOrder(solution);
	solutionInfo_deliveryOrder(solution, solution->info);
	
	return solution->info;
}

void solution_print(Solution * solution)
{
	if(solution != NULL)
	{
		printf("Solution :\n");
		if(solution->info == NULL)
		{
			printf("\tPacks : %d\n", solution->packCount);
			for(unsigned int i = 0; i < solution->packCount; i++)
			{
				printf("\t\t");
				pack_print(solution->packList[i], i);
				printf("\n");
			}
		}
		solutionInfo_print(solution, solution->info);
	}
	else
		printf("Solution : NULL\n");
}

void solution_save(FILE * file, Solution * solution, long double time)
{
	debugPrint("Saving solution %p", solution);
	if(solution->info == NULL)
		return;
	if(file != NULL)
	{
		fprintf(file, "%d\t%Lf\t{ production:[ ", solution_eval(solution)->score, time);
		for(unsigned int i = 0; i < solution->instance->taskCount; i++)
			fprintf(file, "%d%c", solution->info->productionOrder[i], (i == solution->instance->taskCount - 1) ? ' ' : ',');
		fprintf(file, "], tasks:[ ");
		for(unsigned int i = 0; i < solution->packCount; i++)
		{
			fprintf(file, "[ ");
			for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			{
				fprintf(file, "%d%c", solution->info->deliveries[i][j], (j == solution->packList[i]->taskCount - 1) ? ' ' : ',');
			}
			fprintf(file, "]%c", (i == solution->packCount - 1) ? ' ' : ',');
		}
		fprintf(file, "] }\n");
	}
}

long solutionCompare(Solution * solution1, Solution * solution2, Bool diversification)
{
	if(solution1 == NULL && solution2 == NULL)
		return 0;
	if(solution1 == NULL)
		return -1;
	if(solution2 == NULL)
		return 1;
	if(diversification)
		return solution_eval(solution1)->score - solution_eval(solution2)->score;
	return ((long)solution_eval(solution2)->score) - solution_eval(solution1)->score;
}

void solution_printCSV(Solution * solution, FILE * file)
{
	if(solution == NULL)
		fprintf(file, "NULL");
	else
	{
		fprintf(file, "[");
		for(unsigned int packID = 0; packID < solution->packCount; packID++)
		{
			fprintf(file, "[");
			for(unsigned int taskIndex = 0; taskIndex < solution->packList[packID]->taskCount; taskIndex++)
			{
				fprintf(file, "%u", solution->packList[packID]->tasks[taskIndex]);
				if(taskIndex < solution->packList[packID]->taskCount - 1)
					fprintf(file, ", ");
			}
			fprintf(file, "]");
			if(packID < solution->packCount - 1)
				fprintf(file, ", ");
		}
		fprintf(file, "]");
	}
}
