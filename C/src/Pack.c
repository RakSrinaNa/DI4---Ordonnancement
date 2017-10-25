#include "headers/Pack.h"

Pack * pack_create(Instance * instance)
{
	Pack * pack = NULL;
	MMALLOC(pack, Pack, 1, "pack_create");
	pack->instance = instance;
	pack->taskCount = 0;
	pack->deliveryOrder = NULL;
	return pack;
}

void pack_destroy(Pack * pack)
{
	if(pack == NULL)
		return;
	
	free(pack->deliveryOrder);
	free(pack);
}

Bool pack_hasTask(Pack * pack, unsigned int task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
		if(pack->deliveryOrder[i] == task)
			return True;
	return False;
}

int pack_getTaskIndex(Pack * pack, unsigned int task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
		if(pack->deliveryOrder[i] == task)
			return (int) i;
	return -1; // If the task was not found.
}

void pack_addTask(Pack * pack, unsigned int task)
{
	if(task < pack->instance->taskCount) //Check task ID range.
	{
		Bool found = False;
		for(unsigned int i = 0; i < pack->taskCount; i++)
			if(pack->deliveryOrder[i] == task)
			{
				found = True;
				break;
			}
		if(!found) //Not found so we add at the end.
		{
			pack->taskCount++;
			RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_addTask");
			pack->deliveryOrder[pack->taskCount - 1] = task;
		}
		else
			warn("pack_addTask : Task %d is already in the pack.\n", task);
	}
}

Bool pack_removeTask(Pack * pack, unsigned int task)
{
	Bool found = False;
	for(unsigned int i = 0; i < pack->taskCount; i++) // Looking for the task
	{
		// If we already found the task, shifting to the left.
		if(found)
			pack->deliveryOrder[i - 1] = pack->deliveryOrder[i];
		if(pack->deliveryOrder[i] == task)
			found = True;
	}
	if(found)
	{
		pack->taskCount--;
		RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_removeTask");
	}
	else
		warn("pack_removeTask : Task %d is not in the pack.\n", task);
	return (pack->taskCount > 0 ? False : True); // If pack empty, return True so it can be destroyed.
}

void pack_switchDelivery(Pack * pack, unsigned int delivery1, unsigned int delivery2)
{
	debugPrint("Switching deliveries %d and %d.\n", delivery1, delivery2);
	if(delivery1 == delivery2)
		return;
	if(pack_hasTask(pack, delivery1) && pack_hasTask(pack, delivery2))
	{
		unsigned int pos1 = (unsigned int) pack_getTaskIndex(pack, delivery1);
		unsigned int pos2 = (unsigned int) pack_getTaskIndex(pack, delivery2);
		pack->deliveryOrder[pos1] = delivery2;
		pack->deliveryOrder[pos2] = delivery1;
	}
	else
		warn("pack_switchDelivery : Missing task (%d / %d).\n", delivery1, delivery2);
}

void pack_moveDelivery(Pack * pack, unsigned int delivery, unsigned int position)
{
	debugPrint("Moving delivery %d to position %d.\n", delivery, position);
	if(pack_hasTask(pack, delivery))
	{
		unsigned int index = (unsigned int) pack_getTaskIndex(pack, delivery);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < pack->taskCount - direction; i += direction) // Shifting to the correct direction
			pack->deliveryOrder[i] = pack->deliveryOrder[i + direction];
		pack->deliveryOrder[MMIN(position, pack->taskCount - 1)] = delivery;
	}
	else
		warn("pack_moveDelivery : Missing task %d.\n", delivery);
}

void pack_print(Pack * pack)
{
	if(pack != NULL)
	{
		printf("Tasks : %d ( ", pack->taskCount);
		for(unsigned int i = 0; i < pack->taskCount; i++)
			printf("%d(%d) ", pack->deliveryOrder[i], instance_getDueDate(pack->instance, pack->deliveryOrder[i]));
		printf(")");
	}
	else
		printf("PACK NULL");
}
