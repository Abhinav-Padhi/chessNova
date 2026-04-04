/**
 * @file pawn_logic.c
 * @brief Bitboard-based pawn movement and evaluation functions.
 */

#include "defs.h"

/**
 * @brief Calculates the destination squares for white pawns moving one square forward.
 * @param wpawns Bitboard of white pawns.
 * @param empty Bitboard of all empty squares.
 * @return Bitboard of single push target squares.
 */
U64 wSinglePushTargets(U64 wpawns, U64 empty) {
   return shiftNorth(wpawns) & empty;
}

/**
 * @brief Calculates the destination squares for white pawns moving two squares forward.
 * @param wpawns Bitboard of white pawns.
 * @param empty Bitboard of all empty squares.
 * @return Bitboard of double push target squares.
 */
U64 wDblPushTargets(U64 wpawns, U64 empty) {
   const U64 rank4 = (0x00000000FF000000);
   U64 singlePushs = wSinglePushTargets(wpawns, empty);
   return shiftNorth(singlePushs) & empty & rank4;
}

/**
 * @brief Calculates the destination squares for black pawns moving one square forward.
 * @param bpawns Bitboard of black pawns.
 * @param empty Bitboard of all empty squares.
 * @return Bitboard of single push target squares.
 */
U64 bSinglePushTargets(U64 bpawns, U64 empty) {
   return shiftSouth(bpawns) & empty;
}

/**
 * @brief Calculates the destination squares for black pawns moving two squares forward.
 * @param bpawns Bitboard of black pawns.
 * @param empty Bitboard of all empty squares.
 * @return Bitboard of double push target squares.
 */
U64 bDblPushTargets(U64 bpawns, U64 empty) {
   const U64 rank5 = (0x000000FF00000000);
   U64 singlePushs = bSinglePushTargets(bpawns, empty);
   return shiftSouth(singlePushs) & empty & rank5;
}

/**
 * @brief Updates the board state by pushing a white pawn one square.
 * @param pawn Bitboard of the specific pawn to push.
 * @param board Pointer to the board structure.
 */
