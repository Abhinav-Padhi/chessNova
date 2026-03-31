#ifndef SLIDING_H
#define SLIDING_H
#include "types.h"
#include "bitboard.h"

extern U64 nort;
extern U64 noea;
extern U64 sout;

extern U64 rayAttacks[8*64];
void calculate_ray_attacks();

#endif
