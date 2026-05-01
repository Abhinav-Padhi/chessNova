#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void test_search(char *fen, int depth, char *description) {
    Board board;
    parse_fen(fen, &board);
    printf("Test: %s\n", description);
    printf("FEN: %s\n", fen);

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = depth;
    info.starttime = get_time_ms();

    uint32_t move = search_best_move(&board, &info);
    printf("Best Move Found: %s\n\n", move_to_string(move));
}

int main() {
    init_magics();
    init_evaluation_masks();

    printf("--- SEARCH TESTS ---\n\n");

    // Mate in 1
    // 4k3/5P2/8/8/8/8/8/4K3 w - - 0 1
    // White to move, f7f8 is mate (if we promote to Q/R)
    // Actually in this FEN, f7f8 is possible.
    test_search("4k3/5P2/8/8/8/8/8/4K3 w - - 0 1", 3, "Mate in 1 (Promotion)");

    // Capture hanging piece
    // 4k3/8/8/8/q7/8/4P3/4K3 w - - 0 1
    // White pawn at e2, Black queen at a4. Pawn can't capture.
    // Let's put queen at d3.
    test_search("4k3/8/8/8/8/3q4/4P3/4K3 w - - 0 1", 3, "Capture hanging Queen");

    // Scholar's Mate (Defend)
    // rnb1kbnr/pppp1ppp/8/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR b KQkq - 0 1
    // It's black's turn. White threatens Qxf7#. Black should defend.
    test_search("rnb1kbnr/pppp1ppp/8/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR b KQkq - 0 1", 3, "Defend Scholar's Mate");

    return 0;
}