void push_white_pawn(U64 pawn, Board *board) {
    board->bitboards[wp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[wp] ^= wSinglePushTargets(pawn, get_empty(get_occupied(*board)));
}

/**
 * @brief Updates the board state by pushing a white pawn two squares.
 * @param pawn Bitboard of the specific pawn to push.
 * @param board Pointer to the board structure.
 */
void push_dwhite_pawn(U64 pawn, Board *board) {
    board->bitboards[wp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[wp] ^= wDblPushTargets(pawn, get_empty(get_occupied(*board)));
}

/**
 * @brief Updates the board state by pushing a black pawn one square.
 * @param pawn Bitboard of the specific pawn to push.
 * @param board Pointer to the board structure.
 */
void push_black_pawn(U64 pawn, Board *board) {
    board->bitboards[bp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[bp] ^= bSinglePushTargets(pawn, get_empty(get_occupied(*board)));
}

/**
 * @brief Updates the board state by pushing a black pawn two squares.
 * @param pawn Bitboard of the specific pawn to push.
 * @param board Pointer to the board structure.
 */
void push_dblack_pawn(U64 pawn, Board *board) {
    board->bitboards[bp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[bp] ^= bDblPushTargets(pawn, get_empty(get_occupied(*board)));
}

/**
 * @brief Identifies white pawns that are capable of pushing forward one square.
 * @param wpawns Bitboard of white pawns.
 * @param empty Bitboard of empty squares.
 * @return Bitboard of pawns able to push.
 */
U64 wPawnsAble2Push(U64 wpawns, U64 empty) {
   return shiftSouth(empty) & wpawns;
}

/**
 * @brief Identifies white pawns that are capable of pushing forward two squares.
 * @param wpawns Bitboard of white pawns.
 * @param empty Bitboard of empty squares.
 * @return Bitboard of pawns able to double push.
 */
U64 wPawnsAble2DblPush(U64 wpawns, U64 empty) {
   const U64 rank4 = (0x00000000FF000000);
   U64 emptyRank3 = shiftSouth(empty & rank4) & empty;
   return wPawnsAble2Push(wpawns, emptyRank3);
}

/**
 * @brief Identifies black pawns that are capable of pushing forward one square.
 * @param bpawns Bitboard of black pawns.
 * @param empty Bitboard of empty squares.
 * @return Bitboard of pawns able to push.
 */
U64 bPawnsAble2Push(U64 bpawns, U64 empty) {
   return shiftNorth(empty) & bpawns;
}

/**
 * @brief Identifies black pawns that are capable of pushing forward two squares.
 * @param bpawns Bitboard of black pawns.
 * @param empty Bitboard of empty squares.
 * @return Bitboard of pawns able to double push.
 */
U64 bPawnsAble2DblPush(U64 bpawns, U64 empty) {
   const U64 rank5 = (0x000000FF00000000);
   U64 emptyRank6 = shiftNorth(empty & rank5) & empty;
   return bPawnsAble2Push(bpawns, emptyRank6);
}

/** @brief White pawn attack targets to the North-East. */
U64 wPawnEastAttacks(U64 wpawns) {return shiftNortheast(wpawns);}
/** @brief White pawn attack targets to the North-West. */
U64 wPawnWestAttacks(U64 wpawns) {return shiftNorthwest(wpawns);}

/** @brief Black pawn attack targets to the South-East. */
U64 bPawnEastAttacks(U64 bpawns) {return shiftSoutheast(bpawns);}
/** @brief Black pawn attack targets to the South-West. */
U64 bPawnWestAttacks(U64 bpawns) {return shiftSouthwest(bpawns);}

/**
 * @brief Bitboard of all squares attacked by white pawns.
 */
U64 wPawnAnyAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) | wPawnWestAttacks(wpawns);
}

/**
 * @brief Bitboard of squares attacked by at least two white pawns.
 */
U64 wPawnDblAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) & wPawnWestAttacks(wpawns);
}

/**
 * @brief Bitboard of squares attacked by exactly one white pawn.
 */
U64 wPawnSingleAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) ^ wPawnWestAttacks(wpawns);
}

/**
 * @brief Bitboard of all squares attacked by black pawns.
 */
U64 bPawnAnyAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) | bPawnWestAttacks(bpawns);
}

/**
 * @brief Bitboard of squares attacked by at least two black pawns.
 */
U64 bPawnDblAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) & bPawnWestAttacks(bpawns);
}

/**
 * @brief Bitboard of squares attacked by exactly one black pawn.
 */
U64 bPawnSingleAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) ^ bPawnWestAttacks(bpawns);
}

/**
 * @brief Calculates safe squares for white pawns based on black pawn control.
 */
U64 wSafePawnSquares(U64 wpawns, U64 bpawns) {
   U64 wPawnEA =  wPawnEastAttacks(wpawns);
   U64 wPawnWA =  wPawnWestAttacks(wpawns);
   U64 bPawnEA =  bPawnEastAttacks(bpawns);
   U64 bPawnWA =  bPawnWestAttacks(bpawns);
   U64 wPawnDblAttacks  =  wPawnEA &  wPawnWA;
   U64 wPawnOddAttacks  =  wPawnEA ^  wPawnWA;
   U64 bPawnDblAttacks  =  bPawnEA &  bPawnWA;
   U64 bPawnAnyAttacks  =  bPawnEA |  bPawnWA;
   return wPawnDblAttacks |~bPawnAnyAttacks | (wPawnOddAttacks &~bPawnDblAttacks);
}

/**
 * @brief Calculates safe squares for black pawns based on white pawn control.
 */
