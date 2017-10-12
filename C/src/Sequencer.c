#include <string.h>
#include "headers/Sequencer.h"
#include "FLAGS.h"

unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, const unsigned int * tasks)
{
	unsigned int * machineEndTime = NULL;
	MMALLOC(machineEndTime, unsigned int, instance->machineCount, "sequencer_productionFinalTime");
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

unsigned int * sequencer_sequenceProduction(Instance * instance, unsigned int taskCount, unsigned int * tasks)
{
	unsigned int * finalSequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(finalSequence, unsigned int, 1, "sequencer_sequenceProduction");
		finalSequence = memcpy(finalSequence, tasks, sizeof(unsigned int));
	}
	else if(taskCount == 2) //Try every case.
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, unsigned int, taskCount, "sequencer_sequenceProduction");
		MMALLOC(sequence10, unsigned int, taskCount, "sequencer_sequenceProduction");
		sequence01[0] = tasks[0];
		sequence01[1] = tasks[1];
		sequence10[0] = tasks[1];
		sequence10[1] = tasks[0];
		
		unsigned int score01 = sequencer_productionFinalTime(instance, taskCount, sequence01);
		unsigned int score10 = sequencer_productionFinalTime(instance, taskCount, sequence10);
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
		unsigned int ** sequenceList = NULL;
		MMALLOC(sequenceList, unsigned int *, 6, "sequencer_sequenceProduction");
		for(unsigned int i = 0; i < 3; i++) //For every position of the first task
			for(unsigned int j = 0; j < 2; j++) //For every position of the second task.
			{
				MMALLOC(sequenceList[nextSequenceID], unsigned int, 3, "sequencer_sequenceProduction");
				
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
			unsigned int scoreSequence = sequencer_productionFinalTime(instance, taskCount, sequenceList[i]);
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
	else if(taskCount > 3)
	{
		unsigned int * tempSequence;
		MMALLOC(tempSequence, unsigned int, 2, "sequencer_sequenceProduction");
		tempSequence[0] = tasks[0];
		tempSequence[1] = tasks[1];
		unsigned int * bestSequence = sequencer_sequenceProduction(instance, 2, tempSequence); //Get the best order of the 2 first tasks.
		free(tempSequence);
		unsigned int inside = 2;
		RREALLOC(bestSequence, unsigned int, taskCount, "sequencer_sequenceProduction");
		for(unsigned int taskID = 2; taskID < taskCount; taskID++) //Try to insert every task.
		{
			unsigned int bestScore = 999999999;
			unsigned int bestPos = 0;
			for(unsigned int insertPos = 0; insertPos <= taskID; insertPos++) //Try to insert at every position.
			{
				//Build temp sequence with the task inserted.
				MMALLOC(tempSequence, unsigned int, inside + 1, "sequencer_sequenceProduction");
				memcpy(tempSequence, bestSequence, sizeof(unsigned int) * insertPos);
				tempSequence[insertPos] = tasks[taskID];
				memcpy(tempSequence + insertPos + 1, bestSequence + insertPos, sizeof(unsigned int) * (inside - insertPos));
				
				//Keep track of the best position.
				unsigned int tempScore = sequencer_productionFinalTime(instance, inside + 1, tempSequence);
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

unsigned int sequencer_deliveryDelay(Instance * instance, unsigned int count, unsigned int * tasks, unsigned int date)
{
	unsigned int delay = 0;
	unsigned int departure = instance->taskCount;
	unsigned int arrival = tasks[0];
	for(unsigned int i = 0; i <= count; i++)
	{
		date += instance_getDistance(instance, departure, arrival);
		delay += (i < count ? MMAX(0, (int) date - (int) instance_getDueDate(instance, tasks[i])) : 0);
		departure = arrival;
		arrival = (i < count - 1 ? tasks[i + 1] : instance->taskCount);
	}
	return delay;
}

unsigned int * sequencer_sequenceDeliveries(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int initialDate)
{
	unsigned int * sequence = NULL;
	if(taskCount == 1)
	{
		MMALLOC(sequence, unsigned int, 1, "sequencer_sequenceDeliveries");
		sequence = memcpy(sequence, tasks, sizeof(unsigned int));
	}
	else if(taskCount == 2)
	{
		unsigned int * sequence01 = NULL;
		unsigned int * sequence10 = NULL;
		MMALLOC(sequence01, unsigned int, taskCount, "sequencer_sequenceDeliveries");
		MMALLOC(sequence10, unsigned int, taskCount, "sequencer_sequenceDeliveries");
		sequence01[0] = tasks[0];
		sequence01[1] = tasks[1];
		sequence10[0] = tasks[1];
		sequence10[1] = tasks[0];
		
		unsigned int sol01 = sequencer_deliveryDelay(instance, taskCount, sequence01, initialDate);
		unsigned int sol10 = sequencer_deliveryDelay(instance, taskCount, sequence10, initialDate);
		if(sol01 <= sol10)
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
		MMALLOC(seqList, unsigned int *, 6, "sequencer_sequenceDeliveries");
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
			{
				MMALLOC(seqList[seqID], unsigned int, 3, "sequencer_sequenceDeliveries");
				
				seqList[seqID][i] = tasks[0];
				seqList[seqID][i == 0 ? (j + 1) : (i == 1 && j == 1 ? 2 : j)] = tasks[1];
				seqList[seqID][(i + j == 0) ? 2 : ((j == 1 && i == 0) ? 1 : ((i == 1 && j == 0) ? 2 : ((i == 1 && j == 1) ? 0 : ((i == 2 && j == 0) ? 1 : 0))))] = tasks[2];
				seqID++;
			}
		unsigned int * best = NULL;
		unsigned int bestTime = 0xFFFFFFFF;
		for(unsigned int i = 0; i < 6; i++)
		{
			unsigned int seqTime = sequencer_deliveryDelay(instance, taskCount, seqList[i], initialDate);
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
	else if(taskCount > 3)
	{
		if(DELIVERY_NEAREST_NEIGHBOR)
			sequence = sequencer_sequenceDeliveriesNearestNeighbor();
		else
			sequence = sequencer_sequenceDeliveriesDueDate();
		printf("AAAAAAAA a remplir gros sac");
		//TODO @Schttopup
	}
	return sequence;
}

unsigned int * sequencer_sequenceDeliveriesNearestNeighbor()
{
	return NULL; //TODO @Schttopup
}

unsigned int * sequencer_sequenceDeliveriesDueDate()
{
	return NULL; //TODO @Schttopup
}
