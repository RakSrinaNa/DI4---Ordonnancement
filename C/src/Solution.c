#include <string.h>

#include "headers/Solution.h"

#include "FLAGS.h"

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
	
	debugPrint("Created solution : %p\n", solution);
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
	debugPrint("Destroyed solution : %p\n", solution);
}

Solution * solution_copy(Solution * solution)
{
	if(solution == NULL)
		return NULL;
	Solution * copy = solution_create(solution->instance);
	
	//Copy packs
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			solution_moveTaskPack(copy, solution->packList[i]->deliveries[j], i);
	debugPrint("Copied solution %p to %p\n", solution, copy);
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
			if(solution->packList[i]->deliveries[j] == task)
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
		RREALLOC(solution->packList, Pack*, solution->packCount, "solution_setTaskPackIndex");
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
		warn("solution_setTaskPackIndex : Pack %d does not exist.\n", pack);
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
	if(CACHED_SCORE && solution->info != NULL)
		return solution->info;
	
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
		printf("Solution :\n\tPacks : %d\n", solution->packCount);
		for(unsigned int i = 0; i < solution->packCount; i++)
		{
			printf("\t\t");
			pack_print(solution->packList[i]);
			printf("\n");
		}
		solutionInfo_print(solution, solution->info);
	}
	else
		printf("Solution : NULL\n");
}

void solution_save(Solution * solution, const char * filename)
{
	debugPrint("Saving solution %p to %s", solution, filename);
	if(solution->info == NULL)
		return;
	FILE * file = fopen(filename, "w");
	if(file != NULL)
	{
		fprintf(file, "%d %d %d\n", solution->instance->taskCount, solution->packCount, solution->info->score);
		for(unsigned int i = 0; i < solution->instance->taskCount; i++)
			fprintf(file, "%d ", solution->info->productionOrder[i]);
		fprintf(file, "\n");
		for(unsigned int i = 0; i < solution->packCount; i++)
		{
			fprintf(file, "%d ", solution->packList[i]->taskCount);
			for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			{
				fprintf(file, "%d ", solution->info->deliveries[i][j]);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}
