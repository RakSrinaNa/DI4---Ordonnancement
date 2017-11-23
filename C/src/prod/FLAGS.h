#ifndef TABOU_FLAGS_H
#define TABOU_FLAGS_H

#define DEV_LOG_SCORE

#define TABU_ITERATIONS 20000
#define TABU_ITERATIONS_NOIMPROVE 10
#define TABU_DIVERSIFICATION 1
#define TABU_LIST_SIZE 5

#define TABU_SEARCH_EBSR 1
#define TABU_SEARCH_EFSR 1
#define TABU_SEARCH_SWAP 1
#define TABU_2OPT 0
#define TABU_FIRST_IMPROVE 1
#define TABU_LOGIC 0
#define TABU_DELTA 0
#define TABU_DELTA_BATCH 4

#define CACHED_SCORE 1 //Use the score cache system.
#define DELIVERY_NEAREST_NEIGHBOR 1 //Sort deliveries using the nearest neighbor, else sort by lowest due date first.

#define SEED 42
#ifndef SEED
	#define SEED time(NULL)
#endif

#endif //TABOU_FLAGS_H
