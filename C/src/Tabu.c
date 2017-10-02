#include "headers/Tabu.h"

Solution * tabu_solutionInit(Instance * instance)
{
    Solution *solution = solution_create(instance);
    Solution *bestSolution = NULL;
    solution_sortByDD(solution);
    unsigned int nbPackTasks = 1;
    while(nbPackTasks <= instance->taskCount / 2)
    {
        Solution *solutionCopy = solution_copy(solution);
        for(unsigned int i = 0; i < instance->taskCount; i++)
        {
            solution_moveTaskPack(solution, solution->processOrder[i], i/nbPackTasks);
        }
        if(bestSolution == NULL || solution_eval(solutionCopy) > solution_eval(bestSolution))
        {
            solution_destroy(bestSolution);
            bestSolution = solutionCopy;
        }
        else
            solution_destroy(solutionCopy);
        nbPackTasks++;
    }
    solution_destroy(solution);
    return bestSolution;
}
