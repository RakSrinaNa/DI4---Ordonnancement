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
	
	if(solution->info != NULL)
		unit_error("Solution 2: Process info should be NULL");
	
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
	
	Solution * solutionCopy = solution_copy(solution);
	if(solutionCopy->instance != solution->instance)
		unit_error("Solution 12.50: Bad copy instance");
	if(solutionCopy->packCount != solution->packCount)
		unit_error("Solution 12.51: Bad copy pack count");
	if(solution->info != NULL)
		unit_error("Solution 12.52: Bad copy info");
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		if(solution->packList[i]->taskCount != solutionCopy->packList[i]->taskCount)
			unit_error("Solution 12.53: Bad copy delivery count");
		
		if(!unit_uintArrayEquals(solution->packList[i]->deliveries, solutionCopy->packList[i]->deliveries, solution->packList[i]->taskCount))
			unit_error("Solution 12.54: Bad copy delivery order");
	}
	solution_destroy(solutionCopy);
	
	solution_moveTaskPack(solution, 0, 0);
	if(solution->packCount != 3 || solution->packList[0]->taskCount != instance->taskCount - 2 || solution->packList[1]->taskCount != 1 || solution_getTaskPack(solution, 0) != 0)
		unit_error("Solution 13: Bad pack move");
	
	solution_moveTaskPack(solution, 1, 0);
	if(solution->packCount != 2 || solution->packList[0]->taskCount != instance->taskCount - 1 || solution->packList[1]->taskCount != 1 || solution_getTaskPack(solution, 1) != 0 || solution_getTaskPack(solution, 2) != 1)
		unit_error("Solution 14: Bad pack move");
	
	solution_moveTaskPack(solution, 2, 0);
	if(solution->packCount != 1 || solution->packList[0]->taskCount != instance->taskCount || solution_getTaskPack(solution, 2) != 0)
		unit_error("Solution 15: Bad pack move");
	
	solution_destroy(solution);
	instance_destroy(instance);
}
