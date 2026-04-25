/**
 * @file magic.c
 * @brief Magic Bitboard initialization and attack table generation.
 * 
 * This file contains the logic to initialize the Magic Bitboard lookup tables
 * for sliding pieces (Rooks and Bishops). It populates precomputed tables
 * that allow for O(1) attack square lookups by hashing the current board occupancy.
 */

#include "sliding.h"
#include <stdlib.h>

/** @brief Global magic structures for Rook lookups. */
Magic rook_magics[64];

/** @brief Global magic structures for Bishop lookups. */
Magic bishop_magics[64];

/** @brief Large lookup table containing all possible Rook attack bitboards. */
U64 rook_attacks_table[0x19000];  

/** @brief Large lookup table containing all possible Bishop attack bitboards. */
U64 bishop_attacks_table[0x1480]; 

/**
 * @brief Pre-calculated magic numbers for Bishop attack generation.
 */
const U64 bishop_magics_values[64] = {
    0x40040844404081ULL, 0x2008021041040001ULL, 0x4000608531044202ULL, 0x10000509001410ULL, 0x1000840101006ULL, 0x1500440020082ULL, 0x802001100201ULL, 0x40300022020224ULL,
    0x1200004101ULL, 0x8008040010010020ULL, 0x1004000801201ULL, 0x210080100010001ULL, 0x4100040200801ULL, 0x101001000210001ULL, 0x100040081010ULL, 0x4080004040008ULL,
    0x40100102100ULL, 0x400800200004001ULL, 0x1001000102ULL, 0x400020010100ULL, 0x202020202ULL, 0x12400400800ULL, 0x402002100ULL, 0x404402100ULL,
    0x10020020201ULL, 0x41040201ULL, 0x1020101ULL, 0x20101ULL, 0x10201ULL, 0x201ULL, 0x1ULL, 0x1ULL,
    0x40004ULL, 0x40ULL, 0x200ULL, 0x100ULL, 0x4ULL, 0x10ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL
};

/**
 * @brief Pre-calculated magic numbers for Rook attack generation.
 */
const U64 rook_magics_values[64] = {
    0xa8002c8610280041ULL, 0xa000100080c00081ULL, 0x5402c84106200040ULL, 0x620030060240008ULL, 0x200420040ULL, 0x4104004001ULL, 0x40480801088ULL, 0x40002010011ULL,
    0x1200004101ULL, 0x8008040010010020ULL, 0x1004000801201ULL, 0x210080100010001ULL, 0x4100040200801ULL, 0x101001000210001ULL, 0x100040081010ULL, 0x4080004040008ULL,
    0x20040008ULL, 0x20001002008ULL, 0x10010ULL, 0x20444ULL, 0x10002ULL, 0x11000ULL, 0x40ULL, 0x10ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x8a4a44100ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL,
    0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL, 0x1ULL
};

/* Using your existing masks from sliding.c via extern */
extern U64 mask_rook_attacks(int sq);
extern U64 mask_bishop_attacks(int sq);
extern int rook_mask_bits[64];
extern int bishop_mask_bits[64];
extern U64 set_occupancy(int index, int bits_in_mask, U64 mask);

/**
 * @brief Generates bishop attacks for a given square and occupancy on the fly.
 * @param sq The square of the bishop.
 * @param block The current occupancy bitboard.
 * @return Bitboard of attack squares.
 */
U64 bishop_attacks_on_the_fly(int sq, U64 block) {
    U64 attacks = 0;
    int r, f;
    int tr = sq / 8;
    int tf = sq % 8;
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    return attacks;
}

/**
 * @brief Generates rook attacks for a given square and occupancy on the fly.
 * @param sq The square of the rook.
 * @param block The current occupancy bitboard.
 * @return Bitboard of attack squares.
 */
U64 rook_attacks_on_the_fly(int sq, U64 block) {
    U64 attacks = 0;
    int r, f;
    int tr = sq / 8;
    int tf = sq % 8;
    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) break;
    }
    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) break;
    }
    return attacks;
}

/**
 * @brief Initializes Magic Bitboard attack tables for all squares.
 * 
 * This function iterates through all 64 squares, generating all possible
 * occupancy variations for both Rooks and Bishops, and stores their
 * corresponding attack bitboards in lookup tables using magic hashing.
 */
void init_magics() {
    U64 *rook_ptr = rook_attacks_table;
    U64 *bishop_ptr = bishop_attacks_table;

    for (int sq = 0; sq < 64; sq++) {
        // Init Bishop
        bishop_magics[sq].mask = mask_bishop_attacks(sq);
        bishop_magics[sq].magic = bishop_magics_values[sq];
        bishop_magics[sq].shift = 64 - bishop_mask_bits[sq];
        bishop_magics[sq].attacks = bishop_ptr;
        
        int variations = 1 << bishop_mask_bits[sq];
        for (int i = 0; i < variations; i++) {
            U64 occ = set_occupancy(i, bishop_mask_bits[sq], bishop_magics[sq].mask);
            int magic_idx = (occ * bishop_magics[sq].magic) >> bishop_magics[sq].shift;
            bishop_magics[sq].attacks[magic_idx] = bishop_attacks_on_the_fly(sq, occ);
        }
        bishop_ptr += variations;

        // Init Rook
        rook_magics[sq].mask = mask_rook_attacks(sq);
        rook_magics[sq].magic = rook_magics_values[sq];
        rook_magics[sq].shift = 64 - rook_mask_bits[sq];
        rook_magics[sq].attacks = rook_ptr;
        
        variations = 1 << rook_mask_bits[sq];
        for (int i = 0; i < variations; i++) {
            U64 occ = set_occupancy(i, rook_mask_bits[sq], rook_magics[sq].mask);
            int magic_idx = (occ * rook_magics[sq].magic) >> rook_magics[sq].shift;
            rook_magics[sq].attacks[magic_idx] = rook_attacks_on_the_fly(sq, occ);
        }
        rook_ptr += variations;
    }
}
