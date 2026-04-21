#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    for (int i = 0; i < 8; i++) {
        print_bitboard(fileFill(1 << (i + a7)));
    }

    for (int i = 0; i < 8; i++) {
        print_bitboard(arrFiles[i]);
    }
}
