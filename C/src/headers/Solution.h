#ifndef TABOU_SOLUTION_H
#define TABOU_SOLUTION_H

struct _Solution;

#include "Sequencer.h"
#include "Pack.h"

typedef struct _Solution
{
	Instance * instance; // Reference to the instance.
	unsigned int packCount; // Number of packs in this solution.
	Pack ** packList; // Ordered list of the packs in shipping order.
	SolutionInfo * info;
} Solution;

/**
 * Creates an empty solution.
 *
 * @param instance The instance the solution is for. Not null.
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
 * Copies a solution.
 *
 * @param solution The solution to copy.
 * @return A new solution.
 */
Solution * solution_copy(Solution * solution);

/**
 * Gets the production index of the given task.
 *
 * @param solution The solution. Not null.
 * @param task The task id to get.
 * @return The index of the task if it is present, -1 otherwise.
 */
int solution_getProductionIndex(Solution * solution, unsigned int task);

/**
 * Moves a task in the production list.
 * If the position is greater than the task count, it is moved at the end of the list.
 *
 * @param solution The solution. Not null.
 * @param task The task to move.
 * @param position The new position.
 */
void solution_setProductionIndex(Solution * solution, unsigned int task, unsigned int position);

/**
 * Gets the corresponding pack index for the given task.
 *
 * @param solution The solution. Not null.
 * @param task The task to search.
 * @return The index of the pack containing the task. If the task doesn't exist, returns -1. If the task exists, but is not in any pack, a fatal error is thrown (and it returns value -1).
 */
int solution_getTaskPack(Solution * solution, unsigned int task);

/**
 * Assigns a pack to the given task.
 *
 * @param solution The solution. Not null.
 * @param task The task.
 * @param pack The pack to assign.
 */
void solution_moveTaskPack(Solution * solution, unsigned int task, unsigned int pack);

//TODO: @Schttopup Doc
int solution_eval(Solution * solution);

/**
 * Prints the solution.
 */
void solution_print(Solution * solution);

#endif //TABOU_SOLUTION_H
