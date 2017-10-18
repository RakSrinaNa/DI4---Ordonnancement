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
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		pack_addTask(solution->packList[0], i);
	solution->packCount++;
	
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
	//TODO solutionInfo_destroy(solution->info);
	free(solution);
}

Solution * solution_copy(Solution * solution)
{
	if(solution == NULL)
		return NULL;
	Solution * copy = solution_create(solution->instance);
	
	//Copy packs
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			solution_moveTaskPack(copy, solution->packList[i]->deliveryOrder[j], i);
	return copy;
}

int solution_getTaskPack(Solution * solution, unsigned int task)
{
	if(task >= solution->instance->taskCount)
	{
		warn("solution_getTaskPack : given task does not exist (%d)\n", task);
		return -1;
	}
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			if(solution->packList[i]->deliveryOrder[j] == task)
				return i;
	fatalError("solution_getTask : given task exists, but is not in any pack (%d)", task);
	return -1;
}

void solution_moveTaskPack(Solution * solution, unsigned int task, unsigned int pack)
{
	int temp = solution_getTaskPack(solution, task);
	if(temp == -1)
	{
		warn("solution_moveTaskPack : given task does not exist (%d)\n");
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
		warn("solution_setTaskPackIndex : given pack is out of range (%d)\n", pack);
}

int solution_eval(Solution * solution)
{
	UNUSED(solution);
	if(CACHED_SCORE)
		return 0;
	
	//TODO
	
	return 0;
}

void solution_print(Solution * solution)
{
	printf("\nSOLUTION\nPacks : %d\n", solution->packCount);
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		printf("\t");
		pack_print(solution->packList[i]);
		printf("\n");
	}
}
