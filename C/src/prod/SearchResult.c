#include "headers/SearchResult.h"

SearchResult * searchResult_create(Solution * solution, TabuItem * tabuItem, SearchMethod method)
{
	SearchResult * searchResult = NULL;
	MMALLOC(searchResult, SearchResult, 1, "searchResult_create");
	searchResult->solution = solution;
	searchResult->tabuItem = tabuItem;
	searchResult->method = method;
	return  searchResult;
}

void searchResult_destroy(SearchResult * searchResult)
{
	if(searchResult == NULL)
		return;
	solution_destroy(searchResult->solution);
	tabuItem_destroy(searchResult->tabuItem);
	free(searchResult);
}
