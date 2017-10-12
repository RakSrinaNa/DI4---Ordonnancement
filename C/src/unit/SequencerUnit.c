#include "headers/SequencerUnit.h"
#include "../headers/Instance.h"
#include "headers/UnitUtils.h"
#include "../headers/Parser.h"
#include "../headers/Sequencer.h"

void sequencerUnit()
{
	sequencerUnit_production();
	sequencerUnit_delivery();
}

void sequencerUnit_production()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance3.txt");
	unsigned int tasks[] = {0, 1, 2};
	unsigned int time = sequencer_productionFinalTime(instance, 3, tasks);
	if(time != 31)
		unit_error("Sequencer 1A: Bad final time, was %d expected %d", time, 31);
	
	unsigned int tasks2[] = {1, 0, 2};
	time = sequencer_productionFinalTime(instance, 3, tasks2);
	if(time != 35)
		unit_error("Sequencer 2A: Bad final time, was %d expected %d", time, 35);
	
	unsigned int tasks3[] = {2, 1, 0};
	time = sequencer_productionFinalTime(instance, 3, tasks3);
	if(time != 51)
		unit_error("Sequencer 3A: Bad final time, was %d expected %d", time, 51);
	
	unsigned int tasks4[] = {0, 2, 1};
	time = sequencer_productionFinalTime(instance, 3, tasks4);
	if(time != 42)
		unit_error("Sequencer 4A: Bad final time, was %d expected %d", time, 42);
	
	unsigned int tasks5[] = {1, 2, 0};
	time = sequencer_productionFinalTime(instance, 3, tasks5);
	if(time != 37)
		unit_error("Sequencer 5A: Bad final time, was %d expected %d", time, 37);
	
	unsigned int tasks6[] = {2, 0, 1};
	time = sequencer_productionFinalTime(instance, 3, tasks6);
	if(time != 46)
		unit_error("Sequencer 6A: Bad final time, was %d expected %d", time, 46);
	
	unsigned int cSol[] = {0, 1, 2};
	unsigned int * sol = sequencer_sequenceProduction(instance, 3, tasks);
	if(!unit_uintArrayEquals(cSol, sol, 3))
		unit_error("Sequencer 7A: Bad sequence process");
	free(sol);
	
	unsigned int tasks7[] = {0, 1};
	unsigned int cSol2[] = {0, 1};
	sol = sequencer_sequenceProduction(instance, 2, tasks7);
	if(!unit_uintArrayEquals(cSol2, sol, 2))
		unit_error("Sequencer 8A: Bad sequence process");
	free(sol);
	
	unsigned int tasks7_2[] = {0};
	unsigned int cSol2_2[] = {0};
	sol = sequencer_sequenceProduction(instance, 1, tasks7_2);
	if(!unit_uintArrayEquals(cSol2_2, sol, 1))
		unit_error("Sequencer 8.5A: Bad sequence process");
	free(sol);
	
	sol = sequencer_sequenceProduction(instance, 0, NULL);
	if(sol != NULL)
		unit_error("Sequencer 8.75A: Bad sequence process");
	free(sol);
	
	unsigned int tasks8[] = {0, 1, 2, 3};
	unsigned int cSol3[] = {0, 1, 2, 3};
	sol = sequencer_sequenceProduction(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol3, sol, 4))
		unit_error("Sequencer 9A: Bad sequence process");
	free(sol);
	
	instance->tasks[3]->machineDurations[0] = 0;
	unsigned int cSol4[] = {3, 0, 1, 2};
	sol = sequencer_sequenceProduction(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol4, sol, 4))
		unit_error("Sequencer 10A: Bad sequence process");
	free(sol);
	
	instance_destroy(instance);
}

void sequencerUnit_delivery()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance3.txt");
	
	unsigned int tasks9[] = {0, 2};
	unsigned int time = sequencer_deliveryDelay(instance, 2, tasks9, 100);
	if(time != 10)
		unit_error("Sequencer 1B: Bad delivery delay, got %d expected %d", time, 10);
	
	time = sequencer_deliveryDelay(instance, 2, tasks9, 0);
	if(time != 0)
		unit_error("Sequencer 2B: Bad delivery delay, got %d expected %d", time, 0);
	
	unsigned int tasks10[] = {0, 1};
	time = sequencer_deliveryDelay(instance, 2, tasks10, 100);
	if(time != 9)
		unit_error("Sequencer 3B: Bad delivery delay, got %d expected %d", time, 9);
	
	unsigned int tasks11[] = {2, 0};
	time = sequencer_deliveryDelay(instance, 2, tasks11, 100);
	if(time != 10)
		unit_error("Sequencer 4B: Bad delivery delay, got %d expected %d", time, 10);
	
	unsigned int tasks12[] = {0, 1, 2, 3};
	time = sequencer_deliveryDelay(instance, 4, tasks12, 100);
	if(time != 16)
		unit_error("Sequencer 5B: Bad delivery delay, got %d expected %d", time, 16);
	
	unsigned int tasks13[] = {3, 0, 1, 2};
	time = sequencer_deliveryDelay(instance, 4, tasks13, 100);
	if(time != 25)
		unit_error("Sequencer 6B: Bad delivery delay, got %d expected %d", time, 25);
	
	instance_destroy(instance);
	
	instance = parser_readInstanceFromFile("./unitResources/Instance4.txt");
	
	unsigned int * sol = sequencer_sequenceDeliveries(instance, 0, NULL, 0);
	if(sol != NULL)
		unit_error("Sequencer 7B: Bad delivery sequence");
	
	unsigned int tasks14[] = {0};
	unsigned int cSol1[] = {0};
	sol = sequencer_sequenceDeliveries(instance, 1, tasks14, 0);
	if(!unit_uintArrayEquals(cSol1, sol, 1))
		unit_error("Sequencer 8B: Bad delivery sequence");
	free(sol);
	
	unsigned int tasks15[] = {0, 1};
	unsigned int cSol2[] = {1, 0};
	sol = sequencer_sequenceDeliveries(instance, 2, tasks15, 0);
	if(!unit_uintArrayEquals(cSol2, sol, 2))
		unit_error("Sequencer 9B: Bad delivery sequence");
	free(sol);
	
	instance_destroy(instance);
}
