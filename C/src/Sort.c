#include "headers/Sort.h"

Solution * sort_swapDeliveries(Solution *solution, unsigned int task1, unsigned int task2)
{
	Solution * sol = solution_copy(solution);
	solution_switchTaskPack(solution, task1, task2);
	return sol;
}


// TODO EBSR & EFSR separated
Solution * sort_moveDeliveries(Solution *solution, unsigned int task, unsigned int pack)
{
	Solution * sol = solution_copy(solution);
	solution_moveTaskPack(solution, task, pack);
	return sol;
}
