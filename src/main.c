#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "sliding.h"
#include "movegen.h"
#include "polyglot.h"

int main() {
    init_magics();
    uci_loop();
    /*
    init_magics();
    init_evaluation_masks();

    Board board;
    parse_fen(startFEN, &board);

    uint64_t hash = polyglot_hash(&board);
    printf("Size: %zu\n", sizeof(polyglot_random) / sizeof(uint64_t));
    printf("FEN: %s\n", startFEN);
    printf("Calculated Polyglot Hash: 0x%016lx\n", hash);
    printf("Expected Polyglot Hash:   0x463b96181691fc9c\n");
    */

    return 0;
}
