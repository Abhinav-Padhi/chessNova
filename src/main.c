#include "../include/defs.h"

int main() {

    Board board;
    reset_board(&board);
    parse_fen(startFEN, &board);

    printf("Visualizing White Pawns Bitboard:");
    print_bitboard(board.bitboards[wp]);

    printf("Visualizing Black Pawns Bitboard:");
    print_bitboard(board.bitboards[bp]);

    printf("Visualizing Black Knights Bitboard:");
    print_bitboard(board.bitboards[bn]);
    return 0;
}
