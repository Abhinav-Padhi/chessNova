#include "../include/bitboard.h"


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
