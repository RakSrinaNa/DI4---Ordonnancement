#include "headers/Pack.h"
#include "headers/Utils.h"

Pack * pack_create()
{
	Pack * pack = NULL;
	MMALLOC(pack, Pack, 1, "pack_create");
	pack->taskCount = 0;
	pack->deliveryOrder = NULL;
	return pack;
}

void pack_destroy(Pack * pack)
{
	free(pack->deliveryOrder);
	free(pack);
}

void pack_addTask(Pack *pack, unsigned int task)
{
	unsigned int i;
	unsigned int found = 0;
	for(i = 0; i < pack->taskCount; i++)
	{
		if(pack->deliveryOrder[i] == task)
		{
			found = 1;
			break;
		}
	}
	if(!found)
	{
		pack->taskCount++;
		RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_addTask");
		pack->deliveryOrder[pack->taskCount-1] = task;
	}
}

void pack_switchDelivery(Pack *pack, unsigned int delivery1, unsigned int delivery2)
{
	
}

void pack_moveDelivery(Pack *pack, unsigned int delivery, unsigned int position)
{
	
}

void pack_removeTask(Pack *pack, unsigned int task)
{
	unsigned int i;
	unsigned int found = 0;
	for(i = 0; i < pack->taskCount; i++)
	{
		if(pack->deliveryOrder[i] == task)
			found = 1;
		if(found)
		{
			pack->deliveryOrder[i] = pack->deliveryOrder[i];
		}
	}
	if(found)
	{
		pack->taskCount--;
		RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_removeTask");
	}
}
