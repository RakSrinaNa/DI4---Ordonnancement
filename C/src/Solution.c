#include <string.h>

#include "headers/Solution.h"
#include "headers/Instance.h"

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
	UNUSED(instance);
	UNUSED(processes);
	// TODO
	return 0;
}

unsigned int * solution_sequenceProcess(Instance *instance, unsigned int taskCount, unsigned int *pack)
{
	unsigned int * sequence;
	if(taskCount == 2)
	{
		unsigned int *sequence01 = NULL, *sequence10 = NULL;
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
		unsigned int *sequence012 = NULL, *sequence021 = NULL, *sequence102 = NULL, *sequence120 = NULL, *sequence201 = NULL, *sequence210 = NULL;
		MMALLOC(sequence012, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence021, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence102, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence120, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence201, unsigned int, taskCount, "solution_sequenceProcess");
		MMALLOC(sequence210, unsigned int, taskCount, "solution_sequenceProcess");
		sequence012[0] = pack[0];
		sequence012[1] = pack[1];
		sequence012[2] = pack[2];
		sequence021[0] = pack[0];
		sequence021[1] = pack[2];
		sequence021[2] = pack[1];
		sequence102[0] = pack[1];
		sequence102[1] = pack[0];
		sequence102[2] = pack[2];
		sequence120[0] = pack[1];
		sequence120[1] = pack[2];
		sequence120[2] = pack[0];
		sequence201[0] = pack[2];
		sequence201[1] = pack[0];
		sequence201[2] = pack[1];
		sequence210[0] = pack[2];
		sequence210[1] = pack[1];
		sequence210[2] = pack[0];
		
		unsigned int * best = sequence012;
		unsigned int bestScore = solution_processFinalTime(instance, taskCount, sequence012);
		unsigned int seqScore = 0;
		
		seqScore = solution_processFinalTime(instance, taskCount, sequence021);
		if(seqScore < bestScore)
		{
			free(best);
			best = sequence021;
			bestScore = seqScore;
		} else {
			free(sequence021);
		}
		seqScore = solution_processFinalTime(instance, taskCount, sequence102);
		if(seqScore < bestScore)
		{
			free(best);
			best = sequence102;
			bestScore = seqScore;
		} else {
			free(sequence102);
		}
		seqScore = solution_processFinalTime(instance, taskCount, sequence120);
		if(seqScore < bestScore)
		{
			free(best);
			best = sequence120;
			bestScore = seqScore;
		} else {
			free(sequence120);
		}
		seqScore = solution_processFinalTime(instance, taskCount, sequence201);
		if(seqScore < bestScore)
		{
			free(best);
			best = sequence201;
			bestScore = seqScore;
		} else {
			free(sequence201);
		}
		seqScore = solution_processFinalTime(instance, taskCount, sequence210);
		if(seqScore < bestScore)
		{
			free(best);
			best = sequence210;
			bestScore = seqScore;
		} else {
			free(sequence210);
		}
		sequence = best;
		// TODO !!!
		
		
	}
	else if(taskCount > 3)
	{
		unsigned int * tempSequence;
		MMALLOC(tempSequence, unsigned int, 2, "solution_sequenceProcess");
		tempSequence[0] = pack[0];
		tempSequence[1] = pack[1];
		unsigned int * finalSequence = solution_sequenceProcess(instance, 2, tempSequence);
		free(tempSequence);
		unsigned int inside = 2;
		RREALLOC(finalSequence, unsigned int, taskCount, "solution_sequenceProcess");
		for(unsigned int i = 2; i < taskCount; i++)
		{
			unsigned int bestScore = -1;
			unsigned int bestPos = 0;
			for(unsigned int j = 0; j <= i; j++)
			{
				MMALLOC(tempSequence, unsigned int, inside + 1, "solution_sequenceProcess");
				memcpy(tempSequence, finalSequence, sizeof(unsigned int) * j);
				tempSequence[j] = pack[i];
				memcpy(tempSequence + j + 1, finalSequence + j, sizeof(unsigned int) * (inside - j));
				unsigned int tempScore = solution_processFinalTime(instance, inside + 1, tempSequence);
				if(tempScore < bestScore)
				{
					bestScore = tempScore;
					bestPos = j;
				}
				free(tempSequence);
			}
			memmove(finalSequence + bestPos, finalSequence + bestPos + 1, inside - bestPos);
			finalSequence[bestPos] = i;
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
