#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/pawns.h"

int main() {

    Board board;
    memset(board.bitboards, 0, sizeof(board.bitboards));
    board.bitboards[wp] = 0xFF00ULL;
    board.bitboards[bp] = 0x10000ULL;

    printf("Visualizing White Pawns Bitboard:");
    print_bitboard(board.bitboards[wp]);

    printf("Visualizing Black Pawns Bitboard:");
    print_bitboard(board.bitboards[bp]);

    printf("Visualizing Black Pawns capture squares:");
    print_bitboard(bPawnsAble2CaptureAny(board.bitboards[bp], board.bitboards[wp]));
    return 0;
}
