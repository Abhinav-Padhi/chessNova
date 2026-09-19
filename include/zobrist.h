#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "board.h"
#include "types.h"

extern U64 PieceKeys[12][64];
extern U64 SideKey;
extern U64 CastleKeys[16];
extern U64 EnPassantKeys[8];

void init_zobrist(void);
U64 generate_pos_key(const Board* board);

#endif // ZOBRIST_H