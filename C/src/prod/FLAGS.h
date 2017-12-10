#ifndef TABOU_FLAGS_H
#define TABOU_FLAGS_H

//!Logs the score of each iteration.
#define DEV_LOG_SCORE 1

//!Maximum iterations for the Tabu algorithm.
#define TABU_ITERATIONS 0xFFFFFFFF
//!Maximum iterations with no improvement before diversification.
#define TABU_ITERATIONS_NOIMPROVE 15
//!If no improvement was found, try a new, different solution.
#define TABU_DIVERSIFICATION 0
//!Maximum size of the Tabu list.
#define TABU_LIST_SIZE 5

//!Use EBSR for Tabu.
#define TABU_SEARCH_EBSR 0
//!Use EFSR for Tabu.
#define TABU_SEARCH_EFSR 0
//!Use swap for Tabu.
#define TABU_SEARCH_SWAP 1

//!2-OPT
#define TABU_2OPT 0
//!Selects the first better solution instead of the best solution.
#define TABU_FIRST_IMPROVE 0
//!Prevents swapping if already swapped the inverse
#define TABU_LOGIC 0
//!Minimum gap between swaps, EBSRs, EFSRs
#define TABU_DELTA 5
//!Minimum batch gap between swaps, EBSRs, EFSRs
#define TABU_DELTA_BATCH 4
//!EOJZRHt //TODO Wut?
#define TABU_PROPAGATE 0

//!Use the score cache system.
#define CACHED_SCORE 1
//!Sort deliveries using the nearest neighbor, else sort by lowest due date first.
#define DELIVERY_NEAREST_NEIGHBOR 0
//!Random swap, EBSR, EFSR instead of trying all the cases.
#define TABU_RANDOM 0

//!Seed for random number generation. Value can be fixed for tests.
#define SEED 42
#ifndef SEED
	#define SEED time(NULL)
#endif

#endif //TABOU_FLAGS_H
