#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include "Solution.h"
#include "Instance.h"

/**
 * Initializes the first solution.
 *
 * @param instance The instance to build the solution for. Not null.
 * @return The initialized solution.
 */
Solution * tabu_solutionInit(Instance * instance);

#endif
