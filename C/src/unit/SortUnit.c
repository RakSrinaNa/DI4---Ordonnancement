#include "headers/SortUnit.h"
#include "../prod/headers/Solution.h"
#include "../prod/headers/Parser.h"
#include "../prod/headers/Sort.h"
#include "headers/UnitUtils.h"

void sortUnit()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance2.txt");
	Solution * solution = solution_create(instance);
	
	solution_moveTaskPack(solution, 3, 1);
	solution_moveTaskPack(solution, 4, 1);
	
	Solution * swapped = sort_swapDeliveries(solution, 1, 4);
	if(swapped == solution)
		unit_error("Sort 1: Solution is not duplicated");
	if(solution_getTaskPack(swapped, 0) != 0 || solution_getTaskPack(swapped, 1) != 1 || solution_getTaskPack(swapped, 2) != 0 || solution_getTaskPack(swapped, 3) != 1 || solution_getTaskPack(swapped, 4) != 0)
		unit_error("Sort 2: Wrong swap");
	solution_destroy(swapped);
	
	swapped = sort_swapDeliveries(solution, 0, 1);
	if(swapped == solution)
		unit_error("Sort 3: Solution is not duplicated");
	if(solution_getTaskPack(swapped, 0) != 0 || solution_getTaskPack(swapped, 1) != 0 || solution_getTaskPack(swapped, 2) != 0 || solution_getTaskPack(swapped, 3) != 1 || solution_getTaskPack(swapped, 4) != 1)
		unit_error("Sort 4: Wrong swap");
	solution_destroy(swapped);
	
	solution_destroy(solution);
	instance_destroy(instance);
}
