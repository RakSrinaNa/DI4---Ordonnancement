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
	unsigned int * sol = sequencer_sequenceProductionPack(instance, 3, tasks);
	if(!unit_uintArrayEquals(cSol, sol, 3))
		unit_error("Sequencer 7A: Bad sequence process");
	free(sol);
	
	unsigned int tasks7[] = {0, 1};
	unsigned int cSol2[] = {0, 1};
	sol = sequencer_sequenceProductionPack(instance, 2, tasks7);
	if(!unit_uintArrayEquals(cSol2, sol, 2))
		unit_error("Sequencer 8A: Bad sequence process");
	free(sol);
	
	unsigned int tasks7_2[] = {0};
	unsigned int cSol2_2[] = {0};
	sol = sequencer_sequenceProductionPack(instance, 1, tasks7_2);
	if(!unit_uintArrayEquals(cSol2_2, sol, 1))
		unit_error("Sequencer 8.5A: Bad sequence process");
	free(sol);
	
	sol = sequencer_sequenceProductionPack(instance, 0, NULL);
	if(sol != NULL)
		unit_error("Sequencer 8.75A: Bad sequence process");
	free(sol);
	
	unsigned int tasks8[] = {0, 1, 2, 3};
	unsigned int cSol3[] = {0, 1, 2, 3};
	sol = sequencer_sequenceProductionPack(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol3, sol, 4))
		unit_error("Sequencer 9A: Bad sequence process");
	free(sol);
	
	instance->tasks[3]->machineDurations[0] = 0;
	unsigned int cSol4[] = {3, 0, 1, 2};
	sol = sequencer_sequenceProductionPack(instance, 4, tasks8);
	if(!unit_uintArrayEquals(cSol4, sol, 4))
		unit_error("Sequencer 10A: Bad sequence process");
	free(sol);
	
	instance_destroy(instance);
}

