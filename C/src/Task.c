#include <stdlib.h>

#include "headers/Task.h"

Task * task_create(Instance * instance)
{
	Task * task;
	MMALLOC(task, Task, 1, "task_create");
	task->machineDurations = NULL;
	task->instance = instance;
	task->dueDate = 0;
	MMALLOC(task->machineDurations, unsigned int, instance->machineCount, "task_create");
	debugPrint("Task created : %p\n", task);
	return task;
}

void task_destroy(Task * task)
{
	if(task == NULL)
		return;
	free(task->machineDurations);
	free(task);
	debugPrint("Task destroyed : %p\n", task);
}

void task_setMachineDuration(Task * task, unsigned int machineID, unsigned int duration)
{
	if(machineID >= task->instance->machineCount)
		warn("task_setMachineDuration : Machine %d doesn't exist.\n", machineID);
	else
		task->machineDurations[machineID] = duration;
}

unsigned int task_getMachineDuration(Task * task, unsigned int machineID)
{
	if(machineID > task->instance->machineCount)
		fatalError("task_getMachineDuration: Machine %d doesn't exist.\n", machineID);
	return task->machineDurations[machineID];
}

void task_print(Task * task)
{
	if(task != NULL)
	{
		printf("Due date: %d\nTimes: ", task->dueDate);
		for(unsigned int i = 0; i < task->instance->machineCount; i++)
			printf("%-6d", task_getMachineDuration(task, i));
		printf("\n");
	}
}
