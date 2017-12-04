#ifndef TABOU_SEARCHRESULT_H
#define TABOU_SEARCHRESULT_H

#include "TabuList.h"
#include "Solution.h"

typedef enum _SearchMethod{
	SWAP,
	EBSR,
	EFSR
} SearchMethod;

typedef struct _SearchResult{
	Solution * solution;
	TabuItem * tabuItem;
	SearchMethod method;
} SearchResult;

//TODO Doc
SearchResult * searchResult_create(Solution * solution, TabuItem * tabuItem, SearchMethod method);

//TODO Doc
void searchResult_destroy(SearchResult * searchResult);

#endif
