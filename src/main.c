#include "../include/defs.h"

int main() {
    Board board;
    board.bitboards[bp] = ((U64)1 << a7) | ((U64)1 << c6);
    board.bitboards[wp] = ((U64)1 << a4) | ((U64)1 << e2);
    print_bitboard(board.bitboards[bp]);
    print_bitboard(westAttackFileFill(board.bitboards[bp]));

    print_bitboard(board.bitboards[wp]);
    print_bitboard(eastAttackFileFill(board.bitboards[wp]));


    /*reset_board(&board);
    parse_fen(startFEN, &board);
    
    print_board(&board);
    */
    reset_board(&board);
    parse_fen(startFEN, &board);
    print_board(&board);
}
