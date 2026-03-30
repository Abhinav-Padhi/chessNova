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

    push_white_pawn(white_pawn, &board);

    printf("Visualizing White Pawns Bitboard:");
    print_bitboard(board.bitboards[wp]);
    return 0;
}
