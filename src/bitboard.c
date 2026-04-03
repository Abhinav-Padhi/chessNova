#include "defs.h"

/** @brief Shifts a bitboard south (down). */
U64 shiftSouth(U64 b) {return b >> 8;}
/** @brief Shifts a bitboard north (up). */
U64 shiftNorth(U64 b) {return b << 8;}

/** @brief Returns a bitboard with all occupied squares set. */
U64 get_occupied(Board board) {
    U64 occupied = 0ULL;

    for (int piece = wp; piece <= bk; piece++) {
        occupied |= board.bitboards[piece];
    }

    return occupied;
}

/** @brief Returns a bitboard with all empty squares set. */
U64 get_empty(U64 occupied) {
    return ~occupied;
}

/** @brief Shifts a bitboard east (right). */
U64 shiftEast(U64 b)      {return (b << 1) & notAFile;}
/** @brief Shifts a bitboard northeast (up and right). */
U64 shiftNortheast(U64 b) {return (b << 9) & notAFile;}
/** @brief Shifts a bitboard southeast (down and right). */
U64 shiftSoutheast(U64 b) {return (b >> 7) & notAFile;}
/** @brief Shifts a bitboard west (left). */
U64 shiftWest(U64 b)      {return (b >> 1) & notHFile;}
/** @brief Shifts a bitboard southwest (down and left). */
U64 shiftSouthwest(U64 b) {return (b >> 9) & notHFile;}
/** @brief Shifts a bitboard northwest (up and left). */
U64 shiftNorthwest(U64 b) {return (b << 7) & notHFile;}

/** @brief Tests if a bit is set at the given square. */
bool test_bit(U64 b, int square) {
    return (b & (1ULL << square));
}

/** @brief Sets a bit at the given square. */
void set_bit(U64 *b, int square) {
    *b |= (1ULL << square);
}

/** @brief Clears a bit at the given square. */
void clear_bit(U64 *b, int square) {
    *b &= ~(1ULL << square);
}

/** @brief Toggles a bit at the given square. */
void toggle_bit(U64 *b, int square) {
    *b ^= (1ULL << square);
}
