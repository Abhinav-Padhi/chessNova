#include "defs.h"

/** @brief Shifts a bitboard south (down).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftSouth(U64 b) {return b >> 8;}
/** @brief Shifts a bitboard north (up).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftNorth(U64 b) {return b << 8;}

/** @brief Returns a bitboard with all occupied squares set.
 * @param board The board to get the occupied squares from.
 * @return A bitboard with all occupied squares set.
 */
U64 get_occupied(Board board) {
    U64 occupied = 0ULL;

    for (int piece = wp; piece <= bk; piece++) {
        occupied |= board.bitboards[piece];
    }

    return occupied;
}

/** @brief Returns a bitboard with all empty squares set.
 * @param occupied The bitboard with all occupied squares set.
 * @return A bitboard with all empty squares set.
 */
U64 get_empty(U64 occupied) {
    return ~occupied;
}

/** @brief Shifts a bitboard east (right).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftEast(U64 b)      {return (b << 1) & notAFile;}
/** @brief Shifts a bitboard northeast (up and right).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftNortheast(U64 b) {return (b << 9) & notAFile;}
/** @brief Shifts a bitboard southeast (down and right).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftSoutheast(U64 b) {return (b >> 7) & notAFile;}
/** @brief Shifts a bitboard west (left).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftWest(U64 b)      {return (b >> 1) & notHFile;}
/** @brief Shifts a bitboard southwest (down and left).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftSouthwest(U64 b) {return (b >> 9) & notHFile;}
/** @brief Shifts a bitboard northwest (up and left).
 * @param b The bitboard to shift.
 * @return The shifted bitboard.
 */
U64 shiftNorthwest(U64 b) {return (b << 7) & notHFile;}

/** @brief Tests if a bit is set at the given square.
 * @param b The bitboard to test.
 * @param square The square to test.
 * @return True if the bit is set, false otherwise.
 */
bool test_bit(U64 b, int square) {
    return (b & (1ULL << square));
}

/** @brief Sets a bit at the given square.
 * @param b The bitboard to set.
 * @param square The square to set.
 */
void set_bit(U64 *b, int square) {
    *b |= (1ULL << square);
}

/** @brief Clears a bit at the given square.
 * @param b The bitboard to clear.
 * @param square The square to clear.
 */
void clear_bit(U64 *b, int square) {
    *b &= ~(1ULL << square);
}

/** @brief Toggles a bit at the given square.
 * @param b The bitboard to toggle.
 * @param square The square to toggle.
 */
void toggle_bit(U64 *b, int square) {
    *b ^= (1ULL << square);
}
/*
U64 attacksTo(U64 occupied, int sq, Board *board) {
   U64 knights, kings, bishopsQueens, rooksQueens;
   knights        = board->bitboards[wn] | board->bitboards[bn];
   kings          = board->bitboards[wk] | board->bitboards[bk];
   bishopsQueens  = board->bitboards[wq] | board->bitboards[bq];
   rooksQueens    = board->bitboards[wr] | board->bitboards[br];

   return (arrwPawnAttacks[sq] & board->bitboards[bp])
        | (arrbPawnAttacks[sq] & board->bitboards[wp])
        | (arrKnightAttacks      [sq] & knights)
        | (arrKingAttacks        [sq] & kings)
        | (bishopAttacks(occupied,sq) & bishopsQueens)
        | (rookAttacks  (occupied,sq) & rooksQueens)
        ;
}
*/

/**
 * @brief BitTable Array
 */
const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

/**
 * @brief Pops the least significant bit set in a 64-bit integer.
 * @param bb The bitboard to pop.
 * @return The index of the least significant bit set.
 */
int PopBit(U64 *bb) {
  U64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

/**
 * @brief Counts the number of bits set in a 64-bit integer.
 * @param b The bitboard to count.
 * @return The number of bits set.
 */
int CountBits(U64 b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}
