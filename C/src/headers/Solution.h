#ifndef TABOU_SOLUTION_H
#define TABOU_SOLUTION_H

#include "Pack.h"

struct _Solution;

typedef struct _Solution
{
	Instance *instance; // Reference to the instance.
	unsigned int packCount; // Number of packs in this solution.
	Pack *packList; // Ordered list of the packs in shipping order.
	unsigned int * processOrder; // Order of the processes of the solution.
} Solution;

/**
 * Creates an empty solution.
 *
 * @return A new solution.
*/
Solution* solution_create(Instance *instance);


/**
 * Frees a solution.
 *
 * @param solution The solution to destroy.
*/
void solution_destroy(Solution *solution);

#endif //TABOU_SOLUTION_H
