#ifndef TABOU_SOLUTION_H
#define TABOU_SOLUTION_H

struct _Solution;

#include "SolutionInfo.h"
#include "Sequencer.h"
#include "Pack.h"

typedef struct _Solution
{
	Instance * instance; // Reference to the instance.
	unsigned int packCount; // Number of packs in this solution.
	Pack ** packList; // Ordered list of the packs in shipping order.
	struct _SolutionInfo * info;
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

/**
 * Switches two tasks between two packs.
 *
 * @param solution The solution. Not null.
 * @param task1 The first task.
 * @param task2 The second task.
 */
void solution_switchTaskPack(Solution * solution, unsigned int task1, unsigned int task2);

//TODO: unit
/**
 * Evaluates the score of the given solution and saves it in the info.
 * If the score has already been computed, no changes have been made and CACHED_SCORE = 1, the score is not computed again.
 *
 * @param The solution.
 * @return The info of the solution.
 */
struct _SolutionInfo * solution_eval(Solution * solution);

/**
 * Prints the solution.
 *
 * @param solution The solution.
 */
void solution_print(Solution * solution);

#endif //TABOU_SOLUTION_H
