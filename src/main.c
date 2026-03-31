#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    board.bitboards[wp] = 0x003;
    board.bitboards[bp] = 0x100;

    print_bitboard(duo(board.bitboards[wp]));

}
