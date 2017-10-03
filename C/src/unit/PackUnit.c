#include "headers/PackUnit.h"
#include "../headers/Pack.h"
#include "headers/UnitUtils.h"
#include "../headers/Parser.h"

void packUnit()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance2.txt");
	
	pack_destroy(pack_create(instance));
	
	Pack * pack = pack_create(instance);
	if(pack->instance != instance)
		unit_error("Pack 0: Wrong instance");
	
	if(pack->taskCount != 0)
		unit_error("Pack 1: Created pack is not empty");
	
	if(pack_hasTask(pack, 0) != False || pack_hasTask(pack, 1) != False || pack_hasTask(pack, 2) != False || pack_hasTask(pack, 3) != False || pack_hasTask(pack, 4) != False)
		unit_error("Pack 2: Empty pack contains tasks");
	
	pack_addTask(pack, 0);
	if(pack_hasTask(pack, 0) != True)
		unit_error("Pack 3: Added task is not present");
	
	unsigned int correctOrder1[] = {0};
	if(!unit_uintArrayEquals(correctOrder1, pack->deliveryOrder, 1))
		unit_error("Pack 4: Wrong array order");
	
	pack_addTask(pack, 2);
	pack_addTask(pack, 1);
	if(pack_hasTask(pack, 2) != True || pack_hasTask(pack, 1) != True)
		unit_error("Pack 5: Added task is not present");
	
	unsigned int correctOrder2[] = {0, 2, 1};
	if(!unit_uintArrayEquals(correctOrder2, pack->deliveryOrder, 3))
		unit_error("Pack 6: Wrong array order");
	
	pack_addTask(pack, 9999999);
	if(pack->taskCount > 3)
		unit_error("Pack 7: Non-existent task added");
	
	pack_switchDelivery(pack, 0, 1);
	unsigned int correctOrder3[] = {1, 2, 0};
	if(!unit_uintArrayEquals(correctOrder3, pack->deliveryOrder, 3))
		unit_error("Pack 8: Bad switch");
	
	pack_switchDelivery(pack, 99, 2);
	if(!unit_uintArrayEquals(correctOrder3, pack->deliveryOrder, 3))
		unit_error("Pack 9: Bad switch");
	
	pack_moveDelivery(pack, 1, 1);
	unsigned int correctOrder4[] = {2, 1, 0};
	if(!unit_uintArrayEquals(correctOrder4, pack->deliveryOrder, 3))
		unit_error("Pack 10: Bad move forward");
	
	pack_moveDelivery(pack, 0, 0);
	unsigned int correctOrder5[] = {0, 2, 1};
	if(!unit_uintArrayEquals(correctOrder5, pack->deliveryOrder, 3))
		unit_error("Pack 11: Bad move backward");
	
	if(pack_getTaskIndex(pack, 0) != 0 || pack_getTaskIndex(pack, 2) != 1 || pack_getTaskIndex(pack, 1) != 2)
		unit_error("Pack: 11.5: Bad index");
	
	pack_moveDelivery(pack, 99, 0);
	if(!unit_uintArrayEquals(correctOrder5, pack->deliveryOrder, 3))
		unit_error("Pack 12: Bad move non-existing task");
	
	pack_moveDelivery(pack, 0, 99);
	unsigned int correctOrder6[] = {2, 1, 0};
	if(!unit_uintArrayEquals(correctOrder6, pack->deliveryOrder, 3))
		unit_error("Pack 13: Bad move forward");
	
	pack_removeTask(pack, 1);
	unsigned int correctOrder7[] = {2, 0};
	if(!unit_uintArrayEquals(correctOrder7, pack->deliveryOrder, 2))
		unit_error("Pack 14: Bad delete");
	
	pack_removeTask(pack, 2);
	unsigned int correctOrder8[] = {0};
	if(!unit_uintArrayEquals(correctOrder8, pack->deliveryOrder, 1))
		unit_error("Pack 15: Bad delete");
	
	pack_destroy(pack);
	instance_destroy(instance);
}
