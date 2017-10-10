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
	int score; // Cached score of the solution (-1 if not calculated).
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
 * Copies a solution.
 *
 * @param solution The solution to copy.
 * @return A new solution.
 */
Solution * solution_copy(Solution * solution);

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

/**
 * Orders a pack by due date.
 *
 * @param solution The solution.
 * @param pack The pack.
 */
void solution_sortByDD(Solution *solution);


int solution_eval(Solution *solution);
//TODO A tester
unsigned int solution_processFinalTime(Instance * instance, unsigned int count, unsigned int * processes);
// TODO a tester
unsigned int * solution_sequenceProcess(Instance *instance, unsigned int taskCount, unsigned int *pack);
// TODO a tester

unsigned int solution_deliveryDelay(Instance *instance, unsigned int count, unsigned int * deliveries, unsigned int date);
// TODOa tester
unsigned int * solution_sequenceDeliveries(Instance *instance, unsigned int taskCount, unsigned int * deliveries, unsigned int date);
// TODO a tester

/**
 * Prints the solution.
 */
void solution_print(Solution *solution);

#endif //TABOU_SOLUTION_H
