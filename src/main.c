#include <stdio.h>
#include <string.h>
#include "../include/board.h"
#include "../include/pawns.h"

int main() {

    Board board;
    memset(board.bitboards, 0, sizeof(board.bitboards));
    board.bitboards[wp] = 0xFF00ULL;

    printf("Visualizing White Pawns Bitboard:");
    print_bitboard(board.bitboards[wp]);

    U64 white_pawn = 0x0100ULL;
    board.bitboards[wp] ^= white_pawn;
    print_bitboard(get_empty(get_occupied(board)));
    board.bitboards[wp] ^= wSinglePushTargets(white_pawn, get_empty(get_occupied(board)));

    printf("Visualizing White Pawns Bitboard:");
    print_bitboard(board.bitboards[wp]);
    return 0;
}
