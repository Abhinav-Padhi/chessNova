/**
 * @file test_tt_search_regression.c
 * @brief Regression and benchmarking tests for TT Search Integration.
 */

#include "defs.h"
#include "board.h"
#include "search.h"
#include "tt.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    const char* fen;
    const char* name;
    int depth;
} TestPosition;

static const TestPosition TEST_POSITIONS[] = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Starting Position", 6},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", "Kiwipete Position", 5},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", "Endgame Position", 7},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", "Pos 4 (Tactical)", 5}
};

static void print_uci_move(uint32_t move) {
    if (move == 0) {
        printf("none");
        return;
    }
    int from = GET_FROM(move);
    int to = GET_TO(move);
    int promoted = GET_PROMOTED(move);

    char promo_char = '\0';
    if (promoted != EMPTY) {
        int type = promoted % 6;
        if (type == 1) promo_char = 'n';
        else if (type == 2) promo_char = 'b';
        else if (type == 3) promo_char = 'r';
        else if (type == 4) promo_char = 'q';
    }

    if (promo_char) {
        printf("%c%c%c%c%c",
               'a' + (from % 8), '1' + (from / 8),
               'a' + (to % 8),   '1' + (to / 8),
               promo_char);
    } else {
        printf("%c%c%c%c",
               'a' + (from % 8), '1' + (from / 8),
               'a' + (to % 8),   '1' + (to / 8));
    }
}

static void run_tt_benchmark(void) {
    printf("==================================================\n");
    printf("   Running TT Search Integration Regression Suite \n");
    printf("==================================================\n\n");

    init_tt(64); // 64 MB Transposition Table

    for (size_t i = 0; i < sizeof(TEST_POSITIONS) / sizeof(TEST_POSITIONS[0]); i++) {
        Board board;
        SearchInfo info;

        // DO NOT memset board to zero — parse_fen handles populating the board,
        // and memset wipes out initialized global/struct pointers!
        memset(&info, 0, sizeof(SearchInfo));

        parse_fen(TEST_POSITIONS[i].fen, &board);

        info.depth = TEST_POSITIONS[i].depth;
        info.starttime = get_time_ms();
        info.timeset = false;

        printf("Position [%s] (Depth %d):\n", TEST_POSITIONS[i].name, TEST_POSITIONS[i].depth);
        uint32_t best_move = search_best_move(&board, &info);
        U64 tt_nodes = info.nodes;

        printf("  -> Best Move: ");
        print_uci_move(best_move);
        printf("\n  -> TT Nodes: %llu\n", (unsigned long long)tt_nodes);

        MoveList list;
        generate_all_moves(&board, &list);
        bool move_found = false;
        for (int m = 0; m < list.count; m++) {
            if (list.moves[m].move == best_move) {
                move_found = true;
                break;
            }
        }
        (void)move_found;
        assert(move_found && "TT returned an illegal root move!");
        printf("  -> Legality Check: PASSED\n\n");
    }

    free_tt();
    printf("All Phase 6 TT regression tests passed successfully!\n");
}

int main(void) {
    init_magics();
    init_evaluation_masks();
    run_tt_benchmark();
    return 0;
}