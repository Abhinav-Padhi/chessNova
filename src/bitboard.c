#include "../include/defs.h"


U64 shiftSouth(U64 b) {return b >> 8;}
U64 shiftNorth(U64 b) {return b << 8;}

U64 get_occupied(Board board) {
    U64 occupied = 0ULL;

    for (int piece = wp; piece <= bk; piece++) {
        occupied |= board.bitboards[piece];
    }

    return occupied;
}

U64 get_empty(U64 occupied) {
    return ~occupied;
}

U64 shiftEast(U64 b)      {return (b << 1) & notAFile;}
U64 shiftNortheast(U64 b) {return (b << 9) & notAFile;}
U64 shiftSoutheast(U64 b) {return (b >> 7) & notAFile;}
U64 shiftWest(U64 b)      {return (b >> 1) & notHFile;}
U64 shiftSouthwest(U64 b) {return (b >> 9) & notHFile;}
U64 shiftNorthwest(U64 b) {return (b << 7) & notHFile;}

bool test_bit(U64 b, int square) {
    return (b & (1ULL << square));
}

void set_bit(U64 *b, int square) {
    *b |= (1ULL << square);
}

void clear_bit(U64 *b, int square) {
    *b |=  (1ULL << square); // set bit
    *b ^=  (1ULL << square); // resets set bit
}

void toggle_bit(U64 *b, int square) {
    *b ^= (1ULL << square);
}
