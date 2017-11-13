#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include "Solution.h"

/**
 * Duplicates a solution then swaps two deliveries between two distinct packs.
 *
 * @param solution The solution. Not null.
 * @param task1 The first task to swap.
 * @param task2 The second task to swap.
 * @return The duplicated and swapped solution.
 * @remark The user is in charge of freeing the returned value.
 * @remark No action performed if one of the tasks doesn't exist.
 */
Solution * sort_swapDeliveries(Solution * solution, task_t task1, task_t task2);

/**
 * Duplicates a solution then moves a delivery backwards.
 *
 * @param solution The solution. Not null.
 * @param task The task to move.
 * @param shift The number of packs away the task shall be moved backwards to.
 * @return The duplicated and moved solution.
 * @remark The user is in charge of freeing the returned value.
 * @remark If the task doesn't exist, no move is performed but the solution is still duplicated.
 * @remark If shift implies that the task is moved outside of the number of packs, it is moved to the first pack.
 */
Solution * sort_moveDeliveriesEBSR(Solution * solution, task_t task, unsigned int shift);

/**
 * Duplicates a solution then moves a delivery forward.
 *
 * @param solution The solution.
 * @param task The task to move.
 * @param shift The number of packs away the task shall be moved forward to.
 * @return The duplicated and moved solution.
 * @remark The user is in charge of freeing the returned value.
 * @remark If the task doesn't exist, no move is performed but the solution is still duplicated.
 * @remark If shift implies that the task is moved outside of the number of packs, it is moved to the last pack.
 */
Solution * sort_moveDeliveriesEFSR(Solution * solution, task_t task, unsigned int shift);

#endif
