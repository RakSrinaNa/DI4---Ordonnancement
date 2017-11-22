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
	TabuItem * first;
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
 * @param list The list to add into. Not null.
 * @param item The item to add.
 * @remark The item should be in the heap.
 */
void tabuList_addItem(TabuList * list, TabuItem * item);

/**
 * Search for an item in this list.
 *
 * @param list The list to search in.
 * @param item The item to search for.
 * @return True if found, False otherwise.
 */
Bool tabuList_contains(TabuList * list, TabuItem * item);

/**
 * Compares two items of the list.
 *
 * @param item1 The first item.
 * @param item2 The second item.
 * @return True if the same, False otherwise.
 */
Bool tabuItem_isSame(TabuItem * item1, TabuItem * item2);

/**
 * Create a tabu item.
 *
 * @param source The source of the item.
 * @param destination The destination of the item.
 * @return The item.
 * @remark Needs to be freed with tabuItem_destroy.
 */
TabuItem * tabuItem_create(unsigned int source, unsigned int destination);

/**
 * Frees a tabu item.
 *
 * @param item The item to destroy.
*/
void tabuItem_destroy(TabuItem * item);

#endif //TABOU_TABULIST_H