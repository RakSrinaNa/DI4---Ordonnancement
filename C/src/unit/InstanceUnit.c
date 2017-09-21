#include "headers/InstanceUnit.h"
#include "../headers/Instance.h"
#include "../headers/Utils.h"

#define UNIT_INSTANCE_TASK_COUNT 4

void instanceUnit() //Mainly test for memory leaks
{
	instance_destroy(instance_create());
	
	Instance * instance = instance_create();
	instance->taskCount = UNIT_INSTANCE_TASK_COUNT;
	MMALLOC(instance->distancesMatrix, unsigned int *, UNIT_INSTANCE_TASK_COUNT + 1, "INSTANCE_UNIT");
	for(unsigned int i = 0; i <= UNIT_INSTANCE_TASK_COUNT; i++)
	{
		MMALLOC(instance->distancesMatrix[i], unsigned int, UNIT_INSTANCE_TASK_COUNT + 1, "INSTANCE_UNIT");
	}
	instance_destroy(instance);
	
	instance = instance_create();
	instance->taskCount = UNIT_INSTANCE_TASK_COUNT;
	MMALLOC(instance->tasks, Task *, UNIT_INSTANCE_TASK_COUNT, "INSTANCE_UNIT");
	for(unsigned int i = 0; i < UNIT_INSTANCE_TASK_COUNT; i++)
		instance->tasks[i] = task_create(instance);
	instance_destroy(instance);
}
