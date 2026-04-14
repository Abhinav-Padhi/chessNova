#ifndef BOARD_H
#define BOARD_H
#include "types.h"
typedef struct {
	int pieces[64];
    U64 bitboards[12];    // One for each piece type
    U64 occupancies[3];   // black white or both

    int side;
    int enpassant;
    int castle;
    int fiftyMove;
    int ply;
    int hisply;

    U64 posKey;
} Board;

void print_board(Board *board);
void print_bitboard(U64 bitboard);
void reset_board(Board *board);
void parse_fen(const char *fen, Board *board);

#endif
