#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "sliding.h"
#include "movegen.h"

void run_perft(char *fen, int depth) {
    Board board;
    parse_fen(fen, &board);
    printf("\nPerft Test: %s\n", fen);

    for (int d = 1; d <= depth; d++) {
        clock_t start = clock();
        long long nodes = perft_test(&board, d);
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Depth %d: %lld nodes (%.2f s)\n", d, nodes, time_spent);
    }
}

int main() {
    init_magics();
    init_evaluation_masks();
    run_perft(startFEN, 7);
    run_perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4);

    return 0;
}
