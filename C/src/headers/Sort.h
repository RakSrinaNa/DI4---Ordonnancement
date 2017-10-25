#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include "Solution.h"

// TODO unit
/**
 * Duplicates a solution then swaps two deliveries between two distinct packs.
 *
 * @param solution The solution. Not null.
 * @param task1 The first task to swap.
 * @param task2 The second task to swap.
 * @return The duplicated and swapped solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * sort_swapDeliveries(Solution *solution, unsigned int task1, unsigned int task2);

// TODO unit
/**
 * Duplicates a solution then moves a delivery backwards.
 *
 * @param solution The solution. Not null.
 * @param task The task to move.
 * @param shift The number of packs away the task shall be moved backwards to.
 * @return The duplicated and moved solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * sort_moveDeliveriesEBSR(Solution *solution, unsigned int task, unsigned int shift);

// TODO unit
/**
 * Duplicates a solution then moves a delivery forward.
 *
 * @param solution The solution.
 * @param task The task to move.
 * @param shift The number of packs away the task shall be moved forward to.
 * @return The duplicated and moved solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * sort_moveDeliveriesEFSR(Solution *solution, unsigned int task, unsigned int shift);

#endif
