#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include <sys/timeb.h>

#include "Solution.h"
#include "Instance.h"
#include "TabuList.h"

//!Contains the solution the tabu found, with the time take and iterations.
typedef struct _TabuSolution {
	//!The solution found.
	Solution * solution;
	//!The number of iterations done.
	unsigned int iterations;
	//!The time taken.
	long double time;
} TabuSolution;

/**
 * Creates a tabu solution.
 *
 * @param solution The solution found.
 * @param iterations The number of iterations done.
 * @param time The time taken.
 * @return A new tabu solution.
 * @remark Needs to be freed with tabuSolution_destroy.
 */
TabuSolution * tabuSolution_create(Solution * solution, unsigned int iterations, long double time);


/**
 * Frees a tabu solution.
 *
 * @param solution The solution to destroy.
*/
TabuSolution * tabuSolution_destroy(TabuSolution * solution);

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
TabuSolution * tabu_search(Instance * instance);

Solution * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification);

Solution * tabu_searchEBSR(Solution * currentSolution, TabuList * tabuList, Bool diversification);

Solution * tabu_searchEFSR(Solution * currentSolution, TabuList * tabuList, Bool diversification);

#endif
