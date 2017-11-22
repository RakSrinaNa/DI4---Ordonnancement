#ifndef TABOU_TABULIST_H
#define TABOU_TABULIST_H

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

#endif //TABOU_TABULIST_H
