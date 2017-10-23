#include "headers/SolutionInfoUnit.h"
#include "../headers/Instance.h"
#include "../headers/Parser.h"
#include "../headers/SolutionInfo.h"
#include "headers/UnitUtils.h"

void solutionInfoUnit()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance6.txt");
	
	Solution * solution = solution_create(instance);
	solution_moveTaskPack(solution, 2, 1);
	solution_moveTaskPack(solution, 3, 1);
	solution_moveTaskPack(solution, 4, 2);
	
	SolutionInfo * info = solutionInfo_productionOrder(solution);
	if(info == NULL)
		unit_error("SolutionInfo 1: Null infos");
	
	unsigned int prodOrder[] = {1, 0, 2, 3, 4};
	unsigned int ready[] = {18, 38, 57};
	
	if(!unit_uintArrayEquals(prodOrder, info->productionOrder, 5))
		unit_error("SolutionInfo 2: Bad production order");
	if(!unit_uintArrayEquals(ready, info->readyToDeliver, 3))
		unit_error("SolutionInfo 3: Bad ready to deliver");
	
	solutionInfo_destroy(solution, info);
	solution_destroy(solution);
	instance_destroy(instance);
}
