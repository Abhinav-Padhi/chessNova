#ifndef KNIGHTS_H
#define KNIGHTS_H

#include "types.h"
#include "bitboard.h"

extern U64 arrKnightAttacks[64];
extern U64 sqknightAttacks(int sq);
extern U64 knightFill(U64 knights);
extern U64 knightAttacks(U64 knights);
extern U64 forkTargetSquare(U64 targets);
int knightDistance(int a, int b);
#endif
