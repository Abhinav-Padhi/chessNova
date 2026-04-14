#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);

    for (int i = 0; i < 64; i++) {
        printf("%d, ", count_bits(rook_masks[i]));
    }

    printf("\n");
    printf("\n");

    for (int i = 0; i < 64; i++) {
        printf("%d, ", count_bits(bishop_masks[i]));
    }
    printf("\n");
    printf("\n");

    for (int i = 0; i < 64; i++) {
        printf("%d, ", count_bits(queen_masks[i]));
    }
    printf("\n");
}
