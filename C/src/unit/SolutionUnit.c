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
	
	instance_destroy(instance);
}
