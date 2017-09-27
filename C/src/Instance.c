#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "headers/Instance.h"

void instance_destroy(Instance * instance)
{
	//Free the distance matrix.
	if(instance->distancesMatrix != NULL)
		for(unsigned int i = 0; i <= instance->taskCount; i++)
			free(instance->distancesMatrix[i]);
	free(instance->distancesMatrix);
	
	//Free the tasks.
	if(instance->tasks != NULL)
		for(unsigned int i = 0; i < instance->taskCount; i++)
			task_destroy(instance->tasks[i]);
	free(instance->tasks);
	
	free(instance);
}

Instance * instance_create()
{
	Instance * instance;
	MMALLOC(instance, Instance, 1, "parser_readInstanceFromFile");
	instance->machineCount = 0;
	instance->taskCount = 0;
	instance->distancesMatrix = NULL;
	instance->tasks = NULL;
	return instance;
}

void instance_print(Instance * instance)
{
	printf("Machine count: %d\nTask count: %d\nTasks:\n", instance->machineCount, instance->taskCount);
	for(unsigned int i = 0; i < instance->taskCount; i++)
	{
		task_print(instance->tasks[i]);
		printf("------------------------\n");
	}
	printf("Distances:\n");
	for(unsigned int i = 0; i <= instance->taskCount; i++)
	{
		for(unsigned int j = 0; j <= instance->taskCount; j++)
			printf("%-4d", instance_getDistance(instance, i, j));
		printf("\n");
	}
}
