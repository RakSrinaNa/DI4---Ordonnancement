#include "headers/SolutionUnit.h"
#include "../headers/Solution.h"
#include "../headers/Parser.h"
#include "headers/UnitUtils.h"

void solutionUnit()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance2.txt");
	
	solution_destroy(solution_create(instance));
	
	Solution * solution = solution_create(instance);
	if(solution->instance != instance)
		unit_error("Solution 1: Wrong instance");
	
	if(solution->processOrder == NULL)
		unit_error("Solution 2: Process list shouldn't be NULL");
	
	solution_setProcessIndex(solution, 3, 0);
	solution_setProcessIndex(solution, 4, 3);
	solution_setProcessIndex(solution, 2, 2);
	unsigned int correctOrder1[] = {3, 0, 2, 1, 4};
	if(!unit_uintArrayEquals(correctOrder1, solution->processOrder, 5))
		unit_error("Solution 3: Bad process index");
	
	solution_setProcessIndex(solution, 99, 2);
	if(!unit_uintArrayEquals(correctOrder1, solution->processOrder, 5))
		unit_error("Solution 4: Bad process index");
	
	solution_setProcessIndex(solution, 2, 99);
	unsigned int correctOrder2[] = {3, 0, 1, 4, 2};
	if(!unit_uintArrayEquals(correctOrder2, solution->processOrder, 5))
		unit_error("Solution 4: Bad process index");
	
	if(!solution_getProcessIndex(solution, 3) != 0 || !solution_getProcessIndex(solution, 0) != 1 || !solution_getProcessIndex(solution, 1) != 2 || !solution_getProcessIndex(solution, 4) != 3 || !solution_getProcessIndex(solution, 2) != 4)
		unit_error("Solution 5: Bad process index");
	
	if(solution->packCount != 1)
		unit_error("Solution 6: No packs in solution");
	if(solution->packList[0]->taskCount != instance->taskCount)
		unit_error("Solution 7: Not all tasks in packs");
	
	solution_moveTaskPack(solution, 0, 0);
	if(solution->packCount != 1 || solution->packList[0]->taskCount != instance->taskCount || solution_getTaskPack(solution, 0) != 0)
		unit_error("Solution 8: Bad pack move");
	
	solution_moveTaskPack(solution, 0, 99);
	if(solution->packCount != 1 || solution->packList[0]->taskCount != instance->taskCount || solution_getTaskPack(solution, 0) != 0)
		unit_error("Solution 9: Bad pack move");
	
	solution_moveTaskPack(solution, 0, 1);
	if(solution->packCount != 2 || solution->packList[0]->taskCount != instance->taskCount - 1 || solution->packList[1]->taskCount != 1 || solution_getTaskPack(solution, 0) != 1)
		unit_error("Solution 10: Bad pack move");
	
	solution_moveTaskPack(solution, 1, 1);
	if(solution->packCount != 2 || solution->packList[0]->taskCount != instance->taskCount - 2 || solution->packList[1]->taskCount != 2 || solution_getTaskPack(solution, 1) != 1)
		unit_error("Solution 11: Bad pack move");
	
	solution_moveTaskPack(solution, 2, 2);
	if(solution->packCount != 3 || solution->packList[0]->taskCount != instance->taskCount - 3 || solution->packList[2]->taskCount != 1 || solution_getTaskPack(solution, 2) != 2)
		unit_error("Solution 12: Bad pack move");
	
	solution_moveTaskPack(solution, 0, 0);
	if(solution->packCount != 3 || solution->packList[0]->taskCount != instance->taskCount - 2 || solution->packList[1]->taskCount != 1 || solution_getTaskPack(solution, 0) != 0)
		unit_error("Solution 13: Bad pack move");
	
	solution_moveTaskPack(solution, 1, 0);
	if(solution->packCount != 2 || solution->packList[0]->taskCount != instance->taskCount - 1 || solution->packList[1]->taskCount != 1 || solution_getTaskPack(solution, 1) != 0 || solution_getPackIndex(solution, 2) != 1)
		unit_error("Solution 14: Bad pack move");
	
	solution_moveTaskPack(solution, 2, 0);
	if(solution->packCount != 1 || solution->packList[0]->taskCount != instance->taskCount || solution_getPackIndex(solution, 2) != 0)
		unit_error("Solution 15: Bad pack move");
	
	instance_destroy(instance);
}
