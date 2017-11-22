#include "headers/TabuListUnit.h"
#include "../prod/headers/TabuList.h"
#include "headers/UnitUtils.h"

void tabuListUnit()
{
	TabuItem item1;
	item1.source = 1;
	item1.destination = 2;
	
	TabuItem item2;
	item2.source = 10;
	item2.destination = 11;
	
	if(tabuList_isSame(&item1, &item2))
		unit_error("TabuList 1: Tabu items should not match");
	if(tabuList_isSame(NULL, &item2))
		unit_error("TabuList 2: Tabu items should not match");
	if(tabuList_isSame(NULL, NULL))
		unit_error("TabuList 3: Tabu items should not match");
	
	item2.source = 1;
	item2.destination = 2;
	if(tabuList_isSame(&item1, &item2))
		unit_error("TabuList 4: Tabu items should match");
	
	item2.source = 2;
	item2.destination = 1;
	if(tabuList_isSame(&item1, &item2))
		unit_error("TabuList 5: Tabu items should match");
}