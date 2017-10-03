#include "headers/Solution.h"

Solution * solution_create(Instance * instance)
{
	Solution * solution = NULL;
	MMALLOC(solution, Solution, 1, "solution_create");
	solution->instance = instance;
	solution->packCount = 0;
	solution->packList = NULL;
	solution->processOrder = NULL;
	
	MMALLOC(solution->processOrder, unsigned int, solution->instance->taskCount, "solution_create");
	MMALLOC(solution->packList, Pack *, 1, "solution_create");
	solution->packList[0] = pack_create(instance);
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
	{
		solution->processOrder[i] = i;
		pack_addTask(solution->packList[0], i);
	}
	solution->packCount++;
	
	return solution;
}

void solution_destroy(Solution * solution)
{
	if(solution != NULL)
	{
		for(unsigned int i = 0; i < solution->packCount; i++)
			free(solution->packList[i]);
		free(solution->packList);
		free(solution->processOrder);
		free(solution);
	}
}

Solution * solution_copy(Solution *solution)
{
	if(solution == NULL)
		return NULL;
	Solution *copy = solution_create(solution->instance);
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		copy->processOrder[i] = solution->processOrder[i];
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
		{
			solution_moveTaskPack(copy, solution->packList[i]->deliveryOrder[j], i);
		}
	}
	solution_print(solution);
	solution_print(copy);
	return copy;
}

int solution_getProcessIndex(Solution * solution, unsigned int task)
{
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		if(solution->processOrder[i] == task)
			return (int) i;
	return -1;
}

void solution_setProcessIndex(Solution * solution, unsigned int task, unsigned int position)
{
	if(task < solution->instance->taskCount)
	{
		unsigned int index = (unsigned int) solution_getProcessIndex(solution, task);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < solution->instance->taskCount - direction; i += direction)
			solution->processOrder[i] = solution->processOrder[i + direction];
		solution->processOrder[MMIN(position, solution->instance->taskCount - 1)] = task;
	}
	else
		warn("WARNING : solution_setProcessIndex : given process does not exist (%d)\n", task);
}

int solution_getTaskPack(Solution * solution, unsigned int task)
{
	if(task >= solution->instance->taskCount)
	{
		warn("WARNING : solution_getTaskPack : given task does not exist (%d)\n", task);
		return -1;
	}
	for(unsigned int i = 0; i < solution->packCount; i++)
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
			if(solution->packList[i]->deliveryOrder[j] == task)
				return i;
	fatal_error("CRITICAL : solution_getTask : given task exists, but is not in any pack (%d)", task);
	return -1;
}

void solution_moveTaskPack(Solution * solution, unsigned int task, unsigned int pack)
{
	int temp = solution_getTaskPack(solution, task);
	if(temp == -1)
	{
		warn("WARNING : solution_moveTaskPack : given task does not exist (%d)\n");
		return;
	}
	unsigned int index = (unsigned int) temp;
	if(index == pack)
		return;
	if(pack == solution->packCount)
	{
		solution->packCount++;
		RREALLOC(solution->packList, Pack*, solution->packCount, "solution_setTaskPackIndex");
		solution->packList[solution->packCount - 1] = pack_create(solution->instance);
	}
	if(pack < solution->packCount)
	{
		pack_addTask(solution->packList[pack], task);
		if(pack_removeTask(solution->packList[index], task))
		{
			pack_destroy(solution->packList[index]);
			for(unsigned int i = index; i < solution->packCount - 1; i++)
				solution->packList[i] = solution->packList[i + 1];
			solution->packCount--;
			RREALLOC(solution->packList, Pack*, solution->packCount, "solution_moveTaskPack");
		}
	}
	else
		warn("WARNING : solution_setTaskPackIndex : given pack is out of range (%d)\n", pack);
}

void solution_sortByDD(Solution *solution)
{
	for(unsigned int i = 0; i < solution->instance->taskCount - 1; i++)
	{
		for(unsigned int j = 0; j < solution->instance->taskCount - i - 1; j++)
		{
			if(instance_getDueDate(solution->instance, solution->processOrder[j]) > instance_getDueDate(solution->instance, solution->processOrder[j+1]))
			{
				unsigned int temp = solution->processOrder[j];
				solution->processOrder[j] = solution->processOrder[j+1];
				solution->processOrder[j+1] = temp;
			}
		}
	}
}

int solution_eval(Solution *solution)
{
	UNUSED(solution);
	return 0;
}

void solution_print(Solution *solution)
{
	printf("\nSOLUTION\nPacks : %d\n", solution->packCount);
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		printf("\t");
		pack_print(solution->packList[i]);
		printf("\n");
	}
	printf("\tOrder : ( ");
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		printf("%d ", solution->processOrder[i]);
	printf(")\n");
}
