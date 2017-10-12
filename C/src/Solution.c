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
	fatal_error("solution_getTask : given task exists, but is not in any pack (%d)", task);
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

unsigned int solution_productionFinalTime(Instance * instance, unsigned int count, const unsigned int * tasks)
{
	unsigned int * machineEndTime = NULL;
	MMALLOC(machineEndTime, unsigned int, instance->machineCount, "solution_productionFinalTime");
	for(unsigned int i = 0; i < instance->machineCount; i++)
		machineEndTime[i] = 0;
	
	//Update the ending time of a machine after the task i is processed.
	for(unsigned int i = 0; i < count; i++)
		for(unsigned int m = 0; m < instance->machineCount; m++)
			machineEndTime[m] = MMAX(m == 0 ? 0 : machineEndTime[m - 1], machineEndTime[m]) + task_getMachineDuration(instance->tasks[tasks[i]], m);
	unsigned int finalTime = machineEndTime[instance->machineCount - 1];
	free(machineEndTime);
	return finalTime;
}

unsigned int * solution_sequenceProduction(Instance * instance, unsigned int taskCount, unsigned int * tasks)
{
	unsigned int * finalSequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(finalSequence, unsigned int, 1, "solution_sequenceProduction");
		finalSequence = memcpy(finalSequence, tasks, sizeof(unsigned int));
	}
	else if(taskCount == 2) //Try every case.
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, unsigned int, taskCount, "solution_sequenceProduction");
		MMALLOC(sequence10, unsigned int, taskCount, "solution_sequenceProduction");
		sequence01[0] = tasks[0];
		sequence01[1] = tasks[1];
		sequence10[0] = tasks[1];
		sequence10[1] = tasks[0];
		
		unsigned int score01 = solution_productionFinalTime(instance, taskCount, sequence01);
		unsigned int score10 = solution_productionFinalTime(instance, taskCount, sequence10);
		if(score01 < score10)
		{
			finalSequence = sequence01;
			free(sequence10);
		}
		else
		{
			finalSequence = sequence10;
			free(sequence01);
		}
	}
	else if(taskCount == 3) //Try every case.
	{
		unsigned int nextSequenceID = 0;
		unsigned int ** sequenceList = NULL;
		MMALLOC(sequenceList, unsigned int *, 6, "solution_sequenceProduction");
		for(unsigned int i = 0; i < 3; i++) //For every position of the first task
			for(unsigned int j = 0; j < 2; j++) //For every position of the second task.
			{
				MMALLOC(sequenceList[nextSequenceID], unsigned int, 3, "solution_sequenceProduction");
				
				sequenceList[nextSequenceID][i] = tasks[0];
				sequenceList[nextSequenceID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = tasks[1];
				sequenceList[nextSequenceID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = tasks[2];
				nextSequenceID++;
			}
		
		//Identify the best sequence.
		unsigned int * bestSequence = NULL;
		unsigned int bestTime = 0xFFFFFFFF;
		for(unsigned int i = 0; i < 6; i++)
		{
			unsigned int scoreSequence = solution_productionFinalTime(instance, taskCount, sequenceList[i]);
			if(bestSequence == NULL || scoreSequence < bestTime)
			{
				free(bestSequence);
				bestSequence = sequenceList[i];
				bestTime = scoreSequence;
			}
			else
				free(sequenceList[i]);
		}
		free(sequenceList);
		finalSequence = bestSequence;
	}
	else
	{
		unsigned int * tempSequence;
		MMALLOC(tempSequence, unsigned int, 2, "solution_sequenceProduction");
		tempSequence[0] = tasks[0];
		tempSequence[1] = tasks[1];
		unsigned int * bestSequence = solution_sequenceProduction(instance, 2, tempSequence); //Get the best order of the 2 first tasks.
		free(tempSequence);
		unsigned int inside = 2;
		RREALLOC(bestSequence, unsigned int, taskCount, "solution_sequenceProduction");
		for(unsigned int taskID = 2; taskID < taskCount; taskID++) //Try to insert every task.
		{
			unsigned int bestScore = 999999999;
			unsigned int bestPos = 0;
			for(unsigned int insertPos = 0; insertPos <= taskID; insertPos++) //Try to insert at every position.
			{
				//Build temp sequence with the task inserted.
				MMALLOC(tempSequence, unsigned int, inside + 1, "solution_sequenceProduction");
				memcpy(tempSequence, bestSequence, sizeof(unsigned int) * insertPos);
				tempSequence[insertPos] = tasks[taskID];
				memcpy(tempSequence + insertPos + 1, bestSequence + insertPos, sizeof(unsigned int) * (inside - insertPos));
				
				//Keep track of the best position.
				unsigned int tempScore = solution_productionFinalTime(instance, inside + 1, tempSequence);
				if(tempScore < bestScore)
				{
					bestScore = tempScore;
					bestPos = insertPos;
				}
				free(tempSequence);
			}
			
			//Set the task at the best found position.
			memmove(bestSequence + bestPos + 1, bestSequence + bestPos, sizeof(unsigned int) * (inside - bestPos));
			bestSequence[bestPos] = taskID;
			inside++;
		}
		finalSequence = bestSequence;
	}
	return finalSequence;
}

unsigned int solution_deliveryDelay(Instance * instance, unsigned int count, unsigned int * deliveries, unsigned int date)
{
	unsigned int delay = 0;
	unsigned int dep = instance->taskCount;
	unsigned int arr = deliveries[0];
	for(unsigned int i = 0; i <= count; i++)
	{
		date += instance_getDistance(instance, dep, arr);
		delay += (i < count ? MMAX(0, (int)date - (int)instance_getDueDate(instance, deliveries[i])) : 0);
		dep = arr;
		arr = (i < count - 1 ? deliveries[i + 1] : instance->taskCount);
	}
	return delay;
}

unsigned int * solution_sequenceDeliveries(Instance * instance, unsigned int taskCount, unsigned int * deliveries, unsigned int date)
{
	unsigned int * sequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(sequence, unsigned int, 1, "solution_sequenceDeliveries");
		sequence = memcpy(sequence, deliveries, sizeof(unsigned int));
	}
	else if(taskCount == 2)
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, unsigned int, taskCount, "solution_sequenceDeliveries");
		MMALLOC(sequence10, unsigned int, taskCount, "solution_sequenceDeliveries");
		sequence01[0] = deliveries[0];
		sequence01[1] = deliveries[1];
		sequence10[0] = deliveries[1];
		sequence10[1] = deliveries[0];
		
		unsigned int sol01 = solution_deliveryDelay(instance, taskCount, sequence01, date);
		unsigned int sol10 = solution_deliveryDelay(instance, taskCount, sequence10, date);
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
		MMALLOC(seqList, unsigned int *, 6, "solution_sequenceDeliveries");
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
			{
				MMALLOC(seqList[seqID], unsigned int, 3, "solution_sequenceDeliveries");
				
				seqList[seqID][i] = deliveries[0];
				seqList[seqID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = deliveries[1];
				seqList[seqID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = deliveries[2];
				seqID++;
			}
		unsigned int * best = NULL;
		unsigned int bestTime = 0xFFFFFFFF;
		for(unsigned int i = 0; i < 6; i++)
		{
			unsigned int seqTime = solution_deliveryDelay(instance, taskCount, seqList[i], date);
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
		printf("AAAAAAAA a remplir gros sac");
		// TODO
	}
	return sequence;
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
