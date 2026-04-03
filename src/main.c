#include "defs.h"

int main() {
    Board board;
    reset_board(&board);

    board.bitboards[bp] = ((U64)1 << a7) | ((U64)1 << c6);
    board.bitboards[wp] = 1<<a2 | 1<< d2;
    print_bitboard(board.bitboards[wp]);
    print_bitboard(board.bitboards[bp]);
    print_bitboard(closedFiles(board.bitboards[wp],board.bitboards[bp]));
    print_bitboard(openFiles(board.bitboards[wp],board.bitboards[bp]));
    
}
