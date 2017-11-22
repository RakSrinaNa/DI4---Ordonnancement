#include "headers/TabuList.h"
#include "headers/Utils.h"

TabuList * tabuList_create(unsigned int maxSize)
{
	TabuList * list = NULL;
	MMALLOC(list, TabuList, 1, "tabuList_create");
	list->maxSize = maxSize;
	list->size = 0;
	list->next = NULL;
	list->last = NULL;
	return list;
}

void tabuList_destroy(TabuList * list)
{
	TabuItem * current = list->next;
	while(current != NULL)
	{
		TabuItem * next = current->next;
		free(current);
		current = next;
	}
	free(list);
}

void tabuList_addItem(TabuItem * item)
{

}

Bool tabuList_contains(TabuItem * item)
{
	return True;
}
