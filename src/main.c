#include "../include/defs.h"

int main() {

    Board board;
    reset_board(&board);

    for (int i = 0; i < 64; i++) {
        U64 attacks = knightAttacks(i);
        print_bitboard(attacks);
    }
}
