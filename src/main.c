#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    for (int square = 0; square < 64; square++)
        print_bitboard(BishopAttacks[square]);
}
