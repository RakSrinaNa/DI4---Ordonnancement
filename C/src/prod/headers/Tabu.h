#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include <sys/timeb.h>

#include "Solution.h"
#include "Instance.h"
#include "TabuList.h"
#include "SearchResult.h"

//!Contains the solution the tabu found, with the time take and iterations.
typedef struct _TabuSolution
{
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
void tabuSolution_destroy(TabuSolution * solution);

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

/**
 * Starts the tabu search on the given instance.
 * @param instance The instance to use.
 * @return The result of the tabu search.
 */
TabuSolution * tabu_search(Instance * instance);

/**
 * Apply the swap search method.
 * @param currentSolution The solution to start with.
 * @param tabuList The tabu list.
 * @param diversification If we diversify or not.
 * @return The result of this search.
 */
SearchResult * tabu_searchSwap(Solution * currentSolution, TabuList * tabuList, Bool diversification);

/**
 * Apply the EBSR search method.
 * @param currentSolution The solution to start with.
 * @param tabuList The tabu list.
 * @param diversification If we diversify or not.
 * @return The result of this search.
 */
SearchResult * tabu_searchEBSR(Solution * currentSolution, TabuList * tabuList, Bool diversification);

/**
 * Apply the EFSR search method.
 * @param currentSolution The solution to start with.
 * @param tabuList The tabu list.
 * @param diversification If we diversify or not.
 * @return The result of this search.
 */
SearchResult * tabu_searchEFSR(Solution * currentSolution, TabuList * tabuList, Bool diversification);

/**
 * Get the fingerprint of the flags activated.
 * @return An int formed of 0s or 1s.
 */
int tabu_flagsFingerprint();

/**
 * Randomize a solution.
 * @param solution The solution to randomize.
 */
void tabu_randomize(Solution *solution);

#endif
