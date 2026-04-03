#include "defs.h"

int main() {
    Board board;
    reset_board(&board);

    board.bitboards[bp] = ((U64)1 << a7) | ((U64)1 << c6);

    print_bitboard(board.bitboards[bp]);
    print_bitboard(southFill(board.bitboards[bp]));


}
