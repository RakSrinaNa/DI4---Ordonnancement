#ifndef TABOU_TABULIST_H
#define TABOU_TABULIST_H

#include "Utils.h"

//!Item of the Tabu list.
typedef struct _TabuItem
{
	//!Swap : one of the two swapped elements ; EBSR / EFSR : the moved task.
	unsigned int source;
	//!Swap : one of the two swapped elements ; EBSR / EFSR : the destination batch.
	unsigned int destination;
	//!The next element in the chained list.
	struct _TabuItem * next;
} TabuItem;

//!Tabu list.
typedef struct _TabuList{
	//!First element of the list.
	TabuItem * next;
	//!Last element of the list.
	TabuItem * last;
	//!Maximum size of the list.
	unsigned int maxSize;
	//!Current size of the list.
	unsigned int size;
} TabuList;

/**
 * Create a tabu list.
 *
 * @param maxSize The maximum size of the list.
 * @return The list.
 * @remark Needs to be freed with tabuList_destroy.
 */
TabuList * tabuList_create(unsigned int maxSize);

/**
 * Frees a tabu list.
 *
 * @param list The list to destroy.
*/
void tabuList_destroy(TabuList * list);

/**
 * Add an item to the list.
 * If the list becomes too big, the oldest element is removed.
 *
 * @param item The item to add.
 */
void tabuList_addItem(TabuItem * item);

/**
 * Search for an item in this list.
 *
 * @param item The item to search for.
 * @return True if found, False otherwise.
 */
Bool tabuList_contains(TabuItem * item);

#endif //TABOU_TABULIST_H
