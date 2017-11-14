#ifndef TABOU_SEQUENCER_H
#define TABOU_SEQUENCER_H

#include "Instance.h"
#include "SolutionInfo.h"

/**
 * Computes the total production time.
 *
 * @param instance The instance. Not null.
 * @param count The number of tasks.
 * @param tasks A table of the tasks.
 * @param machineEndTime The times when the machines are ready.
 * @return The total delay to produce the sequence.
 */
unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, task_t const * tasks, unsigned int * machineEndTime);

/**
 * Orders the production in the given list.
 *
 * @param instance The instance. Not null.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @return An ordered sequence of deliveries.
 * @remark The user is in charge of freeing the returned value.
 */
task_t * sequencer_sequenceProductionPack(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * machineReady);

/**
 * Computes the total delay of this sequence.
 *
 * @param instance The instance. Not null.
 * @param count The number of tasks.
 * @param tasks A table of the tasks.
 * @param date A pointer to the departure time. At the end, contains the arrival time of the truck. Not null.
 * @return The total delay of the sequence.
 */
unsigned int sequencer_deliveryDelay(Instance * instance, unsigned int count, task_t * tasks, unsigned int * date);

/**
 * Orders the deliveries in the given list.
 *
 * @param instance The instance. Not null.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time. Not null.
 * @param diversification True if we want to generate a really different solution, False otherwise.
 * @return An ordered sequence of deliveries.
 * @remark The user is in charge of freeing the returned value.
 */
task_t * sequencer_sequenceDeliveriesPack(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * initialDate, Bool diversification);

/**
 * Orders the deliveries in the given list by nearest neighbor.
 *
 * @param instance The instance. Not null.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time of the truck. Not null.
 * @param diversification True if we want to generate a really different solution, False otherwise.
 * @return An ordered sequence of deliveries.
 * @remark The user is in charge of freeing the returned value.
 */
task_t * sequencer_sequenceDeliveriesNearestNeighbor(Instance * instance, unsigned int taskCount, task_t * tasks, unsigned int * initialDate, Bool diversification);

/**
 * Orders the deliveries in the given list by due date.
 *
 * @param instance The instance. Not null.
 * @param taskCount The number of tasks.
 * @param tasks The list of the tasks.
 * @param initialDate A pointer to the departure time. At the end, contains the arrival time. Not null.
 * @param diversification True if we want to generate a really different solution, False otherwise.
 * @return An ordered sequence of deliveries.
 * @remark The user is in charge of freeing the returned value.
 */
task_t * sequencer_sequenceDeliveriesDueDate(Instance * instance, unsigned int taskCount, const task_t * tasks, unsigned int * initialDate, Bool diversification);

#endif
