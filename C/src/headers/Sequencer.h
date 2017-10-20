#ifndef TABOU_SEQUENCER_H
#define TABOU_SEQUENCER_H

#include "Instance.h"

typedef struct _SolutionInfo
{
	unsigned int * productionOrder;
	unsigned int * readyToDeliver;
	unsigned int ** deliveries;
} SolutionInfo;

/**
 * Computes the total production time.
 *
 * @param instance The instance.
 * @param count The number of tasks.
 * @param tasks A table of the tasks.q
 * @return The total delay of the sequence.
 */
unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, unsigned int const * tasks);

/**
 * Orders the production in the given list.
 *
 * @param instance The instance.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param date A pointer to the starting time. At the end, contains the ending time.
 * @return An ordered sequence of deliveries.
 */
unsigned int * sequencer_sequenceProductionPack(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int * date);

/**
 * Computes the total delay of this sequence.
 * Also updates parameter date with the arrival time of the truck.
 *
 * @param instance The instance.
 * @param count The number of tasks.
 * @param tasks A table of the tasks.
 * @param date A pointer to the departure time. At the end, contains the arrival time.
 * @return The total delay of the sequence.
 */
unsigned int sequencer_deliveryDelay(Instance * instance, unsigned int count, unsigned int * tasks, unsigned int * date);

/**
 * Orders the deliveries in the given list.
 *
 * @param instance The instance.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time.
 * @return An ordered sequence of deliveries.
 */
unsigned int * sequencer_sequenceDeliveriesPack(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int * initialDate);

/**
 * Orders the deliveries in the given list by nearest neighbor.
 *
 * @param instance The instance.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time.
 * @return An ordered sequence of deliveries.
 */
unsigned int * sequencer_sequenceDeliveriesNearestNeighbor(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int * initialDate);

/**
 * Orders the deliveries in the given list by due date.
 *
 * @param instance The instance.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time.
 * @return An ordered sequence of deliveries.
 */
unsigned int * sequencer_sequenceDeliveriesDueDate(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int * initialDate);

#endif
