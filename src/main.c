#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    for (int i = 0; i < 64; i++) {
        print_bitboard(QueenAttacks[i]);
    }
}
