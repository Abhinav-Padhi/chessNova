#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    print_bitboard(QueenAttacks[b3]);
}
