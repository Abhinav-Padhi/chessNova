#ifndef PAWNS_H
#define PAWNS_H

#include "types.h"
#include "bitboard.h"

// push

U64 wSinglePushTargets(U64 wpawns, U64 empty);
U64 bSinglePushTargets(U64 bpawns, U64 empty);
U64 wDblPushTargets(U64 wpawns, U64 empty);
U64 bDblPushTargets(U64 bpawns, U64 empty);
void push_white_pawn(U64 white_pawn, Board *board);
void push_dwhite_pawn(U64 pawn, Board *board);
void push_black_pawn(U64 black_pawn, Board *board);
void push_dblack_pawn(U64 pawn, Board *board);

U64 wPawnsAble2Push(U64 wpawns, U64 empty);
U64 bPawnsAble2Push(U64 bpawns, U64 empty);
U64 wPawnsAble2DblPush(U64 wpawns, U64 empty);
U64 bPawnsAble2DblPush(U64 bpawns, U64 empty);

// Attacks

U64 wPawnEastAttacks(U64 wpawns);
U64 wPawnWestAttacks(U64 wpawns);
U64 bPawnEastAttacks(U64 bpawns);
U64 bPawnWestAttacks(U64 bpawns);

U64 wPawnAnyAttacks(U64 wpawns);
U64 wPawnDblAttacks(U64 wpawns);
U64 wPawnSingleAttacks(U64 wpawns);

U64 bPawnAnyAttacks(U64 bpawns);
U64 bPawnDblAttacks(U64 bpawns);
U64 bPawnSingleAttacks(U64 bpawns);

#endif
