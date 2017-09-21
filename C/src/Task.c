#include <stdlib.h>

#include "headers/Task.h"
#include "headers/Utils.h"

void task_destroy(Task * task)
{
	free(task->machineDurations);
	free(task);
}

Task * task_create(Instance * instance)
{
	Task * task;
	MMALLOC(task, Task, 1, "task_create");
	task->machineDurations = NULL;
	task->instance = instance;
	task->dueDate = 0;
	MMALLOC(task->machineDurations, unsigned int, instance->machineCount, "task_create");
	return task;
}

void task_print(Task * task)
{
	printf("Due date: %d\nTimes: ", task->dueDate);
	for(unsigned int i = 0; i < task->instance->machineCount; i++)
		printf("%d\t", task_getMachineDuration(task, i));
	printf("\n");
}
