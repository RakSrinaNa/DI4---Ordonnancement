#ifndef TABOU_PACK_H
#define TABOU_PACK_H

#include "Instance.h"
#include "Utils.h"

struct _Pack;

typedef struct _Pack
{
	Instance * instance; // Reference to the instance.
	unsigned int taskCount; // Number of tasks in this pack.
	unsigned int * deliveryOrder; // Ordered list of the IDs of the task representing the delivery order.
} Pack;

/**
 * Creates an empty pack.
 *
 * @return A new pack.
 */
Pack * pack_create(Instance *instance);

/**
 * Frees a pack.
 *
 * @param pack The pack to destroy.
 */
void pack_destroy(Pack * pack);

/**
 * Checks if a pack has a certain task.
 *
 * @param pack The pack.
 * @param task The task to check.
 * @return 1 if the task is present, 0 otherwise.
 */
Bool pack_hasTask(Pack *pack, unsigned int task);

/**
 * Gets the index of the given task.
 *
 * @param pack The pack.
 * @param task The task to get.
 */
int pack_getTaskIndex(Pack *pack, unsigned int task);

/**
 * Adds a task to the lists.
 *
 * @param pack The pack.
 * @param task The index of the task.
 */
void pack_addTask(Pack *pack, unsigned int task);

/**
 * Removes a task from the lists.
 *
 * @param pack The pack.
 * @param task The task to remove.
 */
void pack_removeTask(Pack *pack, unsigned int task);

/**
 * Switches two deliveries in the delivery list.
 *
 * @param pack The pack.
 * @param delivery1 The first delivery.
 * @param delivery2 The second delivery.
 */
void pack_switchDelivery(Pack *pack, unsigned int delivery1, unsigned int delivery2);

/**
 * Moves a delivery in the delivery list.
 *
 * @param pack The pack.
 * @param delivery The first delivery.
 * @param position The new position.
 */
void pack_moveDelivery(Pack *pack, unsigned int delivery, unsigned int position);

#endif //TABOU_PACK_H
