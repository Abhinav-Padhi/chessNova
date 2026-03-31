#include "../include/defs.h"

int main() {

    Board board;
    reset_board(&board);
    calculate_ray_attacks();

    for (int i = 0; i < 64; i++) {
        print_bitboard(rayAttacks[i][SoEa]);
    }
}