void sequencerUnit_delivery()
{
	Instance * instance = parser_readInstanceFromFile("./unitResources/Instance3.txt");
	
	unsigned int startTime = 100;
	unsigned int tasks9[] = {0, 2};
	unsigned int time = sequencer_deliveryDelay(instance, 2, tasks9, &startTime);
	if(time != 10 || startTime != 110)
		unit_error("Sequencer 1B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 10, startTime, 110);
	
	startTime = 0;
	time = sequencer_deliveryDelay(instance, 2, tasks9, &startTime);
	if(time != 0 || startTime != 10)
		unit_error("Sequencer 2B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 0, startTime, 10);
	
	startTime = 100;
	unsigned int tasks10[] = {0, 1};
	time = sequencer_deliveryDelay(instance, 2, tasks10, &startTime);
	if(time != 9 || startTime != 109)
		unit_error("Sequencer 3B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 9, startTime, 109);
	
	startTime = 100;
	unsigned int tasks11[] = {2, 0};
	time = sequencer_deliveryDelay(instance, 2, tasks11, &startTime);
	if(time != 10 || startTime != 110)
		unit_error("Sequencer 4B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 10, startTime, 110);
	
	startTime = 100;
	unsigned int tasks12[] = {0, 1, 2, 3};
	time = sequencer_deliveryDelay(instance, 4, tasks12, &startTime);
	if(time != 16 || startTime != 114)
		unit_error("Sequencer 5B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 16, startTime, 114);
	
	startTime = 100;
	unsigned int tasks13[] = {3, 0, 1, 2};
	time = sequencer_deliveryDelay(instance, 4, tasks13, &startTime);
	if(time != 25 || startTime != 114)
		unit_error("Sequencer 6B: Bad delivery delay, got %d expected %d, with car back at %d, expected %d", time, 25, startTime, 114);
	
	instance_destroy(instance);
	
	instance = parser_readInstanceFromFile("./unitResources/Instance4.txt");
	
	unsigned int * sol = sequencer_sequenceDeliveriesPack(instance, 0, NULL, 0);
	if(sol != NULL)
		unit_error("Sequencer 7B: Bad delivery sequence");
	
	startTime = 0;
	unsigned int tasks14[] = {0};
	unsigned int cSol1[] = {0};
	sol = sequencer_sequenceDeliveriesPack(instance, 1, tasks14, &startTime);
	if(!unit_uintArrayEquals(cSol1, sol, 1) || startTime != 6)
		unit_error("Sequencer 8B: Bad delivery sequence, with car back at %d, expected %d", startTime, 6);
	free(sol);
	
	startTime = 0;
	unsigned int tasks15[] = {0, 1};
	unsigned int cSol2[] = {0, 1};
	sol = sequencer_sequenceDeliveriesPack(instance, 2, tasks15, &startTime);
	if(!unit_uintArrayEquals(cSol2, sol, 2) || startTime != 8)
		unit_error("Sequencer 9B: Bad delivery sequence, with car back at %d, expected %d", startTime, 8);
	free(sol);
	
	startTime = 100;
	unsigned int cSol3[] = {1, 0};
	sol = sequencer_sequenceDeliveriesPack(instance, 2, tasks15, &startTime);
	if(!unit_uintArrayEquals(cSol3, sol, 2) || startTime != 108)
		unit_error("Sequencer 10B: Bad delivery sequence, with car back at %d, expected %d", startTime, 108);
	free(sol);
	
	startTime = 0;
	unsigned int tasks16[] = {1,2,3};
	unsigned int cSol4[] = {1,2,3};
	sol = sequencer_sequenceDeliveriesPack(instance, 3, tasks16, &startTime);
	if(!unit_uintArrayEquals(cSol4, sol, 3) || startTime != 1005)
		unit_error("Sequencer 11B: Bad delivery sequence, with car back at %d, expected %d", startTime, 1005);
	free(sol);
	
	startTime = 95;
	unsigned int cSol5[] = {1,2,3};
	sol = sequencer_sequenceDeliveriesPack(instance, 3, tasks16, &startTime);
	if(!unit_uintArrayEquals(cSol5, sol, 3) || startTime != 1100)
		unit_error("Sequencer 12B: Bad delivery sequence, with car back at %d, expected %d", startTime, 1100);
	free(sol);
	
	startTime = 0;
	unsigned int tasks17[] = {0};
	unsigned int cSol6[] = {0};
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 1, tasks17, &startTime);
	if(!unit_uintArrayEquals(cSol6, sol, 1) || startTime != 6)
		unit_error("Sequencer 13B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 6);
	free(sol);
	
	startTime = 50;
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 1, tasks17, &startTime);
	if(!unit_uintArrayEquals(cSol6, sol, 1) || startTime != 56)
		unit_error("Sequencer 14B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 56);
	free(sol);
	
	startTime = 0;
	unsigned int tasks18[] = {0, 1};
	unsigned int cSol7[] = {1, 0};
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 2, tasks18, &startTime);
	if(!unit_uintArrayEquals(cSol7, sol, 2) || startTime != 8)
		unit_error("Sequencer 15B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 8);
	free(sol);
	
	startTime = 50;
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 2, tasks18, &startTime);
	if(!unit_uintArrayEquals(cSol7, sol, 2) || startTime != 58)
		unit_error("Sequencer 15B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 58);
	free(sol);
	
	startTime = 0;
	unsigned int tasks19[] = {0, 1, 2};
	unsigned int cSol8[] = {1, 2, 0};
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 3, tasks19, &startTime);
	if(!unit_uintArrayEquals(cSol8, sol, 3) || startTime != 10)
		unit_error("Sequencer 16B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 10);
	free(sol);
	
	startTime = 50;
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 3, tasks19, &startTime);
	if(!unit_uintArrayEquals(cSol8, sol, 3) || startTime != 60)
		unit_error("Sequencer 16B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 60);
	free(sol);
	
	startTime = 0;
	unsigned int tasks20[] = {0, 1, 2, 3};
	unsigned int cSol9[] = {1, 2, 3, 0};
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 4, tasks20, &startTime);
	if(!unit_uintArrayEquals(cSol9, sol, 4) || startTime != 10)
		unit_error("Sequencer 17B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 10);
	free(sol);
	
	startTime = 50;
	sol = sequencer_sequenceDeliveriesNearestNeighbor(instance, 4, tasks20, &startTime);
	if(!unit_uintArrayEquals(cSol9, sol, 4) || startTime != 60)
		unit_error("Sequencer 18B: Bad delivery sequence, nearest neighbor, with car back at %d, expected %d", startTime, 60);
	free(sol);
	
	instance_destroy(instance);
	
	instance = parser_readInstanceFromFile("./unitResources/Instance5.txt");
	
	startTime = 0;
	unsigned int cSol10[] = {0, 1, 3, 2};
	sol = sequencer_sequenceDeliveriesDueDate(instance, 4, tasks20, &startTime);
	if(!unit_uintArrayEquals(cSol10, sol, 4) || startTime != 15)
		unit_error("Sequencer 19B: Bad delivery sequence, due date, with car back at %d, expected %d", startTime, 15);
	free(sol);
	
	startTime = 20;
	sol = sequencer_sequenceDeliveriesDueDate(instance, 4, tasks20, &startTime);
	if(!unit_uintArrayEquals(cSol10, sol, 4) || startTime != 35)
		unit_error("Sequencer 20B: Bad delivery sequence, due date, with car back at %d, expected %d", startTime, 35);
	free(sol);
	
	startTime = 0;
	unsigned int tasks21[] = {0};
	unsigned int cSol11[] = {0};
	sol = sequencer_sequenceDeliveriesDueDate(instance, 1, tasks21, &startTime);
	if(!unit_uintArrayEquals(cSol11, sol, 1) || startTime != 6)
		unit_error("Sequencer 21B: Bad delivery sequence, due date, with car back at %d, expected %d", startTime, 6);
	free(sol);
	
	startTime = 30;
	sol = sequencer_sequenceDeliveriesDueDate(instance, 1, tasks21, &startTime);
	if(!unit_uintArrayEquals(cSol11, sol, 1) || startTime != 36)
		unit_error("Sequencer 22B: Bad delivery sequence, due date, with car back at %d, expected %d", startTime, 36);
	free(sol);
	
	instance_destroy(instance);
}
