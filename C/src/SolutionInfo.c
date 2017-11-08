#include "headers/SolutionInfo.h"

#include <string.h>

SolutionInfo * solutionInfo_create(Solution * solution)
{
	SolutionInfo * info = NULL;
	MMALLOC(info, SolutionInfo, 1, "solutionInfo_create");
	MMALLOC(info->productionOrder, task_t, solution->instance->taskCount, "solutionInfo_create");
	MMALLOC(info->readyToDeliver, unsigned int, solution->packCount, "solutionInfo_create");
	MMALLOC(info->deliveries, task_t  *, solution->packCount, "solutionInfo_create");
	for(unsigned int i = 0; i < solution->packCount; i++)
		info->deliveries[i] = NULL;
	info->score = 0;
	return info;
}

void solutionInfo_destroy(Solution * solution, SolutionInfo * info)
{
	if(info == NULL)
		return;
	free(info->productionOrder);
	free(info->readyToDeliver);
	for(unsigned int i = 0; i < solution->packCount; i++)
		free(info->deliveries[i]);
	free(info->deliveries);
	free(info);
}

SolutionInfo * solutionInfo_productionOrder(Solution * solution)
{
	SolutionInfo * info = solutionInfo_create(solution);
	unsigned int productionIndex = 0;
	task_t * sequence;
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		info->readyToDeliver[i] = (i == 0 ? 0 : info->readyToDeliver[i - 1]); // Storing information for the next production and the deliveries.
		Pack * p = solution->packList[i];
		sequence = sequencer_sequenceProductionPack(solution->instance, p->taskCount, p->deliveries, &(info->readyToDeliver[i]));
		memcpy(&(info->productionOrder[productionIndex]), sequence, p->taskCount * sizeof(task_t)); // Copying the best sequence into the info production.
		productionIndex += p->taskCount;
		free(sequence);
	}
	return info;
}

void solutionInfo_deliveryOrder(struct _Solution * solution, struct _SolutionInfo * info)
{
	info->score = 0;
	unsigned int truckReady = 0;
	unsigned int truckReadyNow = 0;
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		Pack * p = solution->packList[i];
		truckReady = MMAX(truckReady, info->readyToDeliver[i]); // Waiting for the truck to come back and for the processes to be done.
		truckReadyNow = truckReady;
		info->deliveries[i] = sequencer_sequenceDeliveriesPack(solution->instance, p->taskCount, p->deliveries, &truckReady); // Copying the best sequence into the info.
		info->score += sequencer_deliveryDelay(solution->instance, solution->packList[i]->taskCount, info->deliveries[i], &truckReadyNow);
	}
}

void solutionInfo_print(struct _Solution * solution, struct _SolutionInfo * info)
{
	if(info != NULL)
	{
		// Production
		printf("\tInfo :\n\t\tProduction :\n\t\t\t");
		for(unsigned int i = 0; i < solution->instance->taskCount; i++)
			printf(" %d", info->productionOrder[i]);
		// Deliveries
		printf("\n\t\tDeliveries :\n");
		for(unsigned int i = 0; i < solution->packCount; i++)
		{
			printf("\t\t\tBatch : (");
			for(unsigned int j = 0; j < solution->packList[i]->taskCount; j++)
				printf(" T%d", info->deliveries[i][j] + 1);
			printf(" )\n");
		}
		printf("\t\tScore : %d", info->score);
	}
	else
		printf("\tInfo : NULL\n");
}

