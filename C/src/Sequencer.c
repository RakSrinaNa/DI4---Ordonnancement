#include <string.h>
#include "headers/Sequencer.h"
#include "FLAGS.h"

unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, const task_t * tasks, unsigned int * machineReady)
{
	unsigned int * machineEndTime = NULL;
	MMALLOC(machineEndTime, machine_t, instance->machineCount, "sequencer_productionFinalTime");
	for(machine_t i = 0; i < instance->machineCount; i++)
		machineEndTime[i] = machineReady[i];
	
	//Update the ending time of a machine after the task i is processed.
	for(unsigned int taskIndex = 0; taskIndex < count; taskIndex++)
		for(machine_t m = 0; m < instance->machineCount; m++)
			machineEndTime[m] = MMAX(m == 0 ? 0 : machineEndTime[m - 1], machineEndTime[m]) + task_getMachineDuration(instance->tasks[tasks[taskIndex]], m);
	unsigned int finalTime = machineEndTime[instance->machineCount - 1];
	free(machineEndTime);
	debugPrint("Production time for %d tasks is %d\n", count, finalTime);
	return finalTime;
}

task_t * sequencer_sequenceProductionPack(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * machineReady, unsigned int * date)
{	
	debugPrint("Ordering production for %d tasks at %d\n", taskCount, *date);
	task_t * finalSequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(finalSequence, task_t, 1, "sequencer_sequenceProductionPack");
		finalSequence = memcpy(finalSequence, tasks, sizeof(task_t));
	}
	else if(taskCount == 2) //Try every case.
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, task_t, taskCount, "sequencer_sequenceProductionPack");
		MMALLOC(sequence10, task_t, taskCount, "sequencer_sequenceProductionPack");
		sequence01[0] = tasks[0];
		sequence01[1] = tasks[1];
		sequence10[0] = tasks[1];
		sequence10[1] = tasks[0];
		
		unsigned int score01 = sequencer_productionFinalTime(instance, taskCount, sequence01, machineReady);
		unsigned int score10 = sequencer_productionFinalTime(instance, taskCount, sequence10, machineReady);
		if(score01 <= score10)
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
		task_t ** sequenceList = NULL;
		MMALLOC(sequenceList, task_t  *, 6, "sequencer_sequenceProductionPack");
		for(unsigned int i = 0; i < 3; i++) //For every position of the first task
			for(unsigned int j = 0; j < 2; j++) //For every position of the second task.
			{
				MMALLOC(sequenceList[nextSequenceID], task_t, 3, "sequencer_sequenceProductionPack");
				
				sequenceList[nextSequenceID][i] = tasks[0];
				sequenceList[nextSequenceID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = tasks[1];
				sequenceList[nextSequenceID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = tasks[2];
				nextSequenceID++;
			}
		
		//Identify the best sequence.
		task_t * bestSequence = NULL;
		unsigned int bestTime = 0xFFFFFFFF;
		for(unsigned int sequenceIndex = 0; sequenceIndex < 6; sequenceIndex++)
		{
			unsigned int scoreSequence = sequencer_productionFinalTime(instance, taskCount, sequenceList[sequenceIndex], machineReady);
			if(bestSequence == NULL || scoreSequence < bestTime)
			{
				free(bestSequence);
				bestSequence = sequenceList[sequenceIndex];
				bestTime = scoreSequence;
			}
			else
				free(sequenceList[sequenceIndex]);
		}
		free(sequenceList);
		finalSequence = bestSequence;
	}
	else if(taskCount > 3)
	{
		task_t * tempSequence;
		MMALLOC(tempSequence, task_t, 2, "sequencer_sequenceProductionPack");
		tempSequence[0] = tasks[0];
		tempSequence[1] = tasks[1];
		unsigned int time = 0;
		task_t * bestSequence = sequencer_sequenceProductionPack(instance, 2, tempSequence, machineReady, &time); //Get the best order of the 2 first tasks.
		free(tempSequence);
		unsigned int inside = 2;
		RREALLOC(bestSequence, task_t, taskCount, "sequencer_sequenceProductionPack");
		for(unsigned int taskID = 2; taskID < taskCount; taskID++) //Try to insert every task.
		{
			unsigned int bestScore = 0xFFFFFFFF; // Infinity
			unsigned int bestPos = 0;
			for(unsigned int insertPos = 0; insertPos <= taskID; insertPos++) //Try to insert at every position.
			{
				//Build temp sequence with the task inserted.
				MMALLOC(tempSequence, task_t, inside + 1, "sequencer_sequenceProductionPack");
				memcpy(tempSequence, bestSequence, sizeof(task_t) * insertPos);
				tempSequence[insertPos] = tasks[taskID];
				memcpy(tempSequence + insertPos + 1, bestSequence + insertPos, sizeof(task_t) * (inside - insertPos));
				
				//Keep track of the best position.
				unsigned int tempScore = sequencer_productionFinalTime(instance, inside + 1, tempSequence, machineReady);
				if(tempScore < bestScore)
				{
					bestScore = tempScore;
					bestPos = insertPos;
				}
				free(tempSequence);
			}
			
			//Set the task at the best found position.
			memmove(bestSequence + bestPos + 1, bestSequence + bestPos, sizeof(task_t) * (inside - bestPos));
			bestSequence[bestPos] = taskID;
			inside++;
		}
		finalSequence = bestSequence;
	}
	*date += sequencer_productionFinalTime(instance, taskCount, finalSequence, machineReady);
	return finalSequence;
}

