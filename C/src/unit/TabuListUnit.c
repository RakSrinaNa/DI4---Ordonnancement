#include "headers/TabuListUnit.h"
#include "../prod/headers/TabuList.h"
#include "headers/UnitUtils.h"
#include "../prod/FLAGS.h"

void tabuListUnit()
{
	TabuItem * item1 = tabuItem_create(1, 2, SWAP);
	TabuItem * item2 = tabuItem_create(10, 11, SWAP);
	TabuItem * item4 = tabuItem_create(1, 2, EBSR);
	
	if(tabuItem_isSame(item1, item2))
		unit_error("TabuList 1: Tabu items should not match");
	if(tabuItem_isSame(NULL, item2))
		unit_error("TabuList 2: Tabu items should not match");
	if(tabuItem_isSame(NULL, NULL))
		unit_error("TabuList 3: Tabu items should not match");
	if(tabuItem_isSame(item1, item4))
		unit_error("TabuList 3.5: Tabu items should not match");
	tabuItem_destroy(item4);
	
	item2->source = 1;
	item2->destination = 2;
	if(!tabuItem_isSame(item1, item2))
		unit_error("TabuList 4: Tabu items should match");
	
	item2->source = 2;
	item2->destination = 1;
	if(TABU_LOGIC && !tabuItem_isSame(item1, item2))
		unit_error("TabuList 5: Tabu items should match");
	
	item2->destination = 99;
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
		unit_error("TabuList 10: Tabu list should have size 2");
	if(!tabuList_contains(list, item1))
		unit_error("TabuList 11: Tabu list should contain the item1");
	if(!tabuList_contains(list, item2))
		unit_error("TabuList 12: Tabu list should contain the item2");
	
	TabuItem * item3 = tabuItem_create(99, 11, SWAP);
	tabuList_addItem(list, item3);
	if(list->size != 2)
		unit_error("TabuList 13: Tabu list should have size 2");
	if(!tabuList_contains(list, item2))
		unit_error("TabuList 14: Tabu list should contain the item2");
	if(!tabuList_contains(list, item3))
		unit_error("TabuList 15: Tabu list should contain the item3");
	
	tabuList_clear(list);
	if(list->size != 0)
		unit_error("TabuList 16: Tabu list should be empty");
	
	tabuList_destroy(list);
}
