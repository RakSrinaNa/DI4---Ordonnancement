#include "headers/Pack.h"
#include "headers/Utils.h"

Pack * pack_create()
{
	Pack * pack = NULL;
	MMALLOC(pack, Pack, 1, "pack_create");
	pack->taskCount = 0;
	pack->processOrder = NULL;
	pack->deliveryOrder = NULL;
	return pack;
}

void pack_destroy(Pack * pack)
{
	free(pack->processOrder);
	free(pack->deliveryOrder);
	free(pack);
}
