#include "ParserUnit.h"
#include "../headers/Instance.h"
#include "../headers/Parser.h"
#include "headers/UnitUtils.h"

void parserUnit()
{
	Instance * instance1 = parser_readInstanceFromFile("./unitResources/Instance1.txt");
	if(instance1->machineCount != 1 || instance1->taskCount != 2)
		unit_error("Parser 1: Wrong machine or task count.");
	if(task_getDueDate(instance1->tasks[0]) != 7 || task_getDueDate(instance1->tasks[1]) != 8)
		unit_error("Parser 2: Wrong due date.");
	if(task_getMachineDuration(instance1->tasks[0], 0) != 3 || task_getMachineDuration(instance1->tasks[1], 0) != 5)
		unit_error("Parser 3: Wrong machine duration.");
	unsigned int correctMatrix1[3][3] = {{0,5,6},{5,0,9},{6,9,0}};
	for(unsigned int i = 0; i < 3; i++)
		if(!unit_uintArrayEquals(correctMatrix1[i], instance1->distancesMatrix[i], 3))
			unit_error("Parser 4: Wrong distance matrix.");
	instance_destroy(instance1);
	
	
	Instance * instance2 = parser_readInstanceFromFile("./unitResources/Instance2.txt");
	if(instance2->machineCount != 3 || instance2->taskCount != 5)
		unit_error("Parser 5: Wrong machine or task count.");
	if(task_getDueDate(instance2->tasks[0]) != 10 || task_getDueDate(instance2->tasks[1]) != 20 || task_getDueDate(instance2->tasks[2]) != 30 || task_getDueDate(instance2->tasks[3]) != 40 || task_getDueDate(instance2->tasks[4]) != 50)
		unit_error("Parser 6: Wrong due date.");
	if(task_getMachineDuration(instance2->tasks[0], 0) != 1 || task_getMachineDuration(instance2->tasks[1], 0) != 2 || task_getMachineDuration(instance2->tasks[2], 0) != 3 || task_getMachineDuration(instance2->tasks[3], 0) != 4 || task_getMachineDuration(instance2->tasks[4], 0) != 5)
		unit_error("Parser 7.1: Wrong machine duration.");
	if(task_getMachineDuration(instance2->tasks[0], 1) != 6 || task_getMachineDuration(instance2->tasks[1], 1) != 7 || task_getMachineDuration(instance2->tasks[2], 1) != 8 || task_getMachineDuration(instance2->tasks[3], 1) != 9 || task_getMachineDuration(instance2->tasks[4], 1) != 10)
		unit_error("Parser 7.2: Wrong machine duration.");
	if(task_getMachineDuration(instance2->tasks[0], 2) != 11 || task_getMachineDuration(instance2->tasks[1], 2) != 12 || task_getMachineDuration(instance2->tasks[2], 2) != 13 || task_getMachineDuration(instance2->tasks[3], 2) != 14 || task_getMachineDuration(instance2->tasks[4], 2) != 15)
		unit_error("Parser 7.3: Wrong machine duration.");
	unsigned int correctMatrix2[6][6] = {{0,1,2,3,4,5},{1,0,2,3,4,5},{2,2,0,3,4,5},{3,3,3,0,4,5},{4,4,4,4,0,5},{5,5,5,5,5,0}};
	for(unsigned int i = 0; i < 6; i++)
		if(!unit_uintArrayEquals(correctMatrix2[i], instance2->distancesMatrix[i], 3))
			unit_error("Parser 8: Wrong distance matrix.");
	instance_destroy(instance2);
}
