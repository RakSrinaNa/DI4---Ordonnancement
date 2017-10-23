#ifndef SOLUTIONINFO_H_INCLUDED
#define SOLUTIONINFO_H_INCLUDED

struct _SolutionInfo;

#include "Solution.h"

typedef struct _SolutionInfo
{
	unsigned int * productionOrder; // The ordered production.
	unsigned int * readyToDeliver; // Dates when the truck is ready to go.
	unsigned int ** deliveries; // The ordered deliveries.
	unsigned int score; // The score of the info.
} SolutionInfo;

/**
 * Creates an info.
 *
 * @param solution The solution.
 * @return A new info.
 */
SolutionInfo * solutionInfo_create(struct _Solution * solution);

/**
 * Destroys an info.
 *
 * @param solution The solution.
 * @param info The info to destroy.
 */
void solutionInfo_destroy(struct _Solution * solution, SolutionInfo * info);

/**
 * Orders the total production.
 *
 * @param solution The solution.
 * @return An ordered info.
 */
struct _SolutionInfo * solutionInfo_productionOrder(struct _Solution * solution);

/**
 * Orders a delivery pack.
 *
 * @param solution The solution.
 * @param info The info to fill.
 */
void solutionInfo_deliveryOrder(struct _Solution * solution, struct _SolutionInfo * info);


#endif // SOLUTIONINFO_H_INCLUDED
