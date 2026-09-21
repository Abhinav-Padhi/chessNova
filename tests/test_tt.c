/**
 * @file test_tt.c
 * @brief Unit tests for Transposition Table entry structure and mate score normalization.
 */

#include "defs.h"
#include "tt.h"
#include <assert.h>
#include <stdio.h>

static void test_tt_entry_structure(void) {
    printf("Testing TTEntry structure size and layout...\n");

    // TTEntry should be compact (16 bytes)
    printf("  sizeof(TTEntry): %zu bytes\n", sizeof(TTEntry));
    assert(sizeof(TTEntry) == 16 && "TTEntry size should be exactly 16 bytes!");

    TTEntry entry;
    entry.key = 0x123456789ABCDEF0ULL;
    entry.move = 0x00012345;
    entry.score = 350;
    entry.depth = 6;
    entry.flag = TT_EXACT;
    entry.age = 1;

    assert(entry.key == 0x123456789ABCDEF0ULL);
    assert(entry.move == 0x00012345);
    assert(entry.score == 350);
    assert(entry.depth == 6);
    assert(entry.flag == TT_EXACT);
    assert(entry.age == 1);

    printf("  TTEntry layout test passed!\n\n");
}

static void test_mate_score_conversion(void) {
    printf("Testing mate score TT normalization...\n");

    int ply = 5;

    // Standard static evaluations should remain unchanged
    int eval = 150;
    int16_t stored_eval = score_to_tt(eval, ply);
    int probed_eval = score_from_tt(stored_eval, ply);
    assert(stored_eval == 150);
    assert(probed_eval == 150);

    // Winning Mate-in-3 found at ply 5
    int mate_in_3 = MATE_SCORE - 3; // +29997
    int16_t stored_mate = score_to_tt(mate_in_3, ply);
    
    // Key check: Stored score MUST be stored relative to root (MATE_SCORE - 3 + 5 = MATE_SCORE + 2)
    assert(stored_mate == MATE_SCORE + 2);

    // When retrieved at the SAME ply (ply 5), we get back exact mate-in-3 score relative to ply 5
    int probed_mate_same_ply = score_from_tt(stored_mate, ply);
    assert(probed_mate_same_ply == mate_in_3);

    // When probed higher up the tree (e.g. ply 1), it adjusts correctly to mate-in-1 relative to ply 1
    int probed_mate_diff_ply = score_from_tt(stored_mate, 1);
    assert(probed_mate_diff_ply == MATE_SCORE - 1);

    // Losing Mate test (Losing in 4 ply found at ply 3)
    int losing_mate = -MATE_SCORE + 4;
    int16_t stored_losing = score_to_tt(losing_mate, 3);
    assert(stored_losing == -MATE_SCORE + 1);

    int probed_losing_same_ply = score_from_tt(stored_losing, 3);
    assert(probed_losing_same_ply == losing_mate);

    printf("  Mate score normalization test passed!\n\n");
}

int main(void) {
    printf("Running Phase 2 TT Structure Unit Tests...\n\n");

    test_tt_entry_structure();
    test_mate_score_conversion();

    printf("All Phase 2 TT structure tests passed successfully!\n");
    return 0;
}