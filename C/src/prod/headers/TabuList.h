#ifndef TABOU_TABULIST_H
#define TABOU_TABULIST_H

//!Item of the Tabu list.
typedef struct _TabuItem
{
	//!Swap : one of the two swapped elements ; EBSR / EFSR : the moved task.
	unsigned int source;
	//!Swap : one of the two swapped elements ; EBSR / EFSR : the destination batch.
	unsigned int destination;
} TabuItem;

typedef struct _TabuList{
	TabuItem * item;
	struct _TabuList * next;
	unsigned int maxSize;
} TabuList;

TabuList * tabuList_create(unsigned int maxSize);

void tabuLost_destroy();

#endif //TABOU_TABULIST_H
