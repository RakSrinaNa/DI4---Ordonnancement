#include "headers/SolutionInfo.h"
#include "headers/Solution.h"

SolutionInfo * solutionInfo_create(Solution * solution)
{
	SolutionInfo *info = NULL;
	MMALLOC(info, SolutionInfo, 1, "solutionInfo_create");
	MMALLOC(info->productionOrder, unsigned int, solution->instance->taskCount, "solutionInfo_create");
	MMALLOC(info->readyToDeliver, unsigned int, solution->packCount, "solutionInfo_create");
	MMALLOC(info->deliveries, unsigned int *, solution->packCount, "solutionInfo_create");
	for(unsigned int i = 0; i < solution->packCount; i++)
		info->deliveries[i] = NULL;
	return info;
}

void solutionInfo_destroy(Solution * solution, SolutionInfo * info)
{
	free(info->productionOrder);
	free(info->readyToDeliver);
	for(unsigned int i = 0; i < solution->packCount; i++)
		free(info->deliveries[i]);
	free(info->deliveries);
	free(info);
}
