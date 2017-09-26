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
 * @param task The task id to check.
 * @return True if the task is present, False otherwise.
 */
Bool pack_hasTask(Pack *pack, unsigned int task);

/**
 * Gets the index of the given task.
 *
 * @param pack The pack.
 * @param task The task id to get.
 * @return The index of the task if it is present, -1 otherwise.
 */
int pack_getTaskIndex(Pack *pack, unsigned int task);

/**
 * Adds a task to the lists.
 *
 * @param pack The pack.
 * @param task The task id.
 */
void pack_addTask(Pack *pack, unsigned int task);

/**
 * Removes a task from the lists.
 *
 * @param pack The pack.
 * @param task The task id to remove.
 */
void pack_removeTask(Pack *pack, unsigned int task);

/**
 * Switches two deliveries in the delivery list.
 * Has no effect if one of the tasks is absent from the list.
 *
 * @param pack The pack.
 * @param delivery1 The first delivery id.
 * @param delivery2 The second delivery id.
 */
void pack_switchDelivery(Pack *pack, unsigned int delivery1, unsigned int delivery2);

/**
 * Moves a delivery in the delivery list.
 * If the position is greater than the task count, it is moved at the end of the list.
 *
 * @param pack The pack.
 * @param delivery The first delivery id.
 * @param position The new position.
 */
void pack_moveDelivery(Pack *pack, unsigned int delivery, unsigned int position);

#endif //TABOU_PACK_H
