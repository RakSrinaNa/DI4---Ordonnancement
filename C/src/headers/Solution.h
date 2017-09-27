#ifndef TABOU_SOLUTION_H
#define TABOU_SOLUTION_H

#include "Pack.h"

struct _Solution;

typedef struct _Solution
{
	Instance * instance; // Reference to the instance.
	unsigned int packCount; // Number of packs in this solution.
	Pack ** packList; // Ordered list of the packs in shipping order.
	unsigned int * processOrder; // Order of the processes of the solution.
} Solution;

/**
 * Creates an empty solution.
 *
 * @return A new solution.
*/
Solution * solution_create(Instance * instance);

/**
 * Frees a solution.
 *
 * @param solution The solution to destroy.
*/
void solution_destroy(Solution * solution);

/**
 * Gets the index of the given pack.
 *
 * @param solution The solution.
 * @param pack The pack id to get.
 * @return The index of the pack if it is present, -1 otherwise.
 */
int solution_getPackIndex(Solution * solution, unsigned int pack);

/**
 * Gets the index of the given task.
 *
 * @param solution The solution.
 * @param task The task id to get.
 * @return The index of the task if it is present, -1 otherwise.
 */
int solution_getProcessIndex(Solution * solution, unsigned int task);

/**
 * Moves a task in the task list.
 * If the position is greater than the task count, it is moved at the end of the list.
 *
 * @param solution The solution.
 * @param task The task to move.
 * @param position The new position.
 */
void solution_setProcessIndex(Solution * solution, unsigned int task, unsigned int position);

/**
 * Gets the corresponding pack for the given task.
 *
 * @param solution The solution.
 * @param task The task to search.
 * @return The index of the pack containing the task. If the task doesn't exist, returns -1. If the task exists, but is not in any pack, it exits with value -1.
 */
int solution_getTaskPack(Solution * solution, unsigned int task);

/**
 * Assigns a pack to the given task.
 *
 * @param solution The solution.
 * @param task The task.
 * @param pack The pack to assign.
 */
void solution_moveTaskPack(Solution * solution, unsigned int task, unsigned int pack);

#endif //TABOU_SOLUTION_H
