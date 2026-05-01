#ifndef SEARCH_H
#define SEARCH_H

#include "defs.h"

#define INFINITY 30000
#define MATE_SCORE 29000

typedef struct {
    U64 starttime;
    U64 stoptime;
    int depth;
    int depthset;
    int timeset;
    int movestogo;
    int infinite;

    U64 nodes;

    int quit;
    int stopped;
} SearchInfo;

/**
 * Performs a search to find the best move in the current position.
 * @param board Pointer to the Board structure.
 * @param info Pointer to the SearchInfo structure.
 * @return The packed 32-bit best move found.
 */
extern uint32_t search_best_move(Board *board, SearchInfo *info);

#endif
