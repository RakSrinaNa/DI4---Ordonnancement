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
	
	//TODO: Test to parse more instances
}
