#ifndef TABOU_PACK_H
#define TABOU_PACK_H

struct _Pack;

typedef struct _Pack
{
	Instance * instance; // Reference to the parent instance.
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
 * @param pack The pack to destroy.
 */
void pack_destroy(Pack * pack);

/**
 * Adds a task to the lists.
 *
 * @param pack The pack.
 * @param task The index of the task.
 */
int pack_addTask(Pack *pack, unsigned int task);

/**
 * Switches two processes in the process list.
 *
 * @param pack The pack.
 * @param process1 The first process.
 * @param process2 The second process.
 */
int pack_switchProcess(Pack *pack, unsigned int process1, unsigned int process2);

/**
 * Switches two deliveries in the delivery list.
 *
 * @param pack The pack.
 * @param delivery1 The first delivery.
 * @param delivery2 The second delivery.
 */
int pack_switchDelivery(Pack *pack, unsigned int delivery1, unsigned int delivery2);


/**
 * Removes a task from the lists.
 *
 * @param pack The pack.
 * @param task The task to remove.
 */
int pack_removeTask(Pack *pack, unsigned int task);

#endif //TABOU_PACK_H
