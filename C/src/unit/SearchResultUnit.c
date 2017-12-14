#include "headers/SearchResultUnit.h"
#include "../prod/headers/SearchResult.h"
#include "../prod/headers/Parser.h"
#include "headers/UnitUtils.h"

void searchResultUnit()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance6.txt");
	Solution * solution = solution_create(instance);
	TabuItem * item = tabuItem_create(2, 5, SWAP);
	SearchResult * result = searchResult_create(solution, item, SWAP);
	
	if(result->solution != solution || result->method != SWAP || result->tabuItem != item)
		unit_error("SearchResult 1: Wrong result.");
	
	searchResult_destroy(result);
	instance_destroy(instance);
}
