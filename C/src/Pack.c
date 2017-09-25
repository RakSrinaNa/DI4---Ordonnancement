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

int pack_addTask(Pack *pack, unsigned int task)
{
	unsigned int i;
	for(i = 0; i < taskCount; i++)
	{
		if(pack->deliveryOrder[i] == task)
			return -1;
	}
	pack->taskCount++;
	RREALLOC(pack->deliveryOrder, unsigned int, pack->taskCount, "pack_addTask");
	pack->deliveryOrder[pack->taskCount-1] = task;
	return 0;
}

int pack_switchDelivery(Pack *pack, unsigned int delivery1, unsigned int delivery2)
{
	return 0;
}

int pack_removeTask(Pack *pack, unsigned int task)
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
	if(!found)
		return -1;
	pack->taskCount--;
	RREALLOC(pack->deliveryOrder, unsigned int, taskCount, "pack_removeTask");
	return 0;
}
