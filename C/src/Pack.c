#include "headers/Pack.h"

Pack * pack_create(Instance * instance)
{
	Pack * pack = NULL;
	MMALLOC(pack, Pack, 1, "pack_create");
	pack->instance = instance;
	pack->taskCount = 0;
	pack->deliveries = NULL;
	return pack;
}

void pack_destroy(Pack * pack)
{
	if(pack == NULL)
		return;
	
	free(pack->deliveries);
	free(pack);
}

Bool pack_hasTask(Pack * pack, task_t task)
{
	for(task_t i = 0; i < pack->taskCount; i++)
		if(pack->deliveries[i] == task)
			return True;
	return False;
}

int pack_getTaskIndex(Pack * pack, task_t task)
{
	for(task_t i = 0; i < pack->taskCount; i++)
		if(pack->deliveries[i] == task)
			return (int) i;
	return -1; // If the task was not found.
}

void pack_addTask(Pack * pack, task_t task)
{
	if(task < pack->instance->taskCount) //Check task ID range.
	{
		Bool found = False;
		for(task_t i = 0; i < pack->taskCount; i++)
			if(pack->deliveries[i] == task)
			{
				found = True;
				break;
			}
		if(!found) //Not found so we add at the end.
		{
			pack->taskCount++;
			RREALLOC(pack->deliveries, task_t, pack->taskCount, "pack_addTask");
			pack->deliveries[pack->taskCount - 1] = task;
		}
		else
			warn("pack_addTask : Task %d is already in the pack.\n", task);
	}
}

Bool pack_removeTask(Pack * pack, task_t task)
{
	Bool found = False;
	for(task_t i = 0; i < pack->taskCount; i++) // Looking for the task
	{
		// If we already found the task, shifting to the left.
		if(found)
			pack->deliveries[i - 1] = pack->deliveries[i];
		if(pack->deliveries[i] == task)
			found = True;
	}
	if(found)
	{
		pack->taskCount--;
		RREALLOC(pack->deliveries, task_t, pack->taskCount, "pack_removeTask");
	}
	else
		warn("pack_removeTask : Task %d is not in the pack.\n", task);
	return (pack->taskCount > 0 ? False : True); // If pack empty, return True so it can be destroyed.
}

void pack_switchDelivery(Pack * pack, task_t delivery1, task_t delivery2)
{
	debugPrint("Switching deliveries %d and %d.\n", delivery1, delivery2);
	if(delivery1 == delivery2)
		return;
	if(pack_hasTask(pack, delivery1) && pack_hasTask(pack, delivery2))
	{
		task_t pos1 = (task_t) pack_getTaskIndex(pack, delivery1);
		task_t pos2 = (task_t) pack_getTaskIndex(pack, delivery2);
		pack->deliveries[pos1] = delivery2;
		pack->deliveries[pos2] = delivery1;
	}
	else
		warn("pack_switchDelivery : Missing task (%d / %d).\n", delivery1, delivery2);
}

void pack_moveDelivery(Pack * pack, task_t delivery, unsigned int position)
{
	debugPrint("Moving delivery %d to position %d.\n", delivery, position);
	if(pack_hasTask(pack, delivery))
	{
		task_t index = (task_t) pack_getTaskIndex(pack, delivery);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < pack->taskCount - direction; i += direction) // Shifting to the correct direction
			pack->deliveries[i] = pack->deliveries[i + direction];
		pack->deliveries[MMIN(position, pack->taskCount - 1)] = delivery;
	}
	else
		warn("pack_moveDelivery : Missing task %d.\n", delivery);
}

void pack_print(Pack * pack)
{
	if(pack != NULL)
	{
		printf("Tasks : %d ( ", pack->taskCount);
		for(task_t i = 0; i < pack->taskCount; i++)
			printf("%d(%d) ", pack->deliveries[i], instance_getDueDate(pack->instance, pack->deliveries[i]));
		printf(")");
	}
	else
		printf("PACK NULL");
}
