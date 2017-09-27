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
	if(solution->processOrder[0] != 3 || solution->processOrder[2] != 2 || solution->processOrder[3] != 4)
		unit_error("Solution 3: Bad process index");
	
	solution_setProcessIndex(solution, 99, 2);
	if(solution->processOrder[2] != 2)
		unit_error("Solution 4: Bad process index");
	
	instance_destroy(instance);
}
