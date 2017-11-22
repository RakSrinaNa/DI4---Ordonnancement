#include "headers/TabuList.h"

TabuList * tabuList_create(unsigned int maxSize)
{
	TabuList * list = NULL;
	MMALLOC(list, TabuList, 1, "tabuList_create");
	list->maxSize = maxSize;
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
	debugPrint("TabuList created: %p\n", list);
	return list;
}

void tabuList_destroy(TabuList * list)
{
	TabuItem * current = list->first;
	while(current != NULL) //For each element of the list.
	{
		TabuItem * next = current->next;
		free(current);
		current = next;
	}
	free(list);
	debugPrint("TabuList destroyed: %p\n", list);
}

void tabuList_addItem(TabuList * list, TabuItem * item)
{
	if(item == NULL)
	{
		warn("tabuList_addItem: Trying to add NULL item into list %p.\n", list);
		return;
	}
	if(list->size == 0)
		list->first = item;
	else
		list->last->next = item;
	list->last = item;
	list->size++;
	debugPrint("Added tabu item %p in list %p, now have a size of %d/%d\n", item, list, list->size, list->maxSize);
	
	if(list->size > list->maxSize)
	{
		TabuItem * toDel = list->first;
		list->first = toDel->next;
		free(toDel);
		list->size--;
	}
}

Bool tabuList_contains(TabuList * list, TabuItem * item)
{
	debugPrint("Searching for tabu item %p in list %p\n", item, list);
	if(item == NULL || list == NULL)
		return False;
	TabuItem * current = list->first;
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
