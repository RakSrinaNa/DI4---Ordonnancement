#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "headers/Instance.h"

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

void instance_destroy(Instance * instance)
{
	if(instance == NULL)
		return;
	
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

unsigned int instance_getDueDate(Instance * instance, unsigned int task)
{
	if(task > instance->taskCount)
		fatal_error("instance_getDueDate : task out of range(%d)", task);
	return instance->tasks[task]->dueDate;
}

void instance_print(Instance * instance)
{
	printf("\nINSTANCE\nMachine count: %d\nTask count: %d\nTasks:\n", instance->machineCount, instance->taskCount);
	for(unsigned int i = 0; i < instance->machineCount; i++)
	{
		for(unsigned int j = 0; j < instance->taskCount; j++)
			printf("%-4d", task_getMachineDuration(instance->tasks[j], i));
		printf("\n");
	}
	printf("Due:\n");
	for(unsigned int i = 0; i < instance->taskCount; i++)
		printf("%-4d", task_getDueDate(instance->tasks[i]));
	printf("\nDistances:\n");
	for(unsigned int i = 0; i <= instance->taskCount; i++)
	{
		for(unsigned int j = 0; j <= instance->taskCount; j++)
			printf("%-4d", instance_getDistance(instance, i, j));
		printf("\n");
	}
}
