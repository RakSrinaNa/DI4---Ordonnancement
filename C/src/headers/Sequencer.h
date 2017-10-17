#ifndef TABOU_SEQUENCER_H
#define TABOU_SEQUENCER_H

#include "Instance.h"

typedef struct _SolutionInfo
{
	unsigned int * productionOrder;
	unsigned int * readyToDeliver;
	unsigned int ** deliveries;
} SolutionInfo;

//TODO: @Schttopup Doc
unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, unsigned int const * tasks);

//TODO: @Schttopup Doc
unsigned int * sequencer_sequenceProduction(Instance * instance, unsigned int taskCount, unsigned int * tasks);

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

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveries(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int * initialDate);

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveriesNearestNeighbor();

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveriesDueDate();

#endif
