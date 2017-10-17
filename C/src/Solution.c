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
	solution->productionOrder = NULL;
	solution->score = -1;
	
	MMALLOC(solution->productionOrder, unsigned int, solution->instance->taskCount, "solution_create");
	
	//Create a pack and put every tasks inside it.
	MMALLOC(solution->packList, Pack *, 1, "solution_create");
	solution->packList[0] = pack_create(instance);
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
	{
		solution->productionOrder[i] = i;
		pack_addTask(solution->packList[0], i);
	}
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
	free(solution->productionOrder);
	free(solution);
}

Solution * solution_copy(Solution * solution)
{
	if(solution == NULL)
		return NULL;
	Solution * copy = solution_create(solution->instance);
	copy->score = solution->score;
	
	//Copy production order
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		copy->productionOrder[i] = solution->productionOrder[i];
	
	//Copy packs
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			solution_moveTaskPack(copy, solution->packList[i]->deliveryOrder[j], i);
	return copy;
}

int solution_getProductionIndex(Solution * solution, unsigned int task)
{
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		if(solution->productionOrder[i] == task)
			return (int) i;
	return -1;
}

void solution_setProductionIndex(Solution * solution, unsigned int task, unsigned int position)
{
	if(task < solution->instance->taskCount)
	{
		unsigned int index = (unsigned int) solution_getProductionIndex(solution, task);
		int direction = (position > index ? 1 : -1); //Direction of the iteration
		for(unsigned int i = index; i != position && i < solution->instance->taskCount - direction; i += direction)
			solution->productionOrder[i] = solution->productionOrder[i + direction];
		solution->productionOrder[MMIN(position, solution->instance->taskCount - 1)] = task; //If the index is greater than the list size, put it at the end.
		solution->score = -1;
	}
	else
		warn("solution_setProductionIndex : given process does not exist (%d)\n", task);
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
		solution->score = -1;
	}
	else
		warn("solution_setTaskPackIndex : given pack is out of range (%d)\n", pack);
}

void solution_sortByDD(Solution * solution)
{
	for(unsigned int i = 0; i < solution->instance->taskCount - 1; i++)
		for(unsigned int j = 0; j < solution->instance->taskCount - i - 1; j++)
			if(instance_getDueDate(solution->instance, solution->productionOrder[j]) > instance_getDueDate(solution->instance, solution->productionOrder[j + 1]))
			{
				unsigned int temp = solution->productionOrder[j];
				solution->productionOrder[j] = solution->productionOrder[j + 1];
				solution->productionOrder[j + 1] = temp;
			}
	solution->score = -1;
}

int solution_eval(Solution * solution)
{
	if(CACHED_SCORE && solution->score != -1) //We can use the cached score.
		return solution->score;
	
	//TODO
	
	return 0;
}

void solution_print(Solution * solution)
{
	printf("\nSOLUTION\n\tOrder : ( ");
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		printf("%d ", solution->productionOrder[i]);
	printf(")\nPacks : %d\n", solution->packCount);
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		printf("\t");
		pack_print(solution->packList[i]);
		printf("\n");
	}
}
