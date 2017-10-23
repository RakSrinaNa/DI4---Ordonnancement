#include "headers/Sort.h"

Solution * sort_swapDeliveries(Solution *solution, unsigned int task1, unsigned int task2)
{
	Solution * sol = solution_copy(solution);
	solution_switchTaskPack(solution, task1, task2);
	return sol;
}
