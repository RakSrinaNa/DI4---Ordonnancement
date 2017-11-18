#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include <sys/timeb.h>

#include "Solution.h"
#include "Instance.h"

/**
 * Initializes the first solution.
 *
 * @param instance The instance to build the solution for. Not null.
 * @return The initialized solution.
 * @remark The user is in charge of freeing the returned value.
 */
Solution * tabu_solutionInit(Instance * instance);

/**
 * Get the difference between two times in seconds.
 *
 * @param start The starting time.
 * @param end The ending time.
 * @return The elapsed time in seconds.
 */
long double tabu_getTimeDiff(struct timeb start, struct timeb end);

//TODO doc
Solution * tabu_search(Instance * instance);

void tabu_searchSwap(Solution ** currentSolution, Solution ** bestSolution, Solution ** bestNeighbor);

void tabu_searchEBSR(Solution ** currentSolution, Solution ** bestSolution, Solution ** bestNeighbor);

void tabu_searchEFSR(Solution ** currentSolution, Solution ** bestSolution, Solution ** bestNeighbor);

#endif
