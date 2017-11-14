#ifndef TABOU_PACK_H
#define TABOU_PACK_H

#include "Instance.h"
#include "Utils.h"

struct _Pack;

//!Structure representing a batch.
typedef struct _Pack
{
	//!Reference to the instance.
	Instance * instance;
	//!Number of tasks in this pack.
	unsigned int taskCount;
	//!Ordered list of the IDs of the task representing the delivery order.
	task_t * deliveries;
} Pack;

/**
 * Creates an empty pack.
 *
 *
 * @param instance The parent instance. Not null.
 * @return A new pack.
 * @remark Needs to be freed with pack_destroy.
 */
Pack * pack_create(Instance * instance);

/**
 * Frees a pack.
 *
 * @param pack The pack to destroy.
 */
void pack_destroy(Pack * pack);

/**
 * Checks if a pack has a certain task.
 *
 * @param pack The pack. Not null.
 * @param task The task id to check.
 * @return True if the task is present, False otherwise.
 */
Bool pack_hasTask(Pack * pack, task_t task);

/**
 * Gets the index of the given task inside a pack.
 *
 * @param pack The pack. Not null.
 * @param task The task id to get.
 * @return The index of the task inside the pack if it is present, -1 otherwise.
 */
int pack_getTaskIndex(Pack * pack, task_t task);

/**
 * Adds a task to the pack.
 *
 * @param pack The pack. Not null.
 * @param task The task id.
 */
void pack_addTask(Pack * pack, task_t task);

/**
 * Removes a task from the pack.
 *
 * @param pack The pack. Not null.
 * @param task The task id to remove.
 * @return True if the pack is now empty, False otherwise
 */
Bool pack_removeTask(Pack * pack, task_t task);

/**
 * Switches two deliveries in the delivery list.
 * Has no effect if one of the tasks is absent from the list.
 *
 * @param pack The pack. Not null.
 * @param delivery1 The first delivery id.
 * @param delivery2 The second delivery id.
 */
void pack_switchDelivery(Pack * pack, task_t delivery1, task_t delivery2);

/**
 * Moves a delivery in the delivery list.
 * If the position is greater than the task count, it is moved at the end of the list.
 *
 * @param pack The pack. Not null.
 * @param delivery The delivery id.
 * @param position The new position.
 */
void pack_moveDelivery(Pack * pack, task_t delivery, task_t position);

/**
 * Prints a pack.
 *
 * @param pack The pack.
 */
void pack_print(Pack * pack);

#endif
