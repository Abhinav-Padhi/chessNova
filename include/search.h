#ifndef SEARCH_H
#define SEARCH_H

#include "defs.h"

#define INFINITY 30000
#define MATE_SCORE 29000

/**
 * Performs a search to find the best move in the current position.
 * @param board Pointer to the Board structure.
 * @param depth The depth to search.
 * @return The packed 32-bit best move found.
 */
uint32_t search_best_move(Board *board, int depth);

#endif
