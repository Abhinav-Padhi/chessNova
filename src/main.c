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


    printf("Visualizing Pawns able to push:");
    print_bitboard(wPawnsAble2Push(board.bitboards[wp], get_empty(get_occupied(board))));
    return 0;
}
