#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"

/* Piece values */
#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#define KING_VALUE 20000

/**
 * Returns a score for the current position from the perspective of the side to move.
 * Positive score is good for the side to move.
 */
int evaluate(const Board *board);

#endif
