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

U64 bDoublePushTargets(U64 bpawns, U64 empty) {
   const U64 rank5 = (0x000000FF00000000);
   U64 singlePushs = bSinglePushTargets(bpawns, empty);
   return shiftSouth(singlePushs) & empty & rank5;
}
