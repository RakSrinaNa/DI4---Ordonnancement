#ifndef TABOU_PACK_H
#define TABOU_PACK_H

struct _Pack;

typedef struct _Pack{
	unsigned int taskCount; //Number of tasks in this pack.
	unsigned int * processOrder; //Ordered list of the IDs of the task representing the process order.
	unsigned int * deliveryOrder; //Ordered list of the IDs of the task representing the delivery order.
} Pack;

/**
 * Creates an empty pack.
 *
 * @return A new pack.
 */
Pack * pack_create();

/**
 * Frees a pack.
 *
 * @return The pack to destroy.
 */
void pack_destroy(Pack * pack);

#endif //TABOU_PACK_H
