#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"
#include "board.h"

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

#endif // BITBOARD_H
