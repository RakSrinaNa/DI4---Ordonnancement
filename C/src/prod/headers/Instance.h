#ifndef TABOU_INSTANCE_H
#define TABOU_INSTANCE_H

struct _Instance;

//!Structure defining an instance.
typedef struct _Instance
{
	//!Count of machines.
	unsigned int machineCount;
	//!Count of tasks.
	unsigned int taskCount;
	//!Matrix of the distances for the delivery.
	unsigned int ** distancesMatrix;
	//!List of the tasks of the instance.
	struct _Task ** tasks;
	//!Origin file.
	const char * origin;
} Instance;

#include <stdio.h>
#include "Task.h"
#include "Utils.h"

/**
 * Creates a new instance ready to be used.
 *
 * @return A new instance.
 * @remark Needs to be freed with instance_destroy.
 */
Instance * instance_create();

/**
 * Frees an instance.
 *
 * @param instance The instance to free.
 */
void instance_destroy(Instance * instance);

/**
 * Set a distance in the matrix.
 * If the path isn't recognized, nothing will be changed.
 *
 * @param instance The instance concerned. Not null.
 * @param from The starting point.
 * @param to The ending point.
 * @param distance The distance between these points.
 */
static inline void instance_setDistance(Instance * instance, unsigned int from, unsigned int to, unsigned int distance)
{
	if(from > instance->taskCount || to > instance->taskCount)
		warn("instance_setDistance : Error when setting distance, index out of range (from: %d, to: %d).", from, to);
	else
		instance->distancesMatrix[from][to] = distance;
}

/**
 * Get a distance from the matrix.
 * If the path isn't known, a fatal error is thrown.
 *
 * @param instance The instance concerned. Not null.
 * @param from The starting point.
 * @param to The ending point.
 * @return The distance between these points.
 */
static inline int instance_getDistance(Instance * instance, unsigned int from, unsigned int to)
{
	if(from > instance->taskCount || to > instance->taskCount)
	{
		fatalError("instance_getDistance : Error when getting distance, index out of range (from: %d, to: %d).\n", from, to);
	}
	return instance->distancesMatrix[from][to];
}

/**
 * Gets the due date of the given task.
 * If the task isn't found, a fatal error is thrown.
 *
 * @param instance The instance concerned. Not null.
 * @param task The task.
 * @return The due date of the task.
 */
unsigned int instance_getDueDate(Instance * instance, task_t task);

/**
 * Sorts tasks by due date in a new array.
 *
 *
 * @param instance The instance.
 * @return A sorted table of the tasks.
 * @remark The user is in charge of freeing the returned value.
 */
unsigned int * instance_sortByDueDate(Instance * instance);

/**
 * Prints an instance in the console.
 *
 * @param instance The instance to print. Not null.
 */
void instance_print(Instance * instance);

#endif
