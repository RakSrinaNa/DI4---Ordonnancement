#ifndef SOLUTIONINFO_H_INCLUDED
#define SOLUTIONINFO_H_INCLUDED

struct _SolutionInfo;

#include "Solution.h"

typedef struct _SolutionInfo
{
	unsigned int * productionOrder;
	unsigned int * readyToDeliver;
	unsigned int ** deliveries;
} SolutionInfo;

// TODO unit
// TODO doc
SolutionInfo * solutionInfo_create(struct _Solution * solution);

// TODO unit
// TODO doc
void solutionInfo_destroy(struct _Solution * solution, SolutionInfo * info);

#endif // SOLUTIONINFO_H_INCLUDED
