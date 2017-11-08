#ifndef SOLUTIONINFO_H_INCLUDED
#define SOLUTIONINFO_H_INCLUDED

struct _SolutionInfo;

#include "Solution.h"

//Stores the computed informations of a solution.
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
 * @param solution The solution. Not null.
 * @return A new info.
 * @remark Needs to be freed with solutionInfo_destroy.
 */
SolutionInfo * solutionInfo_create(struct _Solution * solution);

/**
 * Destroys an info.
 *
 * @param solution The solution. Not null.
 * @param info The info to destroy.
 */
void solutionInfo_destroy(struct _Solution * solution, SolutionInfo * info);

/**
 * Orders the entire production.
 *
 * @param solution The solution. Not null.
 * @return An ordered info.
 * @remark The user is in charge of freeing the returned value.
 */
struct _SolutionInfo * solutionInfo_productionOrder(struct _Solution * solution);

/**
 * Orders a delivery pack.
 *
 * @param solution The solution. Not null.
 * @param info The info to fill. Not null.
 */
void solutionInfo_deliveryOrder(struct _Solution * solution, struct _SolutionInfo * info);

/**
 * Prints a solution info.
 *
 * @param solution The solution. Not null.
 * @param info The info.
 */
void solutionInfo_print(struct _Solution * solution, struct _SolutionInfo * info);


#endif // SOLUTIONINFO_H_INCLUDED
