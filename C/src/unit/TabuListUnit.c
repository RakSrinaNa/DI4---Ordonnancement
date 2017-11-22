#include "headers/TabuListUnit.h"
#include "../prod/headers/TabuList.h"
#include "headers/UnitUtils.h"

void tabuListUnit()
{
	TabuItem * item1 = NULL;
	MMALLOC(item1, TabuItem, 1, "tabuListUnit");
	item1->source = 1;
	item1->destination = 2;
	
	TabuItem * item2 = NULL;
	MMALLOC(item2, TabuItem, 1, "tabuListUnit");
	item2->source = 10;
	item2->destination = 11;
	
	if(tabuList_isSame(item1, item2))
		unit_error("TabuList 1: Tabu items should not match");
	if(tabuList_isSame(NULL, item2))
		unit_error("TabuList 2: Tabu items should not match");
	if(tabuList_isSame(NULL, NULL))
		unit_error("TabuList 3: Tabu items should not match");
	
	item2->source = 1;
	item2->destination = 2;
	if(tabuList_isSame(item1, item2))
		unit_error("TabuList 4: Tabu items should match");
	
	item2->source = 2;
	item2->destination = 1;
	if(tabuList_isSame(item1, item2))
		unit_error("TabuList 5: Tabu items should match");
	
	TabuList * list = tabuList_create(2);
	if(list->size != 0)
		unit_error("TabuList 6: Tabu list should have size 0");
	if(list->maxSize != 2)
		unit_error("TabuList 7: Tabu list should have maxSize 2");
	
	tabuList_addItem(list, item1);
	if(list->size != 1)
		unit_error("TabuList 8: Tabu list should have size 1");
	if(!tabuList_contains(list, item1))
		unit_error("TabuList 9: Tabu list should contain the item1");
	if(tabuList_contains(list, item2))
		unit_error("TabuList 10: Tabu list should not contain the item2");
	
	tabuList_addItem(list, item2);
	if(list->size != 2)
		unit_error("TabuList 8: Tabu list should have size 2");
	if(!tabuList_contains(list, item1))
		unit_error("TabuList 9: Tabu list should contain the item1");
	if(!tabuList_contains(list, item2))
		unit_error("TabuList 10: Tabu list should contain the item2");
	
	TabuItem * item3 = NULL;
	MMALLOC(item3, TabuItem, 1, "tabuListUnit");
	item3->source = 90;
	item3->destination = 11;
	tabuList_addItem(list, item3);
	if(list->size != 2)
		unit_error("TabuList 11: Tabu list should have size 2");
	if(!tabuList_contains(list, item1))
		unit_error("TabuList 12: Tabu list should not contain the item1");
	if(tabuList_contains(list, item2))
		unit_error("TabuList 13: Tabu list should not contain the item2");
	if(tabuList_contains(list, item2))
		unit_error("TabuList 14: Tabu list should not contain the item3");
	
	tabuList_destroy(list);
}