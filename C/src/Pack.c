#include "headers/Pack.h"
#include "headers/Utils.h"

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
	free(pack->deliveryOrder);
	free(pack);
}

Bool pack_hasTask(Pack * pack, unsigned int task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
	{
		if(pack->deliveryOrder[i] == task)
			return True;
	}
	return False;
}

int pack_getTaskIndex(Pack * pack, unsigned int task)
{
	for(unsigned int i = 0; i < pack->taskCount; i++)
	{
		if(pack->deliveryOrder[i] == task)
			return (int) i;
	}
	return -1;
}

void pack_addTask(Pack * pack, unsigned int task)
{
	if(task < pack->instance->taskCount)
	{
		Bool found = False;
		for(unsigned int i = 0; i < pack->taskCount; i++)
		{
			if(pack->deliveryOrder[i] == task)
			{
				found = True;
				break;
			}
		}
		if(!found)
		{
			pack->taskCount++;
			RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_addTask");
			pack->deliveryOrder[pack->taskCount - 1] = task;
		}
		else
			warn("WARNING : pack_addTask : provided task is already in the list (%d)", task);
	}
}

Bool pack_removeTask(Pack * pack, unsigned int task)
{
	Bool found = False;
	for(unsigned int i = 0; i < pack->taskCount; i++)
	{
		if(found)
		{
			pack->deliveryOrder[i-1] = pack->deliveryOrder[i];
		}
		if(pack->deliveryOrder[i] == task)
			found = True;
	}
	if(found)
	{
		pack->taskCount--;
		RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_removeTask");
	}
	else
		warn("WARNING : pack_removeTask : provided task is not in the list (%d)", task);
	return (pack->taskCount > 0 ? False : True);
}

void pack_switchDelivery(Pack * pack, unsigned int delivery1, unsigned int delivery2)
{
	if(delivery1 == delivery2) return;
	if(pack_hasTask(pack, delivery1) && pack_hasTask(pack, delivery2))
	{
		unsigned int pos1 = (unsigned int) pack_getTaskIndex(pack, delivery1);
		unsigned int pos2 = (unsigned int) pack_getTaskIndex(pack, delivery2);
		pack->deliveryOrder[pos1] = delivery2;
		pack->deliveryOrder[pos2] = delivery1;
	}
	else
		warn("WARNING : pack_switchDelivery : one of the provided tasks does not exist (has %d and %d)\n", delivery1, delivery2);
}

void pack_moveDelivery(Pack * pack, unsigned int delivery, unsigned int position)
{
	if(pack_hasTask(pack, delivery))
	{
		unsigned int index = (unsigned int) pack_getTaskIndex(pack, delivery);
		int direction = (position > index ? 1 : -1); // Direction of the iteration
		for(unsigned int i = index; i != position && i < pack->taskCount; i += direction)
		{
			pack->deliveryOrder[i] = pack->deliveryOrder[i + direction];
		}
		pack->deliveryOrder[MMIN(position, pack->taskCount - 1)] = delivery;
	}
	else
		warn("WARNING : pack_moveDelivery : given position is out of range (%d)\n", position);
}
