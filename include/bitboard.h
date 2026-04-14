#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"
#include "board.h"
#include <stdbool.h>


U64 shiftSouth(U64 b);
U64 shiftNorth(U64 b);

U64 shiftEast(U64 b);
U64 shiftNortheast(U64 b);
U64 shiftSoutheast(U64 b);
U64 shiftWest(U64 b);
U64 shiftSouthwest(U64 b);
U64 shiftNorthwest(U64 b);

U64 get_occupied(Board board);
U64 get_empty(U64 occupied);

bool test_bit(U64 b, int square);
void set_bit(U64 *b, int square);
void clear_bit(U64 *b, int square);
void toggle_bit(U64 *b, int square);
int count_bits(U64 bb);
int get_lsb(U64 bb);
int pop_lsb(U64 *bb);

#endif // BITBOARD_H
