#ifndef TABOU_INSTANCE_H
#define TABOU_INSTANCE_H

struct _Instance;

//Structure defining an instance.
typedef struct _Instance
{
	unsigned int machineCount; //Count of machines.
	unsigned int taskCount; //Count of tasks.
	unsigned int ** distancesMatrix; //Matrix of the distances for the delivery.
	struct _Task ** tasks; //List of the tasks of the instance.
} Instance;

#include <stdio.h>
#include "Task.h"
#include "Utils.h"

/**
 * Frees an instance.
 *
 * @param instance The instance to free.
 */
void instance_destroy(Instance * instance);

/**
 * Creates a new instance ready to be used.
 *
 * @return A new instance.
 */
Instance * instance_create();

/**
 * Set a distance in the matrix.
 *
 * @param instance The instance concerned.
 * @param from The starting point.
 * @param to The ending point.
 * @param distance The distance between these points.
 */
static inline void instance_setDistance(Instance * instance, unsigned int from, unsigned int to, unsigned int distance)
{
	if(from > instance->taskCount || to > instance->taskCount)
	{
		fatal_error("CRITICAL : instance_setDistance : Error when setting distance, index out of range (from: %d, to: %d).", from, to);
	}
	instance->distancesMatrix[from][to] = distance;
}

/**
 * Get a distance from the matrix.
 *
 * @param instance The instance concerned.
 * @param from The starting point.
 * @param to The ending point.
 * @return The distance between these points.
 */
static inline unsigned int instance_getDistance(Instance * instance, unsigned int from, unsigned int to)
{
	if(from > instance->taskCount || to > instance->taskCount)
	{
		fatal_error("CRITICAL : instance_getDistance : Error when getting distance, index out of range (from: %d, to: %d).", from, to);
	}
	return instance->distancesMatrix[from][to];
}

/**
 * Prints an instance in the console.
 *
 * @param instance The instance to print.
 */
void instance_print(Instance * instance);

#endif //TABOU_INSTANCE_H
