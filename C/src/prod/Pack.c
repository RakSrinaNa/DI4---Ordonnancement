#include "headers/Pack.h"

Pack * pack_create(Instance * instance)
{
	Pack * pack = NULL;
	MMALLOC(pack, Pack, 1, "pack_create");
	pack->instance = instance;
	pack->taskCount = 0;
	pack->tasks = NULL;
	debugPrint("Pack created : %p\n", pack);
	return pack;
}

void pack_destroy(Pack * pack)
{
	if(pack == NULL)
		return;
	
	free(pack->tasks);
	free(pack);
	debugPrint("Pack destroyed : %p\n", pack);
}

Bool pack_hasTask(Pack * pack, task_t task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
		if(pack->tasks[i] == task)
			return True;
	return False;
}

int pack_getTaskIndex(Pack * pack, task_t task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
		if(pack->tasks[i] == task)
			return (int) i;
	return -1; // If the task was not found.
}

void pack_addTask(Pack * pack, task_t task)
{
	debugPrint("Adding task %d in pack %p\n", task, pack);
	if(task < pack->instance->taskCount) //Check task ID range.
	{
		Bool found = False;
		for(unsigned int i = 0; i < pack->taskCount; i++)
			if(pack->tasks[i] == task)
			{
				found = True;
				break;
			}
		if(!found) //Not found so we add at the end.
		{
			pack->taskCount++;
			RREALLOC(pack->tasks, task_t, pack->taskCount, "pack_addTask");
			pack->tasks[pack->taskCount - 1] = task;
		}
		else
			warn("pack_addTask : Task %d is already in the pack.\n", task);
	}
}

Bool pack_removeTask(Pack * pack, task_t task)
{
	debugPrint("Removing task %d from pack %p\n", task, pack);
	Bool found = False;
	for(unsigned int i = 0; i < pack->taskCount; i++) // Looking for the task
	{
		// If we already found the task, shifting to the left.
		if(found)
			pack->tasks[i - 1] = pack->tasks[i];
		if(pack->tasks[i] == task)
			found = True;
	}
	if(found)
	{
		pack->taskCount--;
		RREALLOC(pack->tasks, task_t, pack->taskCount, "pack_removeTask");
	}
	else
		warn("pack_removeTask : Task %d is not in the pack.\n", task);
	return (pack->taskCount > 0 ? False : True); // If pack empty, return True so it can be destroyed.
}

void pack_switchDelivery(Pack * pack, task_t delivery1, task_t delivery2)
{
	debugPrint("Switching tasks %d and %d in pack %p\n", delivery1, delivery2, pack);
	if(delivery1 == delivery2)
		return;
	if(pack_hasTask(pack, delivery1) && pack_hasTask(pack, delivery2))
	{
		task_t pos1 = (task_t) pack_getTaskIndex(pack, delivery1);
		task_t pos2 = (task_t) pack_getTaskIndex(pack, delivery2);
		pack->tasks[pos1] = delivery2;
		pack->tasks[pos2] = delivery1;
	}
	else
		warn("pack_switchDelivery : Missing task (%d / %d).\n", delivery1, delivery2);
}

void pack_moveDelivery(Pack * pack, task_t delivery, unsigned int position)
{
	debugPrint("Moving delivery %d to position %d in pack %p\n", delivery, position, pack);
	if(pack_hasTask(pack, delivery))
	{
		task_t index = (task_t) pack_getTaskIndex(pack, delivery);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < pack->taskCount - direction; i += direction) // Shifting to the correct direction
			pack->tasks[i] = pack->tasks[i + direction];
		pack->tasks[MMIN(position, pack->taskCount - 1)] = delivery;
	}
	else
		warn("pack_moveDelivery : Missing task %d.\n", delivery);
}

void pack_print(Pack * pack, unsigned int packID)
{
	if(pack != NULL)
	{
		printf("Pack #%d : %d elements (", packID, pack->taskCount);
		for(task_t i = 0; i < pack->taskCount; i++)
			printf(" T%d", pack->tasks[i] + 1);
		printf(" )");
	}
	else
		printf("Pack : NULL");
}
