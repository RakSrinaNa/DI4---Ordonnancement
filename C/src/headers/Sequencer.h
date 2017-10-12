#ifndef TABOU_SEQUENCER_H
#define TABOU_SEQUENCER_H

#include "Instance.h"

//TODO: @Schttopup Doc
unsigned int sequencer_productionFinalTime(Instance * instance, unsigned int count, unsigned int const * tasks);

//TODO: @Schttopup Doc
unsigned int * sequencer_sequenceProduction(Instance * instance, unsigned int taskCount, unsigned int * tasks);

//TODO: @Schttopup Doc
unsigned int sequencer_deliveryDelay(Instance * instance, unsigned int count, unsigned int * tasks, unsigned int date);

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveries(Instance * instance, unsigned int taskCount, unsigned int * tasks, unsigned int initialDate);

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveriesNearestNeighbor();

//TODO: @Schttopup Doc
//TODO: @MrCraftCod Test
unsigned int * sequencer_sequenceDeliveriesDueDate();

#endif