U64 bSafePawnSquares(U64 bpawns, U64 wpawns) {
   U64 bPawnEA =  bPawnEastAttacks(bpawns);
   U64 bPawnWA =  bPawnWestAttacks(bpawns);
   U64 wPawnEA =  wPawnEastAttacks(wpawns);
   U64 wPawnWA =  wPawnWestAttacks(wpawns);
   U64 bPawnDblAttacks  =  bPawnEA &  bPawnWA;
   U64 bPawnOddAttacks  =  bPawnEA ^  bPawnWA;
   U64 wPawnDblAttacks  =  wPawnEA &  wPawnWA;
   U64 wPawnAnyAttacks  =  wPawnEA |  wPawnWA;
   return bPawnDblAttacks |~wPawnAnyAttacks | (bPawnOddAttacks &~wPawnDblAttacks);
}

/** @brief White pawns that can capture towards the East. */
U64 wPawnsAble2CaptureEast(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnWestAttacks(bpieces);
}

/** @brief White pawns that can capture towards the West. */
U64 wPawnsAble2CaptureWest(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnEastAttacks(bpieces);
}

/** @brief White pawns that can capture in any direction. */
U64 wPawnsAble2CaptureAny(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnAnyAttacks(bpieces);
}

/** @brief Black pawns that can capture towards the East. */
U64 bPawnsAble2CaptureEast(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnWestAttacks(wpieces);
}

/** @brief Black pawns that can capture towards the West. */
U64 bPawnsAble2CaptureWest(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnEastAttacks(wpieces);
}

/** @brief Black pawns that can capture in any direction. */
U64 bPawnsAble2CaptureAny(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnAnyAttacks(wpieces);
}

/** @brief White pawns blocked by a black pawn directly in front (Rams). */
U64 wRam(U64 wpawns, U64 bpawns) {return shiftSouth(bpawns) & wpawns;}
/** @brief Black pawns blocked by a white pawn directly in front (Rams). */
U64 bRam(U64 wpawns, U64 bpawns) {return shiftNorth(wpawns) & bpawns;}

/** @brief White pawns that can capture a black pawn to the East (Lever). */
U64 wEastLever(U64 wpawns, U64 bpawns) {
   return wpawns & bPawnWestAttacks(bpawns);
}

/** @brief White pawns that can capture a black pawn to the West (Lever). */
U64 wWestLever(U64 wpawns, U64 bpawns) {
   return wpawns & bPawnEastAttacks(bpawns);
}

/** @brief White pawns that can capture a black pawn in either direction. */
U64 wAnyLever(U64 wpawns, U64 bpawns) {
   return wEastLever(wpawns, bpawns)
        | wWestLever(wpawns, bpawns);
}

/** @brief Black pawns that can capture a white pawn to the East (Lever). */
U64 bEastLever(U64 bpawns, U64 wpawns) {
   return bpawns & wPawnWestAttacks(wpawns);
}

/** @brief Black pawns that can capture a white pawn to the West (Lever). */
U64 bWestLever(U64 bpawns, U64 wpawns) {
   return bpawns & wPawnEastAttacks(wpawns);
}

/** @brief Black pawns that can capture a white pawn in either direction. */
U64 bAnyLever(U64 bpawns, U64 wpawns) {
   return bEastLever(bpawns, wpawns)
        | bWestLever(bpawns, wpawns);
}

/** @brief White levers on the inner files (a-c and f-h). */
U64 wInnerLever(U64 wpawns, U64 bpawns) {
   const U64 abcFiles =(0x0707070707070707);
   const U64 fghFiles =(0xE0E0E0E0E0E0E0E0);
   return ( wEastLever(wpawns, bpawns) & abcFiles )
        | ( wWestLever(wpawns, bpawns) & fghFiles );
}

/** @brief White levers on the outer files (b-d and e-g). */
U64 wOuterLever(U64 wpawns, U64 bpawns) {
   const U64 bcdFiles =(0x0E0E0E0E0E0E0E0E);
   const U64 efgFiles =(0x7070707070707070);
   return ( wEastLever(wpawns, bpawns) & efgFiles )
        | ( wWestLever(wpawns, bpawns) & bcdFiles );
}

