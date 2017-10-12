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
	
	if(solution->productionOrder == NULL)
		unit_error("Solution 2: Process list shouldn't be NULL");
	
	solution_setProductionIndex(solution, 3, 0);
	solution_setProductionIndex(solution, 4, 3);
	solution_setProductionIndex(solution, 2, 2);
	unsigned int correctOrder1[] = {3, 0, 2, 1, 4};
	if(!unit_uintArrayEquals(correctOrder1, solution->productionOrder, 5))
		unit_error("Solution 3: Bad process index");
	
	solution_setProductionIndex(solution, 99, 2);
	if(!unit_uintArrayEquals(correctOrder1, solution->productionOrder, 5))
		unit_error("Solution 4: Bad process index");
	
	solution_setProductionIndex(solution, 2, 99);
	unsigned int correctOrder2[] = {3, 0, 1, 4, 2};
	if(!unit_uintArrayEquals(correctOrder2, solution->productionOrder, 5))
		unit_error("Solution 4.1: Bad process index");
	
	if(solution_getProductionIndex(solution, 3) != 0 || solution_getProductionIndex(solution, 0) != 1 || solution_getProductionIndex(solution, 1) != 2 || solution_getProductionIndex(solution, 4) != 3 || solution_getProductionIndex(solution, 2) != 4)
		unit_error("Solution 5: Bad process index");
	
	solution_sortByDD(solution);
	unsigned int correctOrder3[] = {2, 3, 1, 4, 0};
	if(!unit_uintArrayEquals(correctOrder3, solution->productionOrder, 5))
		unit_error("Solution 5.5: Bad sort by due date");
	
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
	if(!unit_uintArrayEquals(solution->productionOrder, solutionCopy->productionOrder, solution->instance->taskCount))
		unit_error("Solution 12.52: Bad copy process order");
	for(unsigned int i = 0; i < solution->packCount; i++)
	{
		if(solution->packList[i]->taskCount != solutionCopy->packList[i]->taskCount)
			unit_error("Solution 12.53: Bad copy delivery count");
		
		if(!unit_uintArrayEquals(solution->packList[i]->deliveryOrder, solutionCopy->packList[i]->deliveryOrder, solution->packList[i]->taskCount))
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
	
	solutionUnit_processes();
	solutionUnit_delivery();
}

void solutionUnit_processes()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance3.txt");
	unsigned int tasks[] = {0, 1, 2};
	unsigned int time = solution_productionFinalTime(instance, 3, tasks);
	if(time != 31)
		unit_error("Solution 16: Bad final time, was %d expected %d", time, 31);
	
	unsigned int tasks2[] = {1, 0, 2};
	time = solution_productionFinalTime(instance, 3, tasks2);
	if(time != 35)
		unit_error("Solution 17: Bad final time, was %d expected %d", time, 35);
	
	unsigned int tasks3[] = {2, 1, 0};
	time = solution_productionFinalTime(instance, 3, tasks3);
	if(time != 51)
		unit_error("Solution 18: Bad final time, was %d expected %d", time, 51);
	
	unsigned int tasks4[] = {0, 2, 1};
	time = solution_productionFinalTime(instance, 3, tasks4);
	if(time != 42)
		unit_error("Solution 19: Bad final time, was %d expected %d", time, 42);
	
	unsigned int tasks5[] = {1, 2, 0};
	time = solution_productionFinalTime(instance, 3, tasks5);
	if(time != 37)
		unit_error("Solution 20: Bad final time, was %d expected %d", time, 37);
	
	unsigned int tasks6[] = {2, 0, 1};
	time = solution_productionFinalTime(instance, 3, tasks6);
	if(time != 46)
		unit_error("Solution 21: Bad final time, was %d expected %d", time, 46);
	
	unsigned int cSol[] = {0, 1, 2};
	unsigned int * sol = solution_sequenceProduction(instance, 3, tasks);
	if(!unit_uintArrayEquals(cSol, sol, 3))
		unit_error("Solution 22: Bad sequence process");
	free(sol);
	
	unsigned int tasks7[] = {0, 1};
	unsigned int cSol2[] = {0, 1};
	sol = solution_sequenceProduction(instance, 2, tasks7);
	if(!unit_uintArrayEquals(cSol2, sol, 2))
		unit_error("Solution 23: Bad sequence process");
	free(sol);
	
	unsigned int tasks8[] = {0, 1, 2, 3};
	unsigned int cSol3[] = {0, 1, 2, 3};
	sol = solution_sequenceProduction(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol3, sol, 4))
		unit_error("Solution 24: Bad sequence process");
	free(sol);
	
	instance->tasks[3]->machineDurations[0] = 0;
	unsigned int cSol4[] = {3, 0, 1, 2};
	sol = solution_sequenceProduction(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol4, sol, 4))
		unit_error("Solution 25: Bad sequence process");
	free(sol);
	
	instance_destroy(instance);
}

void solutionUnit_delivery()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance3.txt");
	
	unsigned int tasks9[] = {0, 2};
	unsigned int time = solution_deliveryDelay(instance, 2, tasks9, 100);
	if(time != 10)
		unit_error("Solution 26: Bad delivery delay, got %d expected %d", time, 10);
	
	time = solution_deliveryDelay(instance, 2, tasks9, 0);
	if(time != 0)
		unit_error("Solution 27: Bad delivery delay, got %d expected %d", time, 0);
	
	unsigned int tasks10[] = {0, 1};
	time = solution_deliveryDelay(instance, 2, tasks10, 100);
	if(time != 9)
		unit_error("Solution 28: Bad delivery delay, got %d expected %d", time, 9);
	
	unsigned int tasks11[] = {2, 0};
	time = solution_deliveryDelay(instance, 2, tasks11, 100);
	if(time != 10)
		unit_error("Solution 29: Bad delivery delay, got %d expected %d", time, 10);
	
	unsigned int tasks12[] = {0, 1, 2, 3};
	time = solution_deliveryDelay(instance, 4, tasks12, 100);
	if(time != 16)
		unit_error("Solution 30: Bad delivery delay, got %d expected %d", time, 16);
	
	unsigned int tasks13[] = {3, 0, 1, 2};
	time = solution_deliveryDelay(instance, 4, tasks13, 100);
	if(time != 25)
		unit_error("Solution 30: Bad delivery delay, got %d expected %d", time, 25);
	
	instance_destroy(instance);
}
