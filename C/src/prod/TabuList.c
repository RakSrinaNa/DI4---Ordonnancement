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

void tabuList_addItem(TabuList * list, TabuItem * item)
{

}

Bool tabuList_contains(TabuList * list, TabuItem * item)
{
	if(item == NULL || list == NULL)
		return False;
	TabuItem * current = list->next;
	while(current != NULL)
	{
		if(tabuList_isSame(item, current))
			return True;
		current = current->next;
	}
	return True;
}

Bool tabuList_isSame(TabuItem * item1, TabuItem * item2)
{
	if(item1 == NULL || item2 == NULL)
		return False;
	return (item1->destination == item2->destination && item1->source == item2->source) || (item1->destination == item2->source && item1->source == item2->destination) ? True : False;
}