/** @brief White levers on the center files (d and e). */
U64 wCenterLever(U64 wpawns, U64 bpawns) {
   const U64 dFile =(0x0808080808080808);
   const U64 eFile =(0x1010101010101010);
   return ( wEastLever(wpawns, bpawns) & dFile )
        | ( wWestLever(wpawns, bpawns) & eFile );
}

/** @brief Black levers on the inner files (a-c and f-h). */
U64 bInnerLever(U64 bpawns, U64 wpawns) {
   const U64 abcFiles =(0x0707070707070707);
   const U64 fghFiles =(0xE0E0E0E0E0E0E0E0);
   return ( bEastLever(bpawns, wpawns) & abcFiles )
        | ( bWestLever(bpawns, wpawns) & fghFiles );
}

/** @brief Black levers on the outer files (b-d and e-g). */
U64 bOuterLever(U64 bpawns, U64 wpawns) {
   const U64 bcdFiles =(0x0E0E0E0E0E0E0E0E);
   const U64 efgFiles =(0x7070707070707070);
   return ( bEastLever(bpawns, wpawns) & efgFiles )
        | ( bWestLever(bpawns, wpawns) & bcdFiles );
}

/** @brief Black levers on the center files (d and e). */
U64 bCenterLever(U64 bpawns, U64 wpawns) {
   const U64 dFile =(0x0808080808080808);
   const U64 eFile =(0x1010101010101010);
   return ( bEastLever(bpawns, wpawns) & dFile )
        | ( bWestLever(bpawns, wpawns) & eFile );
}

/** @brief White pawns defended by another white pawn from the West. */
U64 wPawnDefendedFromWest(U64 wpawns) {
   return wpawns & wPawnEastAttacks(wpawns);
}

/** @brief White pawns defended by another white pawn from the East. */
U64 wPawnDefendedFromEast(U64 wpawns) {
   return wpawns & wPawnWestAttacks(wpawns);
}

/** @brief Black pawns defended by another black pawn from the West. */
U64 bPawnDefendedFromWest(U64 bpawns) {
   return bpawns & bPawnEastAttacks(bpawns);
}

/** @brief Black pawns defended by another black pawn from the East. */
U64 bPawnDefendedFromEast(U64 bpawns) {
   return bpawns & bPawnWestAttacks(bpawns);
}

/** @brief White pawns that are currently defending a pawn to their East. */
U64 wPawnDefendersFromWest(U64 wpawns) {
   return wpawns & bPawnWestAttacks(wpawns);
}

/** @brief White pawns that are currently defending a pawn to their West. */
U64 wPawnDefendersFromEast(U64 wpawns) {
   return wpawns & bPawnEastAttacks(wpawns);
}

/** @brief Black pawns that are currently defending a pawn to their East. */
U64 bPawnDefendersFromWest(U64 bpawns) {
   return bpawns & wPawnWestAttacks(bpawns);
}

/** @brief Black pawns that are currently defending a pawn to their West. */
U64 bPawnDefendersFromEast(U64 bpawns) {
   return bpawns & wPawnEastAttacks(bpawns);
}

/** @brief Returns pawns that have at least one neighbor on the file to the East. */
U64 pawnsWithEastNeighbors(U64 pawns) {
   return pawns & shiftWest (pawns);
}

/** @brief Returns pawns that have at least one neighbor on the file to the West. */
U64 pawnsWithWestNeighbors(U64 pawns) {
   return pawnsWithEastNeighbors(pawns) << 1; // * 2
}

/**
 * @brief Identifies pawn duos (two pawns of the same color side-by-side on the same rank).
 * @param pawns Bitboard of pawns.
 * @return Bitboard of pawns forming a duo.
 */
