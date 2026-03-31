#include "../include/defs.h"

U64 wSinglePushTargets(U64 wpawns, U64 empty) {
   return shiftNorth(wpawns) & empty;
}

U64 wDblPushTargets(U64 wpawns, U64 empty) {
   const U64 rank4 = (0x00000000FF000000);
   U64 singlePushs = wSinglePushTargets(wpawns, empty);
   return shiftNorth(singlePushs) & empty & rank4;
}

U64 bSinglePushTargets(U64 bpawns, U64 empty) {
   return shiftSouth(bpawns) & empty;
}

U64 bDblPushTargets(U64 bpawns, U64 empty) {
   const U64 rank5 = (0x000000FF00000000);
   U64 singlePushs = bSinglePushTargets(bpawns, empty);
   return shiftSouth(singlePushs) & empty & rank5;
}

void push_white_pawn(U64 pawn, Board *board) {
    board->bitboards[wp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[wp] ^= wSinglePushTargets(pawn, get_empty(get_occupied(*board)));
}

void push_dwhite_pawn(U64 pawn, Board *board) {
    board->bitboards[wp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[wp] ^= wDblPushTargets(pawn, get_empty(get_occupied(*board)));
}

void push_black_pawn(U64 pawn, Board *board) {
    board->bitboards[bp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[bp] ^= bSinglePushTargets(pawn, get_empty(get_occupied(*board)));
}

void push_dblack_pawn(U64 pawn, Board *board) {
    board->bitboards[bp] ^= pawn;
    print_bitboard(get_empty(get_occupied(*board)));
    board->bitboards[bp] ^= bDblPushTargets(pawn, get_empty(get_occupied(*board)));
}

U64 wPawnsAble2Push(U64 wpawns, U64 empty) {
   return shiftSouth(empty) & wpawns;
}

U64 wPawnsAble2DblPush(U64 wpawns, U64 empty) {
   const U64 rank4 = (0x00000000FF000000);
   U64 emptyRank3 = shiftSouth(empty & rank4) & empty;
   return wPawnsAble2Push(wpawns, emptyRank3);
}

U64 bPawnsAble2Push(U64 bpawns, U64 empty) {
   return shiftNorth(empty) & bpawns;
}

U64 bPawnsAble2DblPush(U64 bpawns, U64 empty) {
   const U64 rank5 = (0x000000FF00000000);
   U64 emptyRank6 = shiftNorth(empty & rank5) & empty;
   return bPawnsAble2Push(bpawns, emptyRank6);
}


U64 wPawnEastAttacks(U64 wpawns) {return shiftNortheast(wpawns);}
U64 wPawnWestAttacks(U64 wpawns) {return shiftNorthwest(wpawns);}

U64 bPawnEastAttacks(U64 bpawns) {return shiftSoutheast(bpawns);}
U64 bPawnWestAttacks(U64 bpawns) {return shiftSouthwest(bpawns);}

U64 wPawnAnyAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) | wPawnWestAttacks(wpawns);
}

U64 wPawnDblAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) & wPawnWestAttacks(wpawns);
}

U64 wPawnSingleAttacks(U64 wpawns) {
   return wPawnEastAttacks(wpawns) ^ wPawnWestAttacks(wpawns);
}

U64 bPawnAnyAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) | bPawnWestAttacks(bpawns);
}

U64 bPawnDblAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) & bPawnWestAttacks(bpawns);
}

U64 bPawnSingleAttacks(U64 bpawns) {
   return bPawnEastAttacks(bpawns) ^ bPawnWestAttacks(bpawns);
}

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

U64 wPawnsAble2CaptureEast(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnWestAttacks(bpieces);
}

U64 wPawnsAble2CaptureWest(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnEastAttacks(bpieces);
}

U64 wPawnsAble2CaptureAny(U64 wpawns, U64 bpieces) {
   return wpawns & bPawnAnyAttacks(bpieces);
}

U64 bPawnsAble2CaptureEast(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnWestAttacks(wpieces);
}

U64 bPawnsAble2CaptureWest(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnEastAttacks(wpieces);
}

U64 bPawnsAble2CaptureAny(U64 bpawns, U64 wpieces) {
   return bpawns & wPawnAnyAttacks(wpieces);
}

