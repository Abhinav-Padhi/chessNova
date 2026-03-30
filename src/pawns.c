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