U64 duo (U64 pawns) {
   U64 withWestNeighbors = pawnsWithWestNeighbors(pawns);
   U64 withEastNeighbors = withWestNeighbors >> 1;

   U64 withOneExclusiveNeighbor  = withWestNeighbors ^ withEastNeighbors;
   U64 withExclusiveWestNeighbor = withWestNeighbors & withOneExclusiveNeighbor;
   U64 withExclusiveEastNeighbor = withEastNeighbors & withOneExclusiveNeighbor;

   U64 duoWestOne = withExclusiveEastNeighbor & (withExclusiveWestNeighbor >> 1);
   U64 duoEastOne = duoWestOne << 1;
   return duoWestOne | duoEastOne;
}

/**
 * @brief Fills pawn bitboard towards the North. Used for evaluation spans.
 * @param wpawns The white pawn bitboard.
 * @return The bitboard after filling towards the North.
 */
U64 northFill(U64 wpawns) {
   wpawns |= shiftNorth(wpawns);
   wpawns |= shiftNorth(shiftNorth(wpawns));
   wpawns |= shiftNorth(shiftNorth(shiftNorth(wpawns)));
   return wpawns;
}

/**
 * @brief Fills pawn bitboard towards the South. Used for evaluation spans.
 * @param bpawns The black pawn bitboard.
 * @return The bitboard after filling towards the South.
 */
U64 southFill(U64 bpawns) {
   bpawns |= shiftSouth(bpawns);
   bpawns |= shiftSouth(shiftSouth(bpawns));
   bpawns |= shiftSouth(shiftSouth(shiftSouth(bpawns)));
   return bpawns;
}

/** @brief Front fill for white pawns.
 * @param wp The white pawn bitboard.
 * @return The bitboard after filling towards the North.
 */
U64 wFrontFill(U64 wp) {return northFill(wp);}
/** @brief Front fill for black pawns.
 * @param bp The black pawn bitboard.
 * @return The bitboard after filling towards the South.
 */
U64 bFrontFill(U64 bp) {return southFill(bp);}

<<<<<<< HEAD
/** @brief Fills the entire file if it has atleast one pawn */
U64 fileFill(U64 pawns)
{
   return northFill(pawns) | southFill(pawns);
}

/** @brief Fills the entire file if it has atleast one white and atleast one black pawn. */
U64 closedFiles(U64 wpawns,U64 bpawns)
{
   return fileFill(wpawns) & fileFill(bpawns);
}

/** @brief Fills the entire file if it has neither a  white nor a black pawn. */
U64 openFiles(U64 wpawns, U64 bpawns) {
   return ~fileFill(wpawns) & ~fileFill(bpawns);
}
=======
/** @brief Array of white pawn attacks. */
U64 arrwPawnAttacks[64] = {
    512, 1280, 2560, 5120, 10240, 20480, 40960, 16384, 131072, 327680, 655360, 1310720, 2621440, 5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320, 671088640, 1342177280, 2684354560, 1073741824, 8589934592, 21474836480, 42949672960, 85899345920, 171798691840, 343597383680, 687194767360, 274877906944, 2199023255552, 5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080, 175921860444160, 70368744177664, 562949953421312, 1407374883553280, 2814749767106560, 5629499534213120, 11258999068426240, 22517998136852480, 45035996273704960, 18014398509481984, 144115188075855872, 360287970189639680, 720575940379279360, 1441151880758558720, 2882303761517117440, 5764607523034234880, 11529215046068469760, 4611686018427387904, 0, 0, 0, 0, 0, 0, 0, 0
}

/** @brief Array of black pawn attacks. */
U64 arrbPawnAttacks[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 10, 20, 40, 80, 160, 64, 512, 1280, 2560, 5120, 10240, 20480, 40960, 16384, 131072, 327680, 655360, 1310720, 2621440, 5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320, 671088640, 1342177280, 2684354560, 1073741824, 8589934592, 21474836480, 42949672960, 85899345920, 171798691840, 343597383680, 687194767360, 274877906944, 2199023255552, 5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080, 175921860444160, 70368744177664, 562949953421312, 1407374883553280, 2814749767106560, 5629499534213120, 11258999068426240, 22517998136852480, 45035996273704960, 18014398509481984
}
>>>>>>> 4c59d4f (ver 0.2.14)
