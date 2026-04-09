#ifndef SLIDING_H
#define SLIDING_H
#include "types.h"
#include "bitboard.h"

extern U64 rayAttacks[8*64];

extern U64 NorthAttacks[64];
extern U64 NorthEastAttacks[64];
extern U64 NorthWestAttacks[64];
extern U64 SouthAttacks[64];
extern U64 SouthEastAttacks[64];
extern U64 SouthWestAttacks[64];
extern U64 EastAttacks[64];
extern U64 WestAttacks[64];

extern U64 rankAttacks[64];
extern U64 fileAttacks[64];
extern U64 diagonalAttacks[64];
extern U64 antiDiagonalAttacks[64];

extern U64 RookAttacks[64];
extern U64 BishopAttacks[64];
extern U64 QueenAttacks[64];
typedef unsigned char uint8;
extern uint8 FirstRankAttacks[256*8];

void calculate_ray_attacks();
void generate_attacks();
void generate_piece_attacks();
void initFirstRankAttacks();
U64 mask_rook_attacks(int sq);
U64 mask_bishop_attacks(int sq);
U64 mask_queen_attacks(int sq);
extern U64 rook_masks[64];
extern U64 bishop_masks[64];
extern U64 queen_masks[64];

#endif
