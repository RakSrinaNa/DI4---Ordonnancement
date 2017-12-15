#ifndef TABOU_SEARCHRESULT_H
#define TABOU_SEARCHRESULT_H

#include "TabuList.h"
#include "Solution.h"

//!Structure containing the result of a search method.
typedef struct _SearchResult
{
	Solution * solution;
	TabuItem * tabuItem;
	SearchMethod method;
} SearchResult;

/**
 * Create a search result.
 * @param solution The best solution found.
 * @param tabuItem The tabu item to get to this solution. NULL if the solution wasn't improved.
 * @param method The method used.
 * @return The created search result.
 * @remark Needs to be freed with searchResult_destroy.
 */
SearchResult * searchResult_create(Solution * solution, TabuItem * tabuItem, SearchMethod method);

/**
 * Frees a search result.
 *
 * @param searchResult The result to destroy.
*/
void searchResult_destroy(SearchResult * searchResult);

#endif
