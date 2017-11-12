#include "headers/Solution.h"

Solution * sort_swapDeliveries(Solution * solution, task_t task1, task_t task2)
{
	debugPrint("Applying swap on tasks %d and %d in solution %p\n", task1, task2, solution);
	Solution * sol = solution_copy(solution);
	solution_switchTaskPack(sol, task1, task2);
	return sol;
}

Solution * sort_moveDeliveriesEBSR(Solution * solution, task_t task, unsigned int shift)
{
	debugPrint("Applying EBSR on task %d with a shift of %d in solution %p\n", task, shift, solution);
	int packIndex = solution_getTaskPack(solution, task);
	if(packIndex < 0)
	{
		warn("sort_moveDeliveriesEBSR : Task %d does not exist.\n", task);
		return NULL;
	}
	Solution * sol = solution_copy(solution);
	if(shift == 0)
		return sol;
	solution_moveTaskPack(solution, task, MMIN(packIndex + shift, solution->packCount));
	return sol;
}

Solution * sort_moveDeliveriesEFSR(Solution * solution, task_t task, unsigned int shift)
{
	debugPrint("Applying EFSR on task %d with a shift of %d in solution %p\n", task, shift, solution);
	int packIndex = solution_getTaskPack(solution, task);
	if(packIndex < 0)
	{
		warn("sort_moveDeliveriesEFSR : Task %d does not exist.\n", task);
		return NULL;
	}
	Solution * sol = solution_copy(solution);
	if(shift == 0)
		return sol;
	solution_moveTaskPack(solution, task, MMAX(packIndex - shift, 0));
	return sol;
}