unsigned int sequencer_deliveryDelay(Instance * instance, unsigned int count, task_t * tasks, unsigned int * date)
{
	if(count == 0)
		return 0;
	unsigned int delay = 0;
	unsigned int departure = instance->taskCount;
	unsigned int arrival = tasks[0];
	for(unsigned int taskIndex = 0; taskIndex <= count; taskIndex++)
	{
		*date += instance_getDistance(instance, departure, arrival);
		delay += (taskIndex < count ? MMAX(0, (int) *date - (int) instance_getDueDate(instance, tasks[taskIndex])) : 0); // If arrival point is the origin, no delay added
		departure = arrival;
		arrival = (taskIndex < count - 1 ? tasks[taskIndex + 1] : instance->taskCount);
	}
	debugPrint("Delay for %d tasks is %d\n", count, delay);
	return delay;
}

task_t * sequencer_sequenceDeliveriesPack(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * initialDate)
{
	debugPrint("Ordering deliveries for %d tasks at %d\n", taskCount, *initialDate);
	unsigned int * sequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(sequence, task_t, 1, "sequencer_sequenceDeliveriesPack");
		sequence = memcpy(sequence, tasks, sizeof(task_t));
		*initialDate += instance_getDistance(instance, instance->taskCount, tasks[0]) + instance_getDistance(instance, instance->taskCount, tasks[0]);
	}
	else if(taskCount == 2) // Try every case.
	{
		task_t * sequence01 = NULL;
		task_t * sequence10 = NULL;
		MMALLOC(sequence01, task_t, taskCount, "sequencer_sequenceDeliveriesPack");
		MMALLOC(sequence10, task_t, taskCount, "sequencer_sequenceDeliveriesPack");
		sequence01[0] = tasks[0];
		sequence01[1] = tasks[1];
		sequence10[0] = tasks[1];
		sequence10[1] = tasks[0];
		
		unsigned int dateSol01 = *initialDate;
		unsigned int sol01 = sequencer_deliveryDelay(instance, taskCount, sequence01, &dateSol01);
		unsigned int dateSol10 = *initialDate;
		unsigned int sol10 = sequencer_deliveryDelay(instance, taskCount, sequence10, &dateSol10);
		if(sol01 <= sol10)
		{
			sequence = sequence01;
			free(sequence10);
			*initialDate = dateSol01;
		}
		else
		{
			sequence = sequence10;
			free(sequence01);
			*initialDate = dateSol10;
		}
	}
	else if(taskCount == 3) // Try every case.
	{
		unsigned int seqID = 0;
		unsigned int ** seqList = NULL;
		MMALLOC(seqList, task_t  *, 6, "sequencer_sequenceDeliveriesPack");
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
			{
				MMALLOC(seqList[seqID], task_t, 3, "sequencer_sequenceDeliveriesPack");
				
				seqList[seqID][i] = tasks[0];
				seqList[seqID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = tasks[1];
				seqList[seqID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = tasks[2];
				seqID++;
			}
		task_t * best = NULL;
		unsigned int bestTime = 0xFFFFFFFF; // Infinity
		unsigned int tempDate;
		unsigned int bestDate;
		for(unsigned int i = 0; i < 6; i++)
		{
			tempDate = *initialDate;
			unsigned int seqTime = sequencer_deliveryDelay(instance, taskCount, seqList[i], &tempDate);
			if(best == NULL || seqTime < bestTime)
			{
				free(best);
				best = seqList[i];
				bestTime = seqTime;
				bestDate = tempDate;
			}
			else
				free(seqList[i]);
		}
		*initialDate = bestDate;
		free(seqList);
		sequence = best;
	}
	else if(taskCount > 3)
	{
		if(DELIVERY_NEAREST_NEIGHBOR)
			sequence = sequencer_sequenceDeliveriesNearestNeighbor(instance, taskCount, tasks, initialDate);
		else
			sequence = sequencer_sequenceDeliveriesDueDate(instance, taskCount, tasks, initialDate);
	}
	return sequence;
}

task_t * sequencer_sequenceDeliveriesNearestNeighbor(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * initialDate)
{
	debugPrint("\tUsing nearest neighbor\n");
	task_t * sequence = NULL;
	MMALLOC(sequence, task_t, taskCount, "sequencer_sequenceDeliveriesNearestNeighbor");
	Bool * explored = NULL; // If we already delivered this node, we shouldn't deliver it again
	MMALLOC(explored, Bool, taskCount, "sequencer_sequenceDeliveriesNearestNeighbor");
	for(unsigned int i = 0; i < taskCount; i++)
		explored[i] = False;
	unsigned int departure = instance->taskCount;
	for(unsigned int i = 0; i < taskCount; i++)
	{
		// Finding the nearest neighbor at each step.
		unsigned int nearestIndex = 0xFFFFFFFF; // Infinity
		for(unsigned int j = 0; j < taskCount; j++)
		{
			if(!explored[j] && (nearestIndex == 0xFFFFFFFF || instance_getDistance(instance, departure, tasks[j]) < instance_getDistance(instance, departure, tasks[nearestIndex])))
			{
				nearestIndex = j;
			}
		}
		
		// Updating the sequence and distance according to the latest delivered node.
		explored[nearestIndex] = True;
		sequence[i] = tasks[nearestIndex];
		*initialDate += instance_getDistance(instance, departure, tasks[nearestIndex]);
		departure = tasks[nearestIndex];
	}
	*initialDate += instance_getDistance(instance, departure, instance->taskCount); // Going back home
	free(explored);
	return sequence;
}

task_t * sequencer_sequenceDeliveriesDueDate(Instance * instance, unsigned int taskCount, const task_t * tasks, unsigned int * initialDate)
{
	debugPrint("\tUsing smallest due date\n");
	task_t * sequence = NULL;
	MMALLOC(sequence, task_t, taskCount, "sequencer_sequenceDeliveriesDueDate");
	for(unsigned int i = 0; i < taskCount; i++)
		sequence[i] = tasks[i];
	
	// Sorting by due date.
	for(unsigned int i = 0; i < taskCount - 1; i++)
		for(unsigned int j = 0; j < taskCount - i - 1; j++)
			if(instance_getDueDate(instance, sequence[j]) > instance_getDueDate(instance, sequence[j + 1]))
			{
				task_t temp = sequence[j];
				sequence[j] = sequence[j + 1];
				sequence[j + 1] = temp;
			}
	
	sequencer_deliveryDelay(instance, taskCount, sequence, initialDate);
	return sequence;
}

