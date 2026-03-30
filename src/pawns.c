#include "../include/pawns.h"

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
