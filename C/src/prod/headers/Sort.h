#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include "Solution.h"

// TODO unit
// TODO @Schttopup What if a task doesn't exists?
/**
 * Duplicates a solution then swaps two deliveries between two distinct packs.
 *
 * @param solution The solution. Not null.
 * @param task1 The first task to swap.
 * @param task2 The second task to swap.
 * @return The duplicated and swapped solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * sort_swapDeliveries(Solution * solution, task_t task1, task_t task2);

// TODO unit
// TODO @Schttopup What if shift is bigger than the # of packs? // If task does not exists?
// TODO Perso le fait de renvoyer NULL dans ce cas, je rouve pas cohérent avec swap qui renvoie sol.
// TODO J'opterais pour faire en sorte que l'on affiche un warn si on swap 2 sur le même pack, et on revoie sol,
// TODO pareil pour EBxR, on warn et renvoie une copie.
/**
 * Duplicates a solution then moves a delivery backwards.
 *
 * @param solution The solution. Not null.
 * @param task The task to move.
 * @param shift The number of packs away the task shall be moved backwards to.
 * @return The duplicated and moved solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * sort_moveDeliveriesEBSR(Solution * solution, task_t task, unsigned int shift);

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
Solution * sort_moveDeliveriesEFSR(Solution * solution, task_t task, unsigned int shift);

#endif
