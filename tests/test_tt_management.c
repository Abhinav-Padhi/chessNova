/**
 * @file test_tt_management.c
 * @brief Unit tests for Transposition Table allocation, index masking, probing, and replacement
 * policy.
 */

#include "defs.h"
#include "tt.h"
#include <assert.h>
#include <stdio.h>

static void test_tt_allocation(void) {
    printf("Testing TT allocation and power-of-2 sizing...\n");

    init_tt(16); // 16 MB
    assert(TT.pTable != NULL);
    assert(TT.numEntries > 0);
    // Check that numEntries is a power of 2: (N & (N - 1)) == 0
    assert((TT.numEntries & (TT.numEntries - 1)) == 0);
    printf("  16MB allocated: %u entries\n", TT.numEntries);

    init_tt(64); // 64 MB
    assert(TT.numEntries > 0);
    assert((TT.numEntries & (TT.numEntries - 1)) == 0);
    printf("  64MB allocated: %u entries\n", TT.numEntries);

    free_tt();
    assert(TT.pTable == NULL);
    assert(TT.numEntries == 0);

    printf("  TT allocation test passed!\n\n");
}

static void test_tt_probing_and_replacement(void) {
    printf("Testing TT probe, store, and replacement policy...\n");

    init_tt(16);
    clear_tt();

    U64 key1 = 0x1111222233334444ULL;
    U64 key2 = 0x5555666677778888ULL;

    uint32_t move1 = 0x1234;
    uint32_t move2 = 0x5678;

    int score = 0;
    uint32_t best_move = 0;

    // 1. Probe empty table
    bool hit = probe_tt(key1, 5, -100, 100, 0, &score, &best_move);
    assert(!hit);
    assert(best_move == 0);

    // 2. Store depth 4 entry and probe with depth 5 (should match move, but no cutoff because depth
    // 4 < 5)
    store_tt(key1, move1, 200, 4, TT_EXACT, 0);
    hit = probe_tt(key1, 5, -100, 100, 0, &score, &best_move);
    assert(!hit);               // Depth insufficient for cutoff
    assert(best_move == move1); // Move still returned for move ordering!

    // 3. Probe with depth 4 (exact match cutoff)
    hit = probe_tt(key1, 4, -100, 100, 0, &score, &best_move);
    assert(hit);
    assert(score == 200);
    assert(best_move == move1);

    // 4. Replacement Test: Shallow entry (depth 2) should NOT replace deeper entry (depth 4) of
    // same age We construct a key2 that collides with key1's slot index
    U64 mask = TT.numEntries - 1;
    key2 = (key1 & mask) | 0x8888888800000000ULL; // Same slot index, different key

    store_tt(key2, move2, 50, 2, TT_EXACT, 0); // Shallow depth 2

    // key1 (deeper depth 4) should remain intact!
    hit = probe_tt(key1, 4, -100, 100, 0, &score, &best_move);
    assert(hit);
    assert(score == 200);

    // 5. Age Replacement Test: Advance generation age, now shallower entry (depth 2) WILL overwrite
    // old key1
    increment_tt_age();
    store_tt(key2, move2, 50, 2, TT_EXACT, 0);

    // key1 is replaced by key2
    hit = probe_tt(key1, 4, -100, 100, 0, &score, &best_move);
    assert(!hit);

    hit = probe_tt(key2, 2, -100, 100, 0, &score, &best_move);
    assert(hit);
    assert(score == 50);
    assert(best_move == move2);

    free_tt();
    printf("  TT probing and replacement test passed!\n\n");
}

int main(void) {
    printf("Running Phase 3 TT Management Unit Tests...\n\n");

    test_tt_allocation();
    test_tt_probing_and_replacement();

    printf("All Phase 3 TT Management tests passed successfully!\n");
    return 0;
}