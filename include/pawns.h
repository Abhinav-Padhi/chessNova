
#ifndef PAWNS_H
#define PAWNS_H

#include "types.h"
#include "bitboard.h"

U64 wSinglePushTargets(U64 wpawns, U64 empty);
U64 bSinglePushTargets(U64 bpawns, U64 empty);
U64 wDblPushTargets(U64 wpawns, U64 empty);
U64 bDblPushTargets(U64 bpawns, U64 empty);
void push_white_pawn(U64 white_pawn, Board *board);

#endif // PAWNS_H
