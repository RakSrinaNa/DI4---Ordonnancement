#include "headers/Sort.h"

Solution * sort_swapDeliveries(Solution *solution, unsigned int task1, unsigned int task2)
{
	Solution * sol = solution_copy(solution);
	solution_switchTaskPack(solution, task1, task2);
	return sol;
}

Solution * sort_moveDeliveriesEBSR(Solution *solution, unsigned int task, unsigned int shift)
{
	int packIndex = solution_getTaskPack(solution, task);
	if(packIndex < 0)
	{
		warn("sort_moveDeliveriesEBSR : task does not exist (%d)", task);
		return NULL;
	}
	Solution * sol = solution_copy(solution);
	if(shift == 0)
		return sol;
	solution_moveTaskPack(solution, task, MMIN(packIndex + shift, solution->packCount));
	return sol;
}

Solution * sort_moveDeliveriesEFSR(Solution *solution, unsigned int task, unsigned int shift)
{
	int packIndex = solution_getTaskPack(solution, task);
	if(packIndex < 0)
	{
		warn("sort_moveDeliveriesEFSR : task does not exist (%d)", task);
		return NULL;
	}
	Solution * sol = solution_copy(solution);
	if(shift == 0)
		return sol;
	solution_moveTaskPack(solution, task, MMAX(packIndex - shift, 0));
	return sol;
}
