#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"

/* Piece values - Middlegame */
#define MG_PAWN_VALUE 82
#define MG_KNIGHT_VALUE 337
#define MG_BISHOP_VALUE 365
#define MG_ROOK_VALUE 477
#define MG_QUEEN_VALUE 1025
#define MG_KING_VALUE 0

/* Piece values - Endgame */
#define EG_PAWN_VALUE 94
#define EG_KNIGHT_VALUE 281
#define EG_BISHOP_VALUE 297
#define EG_ROOK_VALUE 512
#define EG_QUEEN_VALUE 936
#define EG_KING_VALUE 0

#define FLIP(sq) ((sq) ^ 56)

/**
 * Returns a score for the current position from the perspective of the side to move.
 * Positive score is good for the side to move.
 */
int evaluate(const Board *board);

#endif
