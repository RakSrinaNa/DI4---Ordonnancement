#include "headers/Solution.h"

#include "headers/Utils.h"
#include "headers/Pack.h"

Solution* solution_create()
{
	Solution *solution = NULL;
	MMALLOC(solution, Solution, 1, "solution_create");
	solution->packCount = 0;
	solution->packList = NULL;
	return solution;
}

void solution_delete(Solution* solution)
{
	free(solution->packList);
	free(solution);
}
