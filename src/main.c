#include "../include/defs.h"

int main() {
    Board board;
    reset_board(&board);
    parse_fen(startFEN, &board);
    print_board(&board);
}
