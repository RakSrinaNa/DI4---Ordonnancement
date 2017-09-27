#include "headers/TaskUnit.h"
#include "headers/UnitUtils.h"
#include "../headers/Task.h"

void taskUnit() //Mainly check for memory leaks.
{
	Instance * instance = instance_create();
	task_destroy(task_create(instance));
	instance->machineCount = 10;
	Task * task = task_create(instance);
	if(task->instance != instance)
		unit_error("Task 0: Wrong instance");
	for(unsigned int i = 0; i < 100; i++)
	{
		unsigned int rnd = (unsigned int) rand();
		task_setDueDate(task, rnd);
		if(task_getDueDate(task) != rnd)
			unit_error("Task 1: Wrong due date");
	}
	for(unsigned int i = 0; i < 100; i++)
	{
		unsigned int rnd = (unsigned int) rand();
		task_setMachineDuration(task, i % instance->machineCount, rnd);
		if(task_getMachineDuration(task, i % instance->machineCount) != rnd)
			unit_error("Task 2: Wrong machine duration");
	}
	task_destroy(task);
}
