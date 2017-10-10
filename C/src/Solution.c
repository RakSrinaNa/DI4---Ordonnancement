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
	solution->processOrder = NULL;
	solution->score = -1;
	
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
		{
			pack_destroy(solution->packList[i]);
		}
		free(solution->packList);
		free(solution->processOrder);
		free(solution);
	}
}

Solution * solution_copy(Solution * solution)
{
	if(solution == NULL)
		return NULL;
	Solution * copy = solution_create(solution->instance);
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		copy->processOrder[i] = solution->processOrder[i];
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
		{
			solution_moveTaskPack(copy, solution->packList[i]->deliveryOrder[j], i);
		}
	}
	copy->score = solution->score;
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
		solution->score = -1;
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
		solution->score = -1;
	}
	else
		warn("WARNING : solution_setTaskPackIndex : given pack is out of range (%d)\n", pack);
}

void solution_sortByDD(Solution * solution)
{
	for(unsigned int i = 0; i < solution->instance->taskCount - 1; i++)
	{
		for(unsigned int j = 0; j < solution->instance->taskCount - i - 1; j++)
		{
			if(instance_getDueDate(solution->instance, solution->processOrder[j]) > instance_getDueDate(solution->instance, solution->processOrder[j + 1]))
			{
				unsigned int temp = solution->processOrder[j];
				solution->processOrder[j] = solution->processOrder[j + 1];
				solution->processOrder[j + 1] = temp;
			}
		}
	}
	solution->score = -1;
}

int solution_eval(Solution * solution)
{
	if(CACHED_SCORE && solution->score != -1)
		return solution->score;
	
	return 0;
}

unsigned int solution_processFinalTime(Instance * instance, unsigned int count, unsigned int * processes)
{
	unsigned int * machineEndTime = NULL;
	MMALLOC(machineEndTime, unsigned int, instance->machineCount, "solution_processFinalTime");
	for(unsigned int i = 0; i < count; i++)
		for(unsigned int m = 0; m < instance->machineCount; m++)
			machineEndTime[m] = MMAX(m == 0 ? 0 : machineEndTime[m - 1], machineEndTime[m]) + task_getMachineDuration(instance->tasks[processes[i]], m);
	unsigned int finalTime = machineEndTime[instance->machineCount - 1];
	free(machineEndTime);
	return finalTime;
}

unsigned int * solution_sequenceProcess(Instance * instance, unsigned int taskCount, unsigned int * pack)
{
	unsigned int * sequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(sequence, unsigned int, 1, "solution_sequenceProcess");
		sequence = memcpy(sequence, pack, sizeof(unsigned int));
	}
	else if(taskCount == 2)
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence10, unsigned int, taskCount, "solution_sequenceProcess");
		sequence01[0] = pack[0];
		sequence01[1] = pack[1];
		sequence10[0] = pack[1];
		sequence10[1] = pack[0];
		
		unsigned int sol01 = solution_processFinalTime(instance, taskCount, sequence01);
		unsigned int sol10 = solution_processFinalTime(instance, taskCount, sequence10);
		if(sol01 < sol10)
		{
			sequence = sequence01;
			free(sequence10);
		}
		else
		{
			sequence = sequence10;
			free(sequence01);
		}
	}
	
	else if(taskCount == 3)
	{
		unsigned int seqID = 0;
		unsigned int ** seqList = NULL;
		MMALLOC(seqList, unsigned int *, 6, "solution_sequenceProcess");
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
			{
				MMALLOC(seqList[seqID], unsigned int, 3, "solution_sequenceProcess");
				
				seqList[seqID][i] = pack[0];
				seqList[seqID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = pack[1];
				seqList[seqID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = pack[2];
				seqID++;
			}
		
		unsigned int * best = NULL;
		unsigned int bestTime = 0xFFFFFFFF;
		for(unsigned int i = 0; i < 6; i++)
		{
			unsigned int seqTime = solution_processFinalTime(instance, taskCount, seqList[i]);
			if(best == NULL || seqTime < bestTime)
			{
				free(best);
				best = seqList[i];
				bestTime = seqTime;
			}
			else
				free(seqList[i]);
		}
		free(seqList);
		sequence = best;
	}
	else
	{
		unsigned int * tempSequence;
		MMALLOC(tempSequence, unsigned int, 2, "solution_sequenceProcess");
		tempSequence[0] = pack[0];
		tempSequence[1] = pack[1];
		unsigned int * finalSequence = solution_sequenceProcess(instance, 2, tempSequence);
		free(tempSequence);
		unsigned int inside = 2;
		RREALLOC(finalSequence, unsigned int, taskCount, "solution_sequenceProcess");
		for(unsigned int taskID = 2; taskID < taskCount; taskID++)
		{
			unsigned int bestScore = 999999999;
			unsigned int bestPos = 0;
			for(unsigned int insertPos = 0; insertPos <= taskID; insertPos++)
			{
				MMALLOC(tempSequence, unsigned int, inside + 1, "solution_sequenceProcess");
				memcpy(tempSequence, finalSequence, sizeof(unsigned int) * insertPos);
				tempSequence[insertPos] = pack[taskID];
				memcpy(tempSequence + insertPos + 1, finalSequence + insertPos, sizeof(unsigned int) * (inside - insertPos));
				unsigned int tempTime = solution_processFinalTime(instance, inside + 1, tempSequence);
				if(tempTime < bestScore)
				{
					bestScore = tempTime;
					bestPos = insertPos;
				}
				free(tempSequence);
			}
			memmove(finalSequence + bestPos, finalSequence + bestPos + 1, inside - bestPos);
			finalSequence[bestPos] = taskID;
			inside++;
		}
		sequence = finalSequence;
	}
	return sequence;
}

void solution_print(Solution * solution)
{
	printf("\nSOLUTION\n\tOrder : ( ");
	for(unsigned int i = 0; i < solution->instance->taskCount; i++)
		printf("%d ", solution->processOrder[i]);
	printf(")\nPacks : %d\n", solution->packCount);
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		printf("\t");
		pack_print(solution->packList[i]);
		printf("\n");
	}
}