U64 wRam(U64 wpawns, U64 bpawns) {return shiftSouth(bpawns) & wpawns;}
U64 bRam(U64 wpawns, U64 bpawns) {return shiftNorth(wpawns) & bpawns;}

U64 wEastLever(U64 wpawns, U64 bpawns) {
   return wpawns & bPawnWestAttacks(bpawns);
}

U64 wWestLever(U64 wpawns, U64 bpawns) {
   return wpawns & bPawnEastAttacks(bpawns);
}

U64 wAnyLever(U64 wpawns, U64 bpawns) {
   return wEastLever(wpawns, bpawns)
        | wWestLever(wpawns, bpawns);
}

U64 bEastLever(U64 bpawns, U64 wpawns) {
   return bpawns & wPawnWestAttacks(wpawns);
}

U64 bWestLever(U64 bpawns, U64 wpawns) {
   return bpawns & wPawnEastAttacks(wpawns);
}

U64 bAnyLever(U64 bpawns, U64 wpawns) {
   return bEastLever(bpawns, wpawns)
        | bWestLever(bpawns, wpawns);
}

U64 wInnerLever(U64 wpawns, U64 bpawns) {
   const U64 abcFiles =(0x0707070707070707);
   const U64 fghFiles =(0xE0E0E0E0E0E0E0E0);
   return ( wEastLever(wpawns, bpawns) & abcFiles )
        | ( wWestLever(wpawns, bpawns) & fghFiles );
}

U64 wOuterLever(U64 wpawns, U64 bpawns) {
   const U64 bcdFiles =(0x0E0E0E0E0E0E0E0E);
   const U64 efgFiles =(0x7070707070707070);
   return ( wEastLever(wpawns, bpawns) & efgFiles )
        | ( wWestLever(wpawns, bpawns) & bcdFiles );
}

U64 wCenterLever(U64 wpawns, U64 bpawns) {
   const U64 dFile =(0x0808080808080808);
   const U64 eFile =(0x1010101010101010);
   return ( wEastLever(wpawns, bpawns) & dFile )
        | ( wWestLever(wpawns, bpawns) & eFile );
}

U64 bInnerLever(U64 bpawns, U64 wpawns) {
   const U64 abcFiles =(0x0707070707070707);
   const U64 fghFiles =(0xE0E0E0E0E0E0E0E0);
   return ( bEastLever(bpawns, wpawns) & abcFiles )
        | ( bWestLever(bpawns, wpawns) & fghFiles );
}

U64 bOuterLever(U64 bpawns, U64 wpawns) {
   const U64 bcdFiles =(0x0E0E0E0E0E0E0E0E);
   const U64 efgFiles =(0x7070707070707070);
   return ( bEastLever(bpawns, wpawns) & efgFiles )
        | ( bWestLever(bpawns, wpawns) & bcdFiles );
}

U64 bCenterLever(U64 bpawns, U64 wpawns) {
   const U64 dFile =(0x0808080808080808);
   const U64 eFile =(0x1010101010101010);
   return ( bEastLever(bpawns, wpawns) & dFile )
        | ( bWestLever(bpawns, wpawns) & eFile );
}

U64 wPawnDefendedFromWest(U64 wpawns) {
   return wpawns & wPawnEastAttacks(wpawns);
}

U64 wPawnDefendedFromEast(U64 wpawns) {
   return wpawns & wPawnWestAttacks(wpawns);
}

U64 bPawnDefendedFromWest(U64 bpawns) {
   return bpawns & bPawnEastAttacks(bpawns);
}

U64 bPawnDefendedFromEast(U64 bpawns) {
   return bpawns & bPawnWestAttacks(bpawns);
}

U64 wPawnDefendersFromWest(U64 wpawns) {
   return wpawns & bPawnWestAttacks(wpawns);
}

U64 wPawnDefendersFromEast(U64 wpawns) {
   return wpawns & bPawnEastAttacks(wpawns);
}

U64 bPawnDefendersFromWest(U64 bpawns) {
   return bpawns & wPawnWestAttacks(bpawns);
}

U64 bPawnDefendersFromEast(U64 bpawns) {
   return bpawns & wPawnEastAttacks(bpawns);
}

U64 pawnsWithEastNeighbors(U64 pawns) {
   return pawns & shiftWest (pawns);
}

U64 pawnsWithWestNeighbors(U64 pawns) {
   return pawnsWithEastNeighbors(pawns) << 1; // * 2
}

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
