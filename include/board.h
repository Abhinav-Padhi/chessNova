#ifndef BOARD_H
#define BOARD_H
#include "types.h"
typedef struct {
    U64 bitboards[12];    // One for each piece type
    U64 occupancies[3];   // black white or both

    int side;
    int enpassant;
    int castle;
} Board;

void print_board(Board *board);
void print_bitboard(U64 bitboard);

#endif
