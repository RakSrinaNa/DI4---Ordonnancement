#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include "Solution.h"
#include "Instance.h"

//TODO: @Schttopup Doc
/**
 *
 * @param instance The instance to build the solution for. Not null.
 * @return
 */
Solution * tabu_solutionInit(Instance * instance);

#endif
