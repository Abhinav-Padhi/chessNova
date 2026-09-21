/**
 * @file test_zobrist.c
 * @brief Test suite for Zobrist hashing functionality and incremental updates.
 */

#include "defs.h"
#include "zobrist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Recursive perft helper that verifies Zobrist hashing at every node.
 */
static void verify_zobrist_tree(Board* board, int depth) {
    // 1. Verify that incrementally calculated key matches recomputed key from scratch
    U64 recomputed_key = generate_pos_key(board);
    assert(board->posKey == recomputed_key &&
           "Incremental Zobrist key mismatch with generate_pos_key()!");

    if (depth == 0)
        return;

    MoveList list;
    generate_all_moves(board, &list);

    U64 initial_key = board->posKey;

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;

        if (!make_move(board, move))
            continue;

        // Recurse into child position
        verify_zobrist_tree(board, depth - 1);

        unmake_move(board);

        // 2. Verify unmake_move perfectly restored the original posKey
        assert(board->posKey == initial_key && "unmake_move failed to restore original posKey!");
    }
}

/**
 * Tests null move make/unmake key updates.
 */
static void test_null_move_zobrist(Board* board) {
    U64 initial_key = board->posKey;

    make_null_move(board);
    U64 null_key = board->posKey;

    // Side to move flipped, so key must change
    assert(null_key != initial_key && "Null move failed to change posKey!");

    // Recomputed key should match incremental key after null move
    assert(null_key == generate_pos_key(board) &&
           "Incremental posKey mismatch after make_null_move()!");

    unmake_null_move(board);

    // Key must be perfectly restored
    assert(board->posKey == initial_key && "unmake_null_move failed to restore original posKey!");
}

int main() {
    init_magics();
    init_zobrist();

    Board board;

    // Test positions
    const char* start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const char* kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    const char* pos3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    const char* pos4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

    printf("Running Zobrist hashing verification tests...\n\n");

    // 1. Start position verification
    printf("Testing Start Position...\n");
    parse_fen(start_pos, &board);
    test_null_move_zobrist(&board);
    verify_zobrist_tree(&board, 3);
    printf("  Start Position Zobrist test passed!\n\n");

    // 2. Kiwipete position (heavy on castling and en-passant mechanics)
    printf("Testing Kiwipete Position...\n");
    parse_fen(kiwipete, &board);
    test_null_move_zobrist(&board);
    verify_zobrist_tree(&board, 3);
    printf("  Kiwipete Zobrist test passed!\n\n");

    // 3. Position 3
    printf("Testing Position 3...\n");
    parse_fen(pos3, &board);
    test_null_move_zobrist(&board);
    verify_zobrist_tree(&board, 3);
    printf("  Position 3 Zobrist test passed!\n\n");

    // 4. Position 4
    printf("Testing Position 4...\n");
    parse_fen(pos4, &board);
    test_null_move_zobrist(&board);
    verify_zobrist_tree(&board, 3);
    printf("  Position 4 Zobrist test passed!\n\n");

    printf("All Zobrist hashing tests passed successfully!\n");
    return 0;
}