/**
 * @file test_book_search.c
 * @brief Integration tests for opening book lookup in the search pipeline.
 */

#include "defs.h"
#include "board.h"
#include "search.h"
#include "polyglot.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define NULL_DEV "NUL"
#else
#include <unistd.h>
#define NULL_DEV "/dev/null"
#endif

static char original_book_path[256];

static void locate_book(void) {
    FILE* f_test = fopen("book.bin", "rb");
    if (f_test) {
        snprintf(book_file_path, sizeof(book_file_path), "book.bin");
        fclose(f_test);
    } else {
        f_test = fopen("../book.bin", "rb");
        if (f_test) {
            snprintf(book_file_path, sizeof(book_file_path), "../book.bin");
            fclose(f_test);
        } else {
            f_test = fopen("../../book.bin", "rb");
            if (f_test) {
                snprintf(book_file_path, sizeof(book_file_path), "../../book.bin");
                fclose(f_test);
            } else {
                snprintf(book_file_path, sizeof(book_file_path), "book.bin");
            }
        }
    }
    snprintf(original_book_path, sizeof(original_book_path), "%s", book_file_path);
}

/**
 * Test 1: Starting Position
 * Verify that the engine plays a move from the opening book instead of immediately searching.
 * Expected: Move returned exists in book.bin, search is skipped (0 nodes searched).
 */
static void test_starting_position(void) {
    printf("Running Test 1: Starting Position...\n");
    Board board;
    parse_fen(startFEN, &board);

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = 6;
    info.starttime = get_time_ms();

    use_book = true;
    uint32_t move = search_best_move(&board, &info);

    assert(move != 0 && "Expected valid move from opening book for start position");
    assert(info.nodes == 0 && "Expected search to be skipped (0 nodes) when book move is found");

    char* move_str = move_to_string(move);
    printf("  -> Starting position book move: %s (nodes: %llu)\n", move_str, (unsigned long long)info.nodes);
    assert((strcmp(move_str, "e2e4") == 0 || strcmp(move_str, "d2d4") == 0) &&
           "Expected starting move to be e2e4 or d2d4 from book.bin");
    printf("  ✓ Test 1 Passed!\n\n");
}

/**
 * Test 2: Known Book Line
 * Play several opening moves that remain inside the book.
 * Expected: Engine continues selecting valid book moves with search skipped.
 */
static void test_known_book_line(void) {
    printf("Running Test 2: Known Book Line...\n");
    Board board;
    parse_fen(startFEN, &board);

    use_book = true;

    // 1. Play 1. e4
    uint32_t m1 = parse_move("e2e4", &board);
    assert(m1 != 0);
    make_move(&board, m1);

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = 6;
    info.starttime = get_time_ms();

    uint32_t reply = search_best_move(&board, &info);
    assert(reply != 0 && "Expected valid book response to 1. e4");
    assert(info.nodes == 0 && "Expected search to be skipped for book move");
    char* r_str = move_to_string(reply);
    printf("  -> Response to 1. e4: %s (nodes: %llu)\n", r_str, (unsigned long long)info.nodes);
    assert((strcmp(r_str, "c7c5") == 0 || strcmp(r_str, "c7c6") == 0) &&
           "Expected response to 1. e4 to be c7c5 or c7c6 from book.bin");

    // Play 1... c5
    uint32_t m2 = parse_move("c7c5", &board);
    if (m2 != 0 && make_move(&board, m2)) {
        memset(&info, 0, sizeof(SearchInfo));
        info.depth = 6;
        info.starttime = get_time_ms();

        uint32_t w_reply = search_best_move(&board, &info);
        assert(w_reply != 0 && "Expected valid book response to 1... c5");
        assert(info.nodes == 0 && "Expected search skipped for book move");
        printf("  -> Response to 1... c5: %s (nodes: %llu)\n", move_to_string(w_reply), (unsigned long long)info.nodes);
        assert(strcmp(move_to_string(w_reply), "g1f3") == 0 && "Expected 2. Nf3 (g1f3) from book");
    }

    printf("  ✓ Test 2 Passed!\n\n");
}

/**
 * Test 3: Out-of-Book Position
 * Reach a position not present in the opening book.
 * Expected: Engine falls back to normal search, best move produced by search algorithm.
 */
static void test_out_of_book_position(void) {
    printf("Running Test 3: Out-of-Book Position...\n");
    Board board;
    // Mate in 1 tactical position (definitely not in book.bin)
    parse_fen("4k3/5P2/8/8/8/8/8/4K3 w - - 0 1", &board);

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = 3;
    info.starttime = get_time_ms();

    use_book = true;
    uint32_t move = search_best_move(&board, &info);

    assert(move != 0 && "Expected search to return a move");
    assert(info.nodes > 0 && "Expected search to run (nodes > 0) when out of book");
    printf("  -> Out-of-book move: %s (nodes: %llu)\n", move_to_string(move), (unsigned long long)info.nodes);
    printf("  ✓ Test 3 Passed!\n\n");
}

/**
 * Test 4: Invalid/Missing Book
 * Temporarily set book_file_path to a nonexistent file.
 * Expected: Engine still functions normally without crashes; search operates normally.
 */
static void test_invalid_missing_book(void) {
    printf("Running Test 4: Invalid/Missing Book...\n");
    Board board;
    parse_fen(startFEN, &board);

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = 2;
    info.starttime = get_time_ms();

    // Set invalid book file path
    snprintf(book_file_path, sizeof(book_file_path), "nonexistent_book_12345.bin");
    use_book = true;

    uint32_t move = search_best_move(&board, &info);

    assert(move != 0 && "Expected search to succeed even if book.bin is missing");
    assert(info.nodes > 0 && "Expected fallback to normal search (nodes > 0)");
    printf("  -> Missing book move: %s (nodes: %llu)\n", move_to_string(move), (unsigned long long)info.nodes);

    // Restore original path
    snprintf(book_file_path, sizeof(book_file_path), "%s", original_book_path);
    printf("  ✓ Test 4 Passed!\n\n");
}

/**
 * Test 5: Castling Book Move
 * Build a temporary one-entry Polyglot book for a position where castling is
 * legal, storing the move in Polyglot's own encoding (king moves onto its own
 * rook, e.g. e1h1). The engine must remap that to the UCI form (e1g1), return
 * the castling move, and skip the search.
 */
static void test_castling_scenario(const char* fen, uint16_t pg_move, const char* expected_uci,
                                   const char* desc) {
    Board board;
    parse_fen(fen, &board);

    const char* tmp_path = "book_castle_test.bin";
    FILE* tmp = fopen(tmp_path, "wb");
    if (!tmp) {
        fprintf(stderr, "[ERR] fopen failed for %s\n", tmp_path);
        return;
    }

    PolyglotEntry e;
    e.key = polyglot_hash(&board);
    e.move = pg_move;
    e.weight = 1;
    e.learn = 0;

    /* Polyglot files are big-endian; get_polyglot_move() byte-swaps on read. */
    unsigned char buf[16];
    for (int i = 0; i < 8; i++)
        buf[i] = (unsigned char)(e.key >> (56 - 8 * i));
    buf[8] = (unsigned char)(e.move >> 8);
    buf[9] = (unsigned char)(e.move & 0xFF);
    buf[10] = (unsigned char)(e.weight >> 8);
    buf[11] = (unsigned char)(e.weight & 0xFF);
    buf[12] = (unsigned char)(e.learn >> 24);
    buf[13] = (unsigned char)(e.learn >> 16);
    buf[14] = (unsigned char)(e.learn >> 8);
    buf[15] = (unsigned char)(e.learn & 0xFF);
    size_t nw = fwrite(buf, 1, sizeof(buf), tmp);
    if (nw != sizeof(buf)) {
        fclose(tmp);
        return;
    }
    fclose(tmp);

    snprintf(book_file_path, sizeof(book_file_path), "%s", tmp_path);
    use_book = true;

    SearchInfo info;
    memset(&info, 0, sizeof(SearchInfo));
    info.depth = 6;
    info.starttime = get_time_ms();

    uint32_t move = search_best_move(&board, &info);
    assert(move != 0 && "Expected castling move returned from book");
    assert(info.nodes == 0 && "Expected search to be skipped (book move found)");

    char* move_str = move_to_string(move);
    printf("  -> %s: %s (nodes: %llu)\n", desc, move_str, (unsigned long long)info.nodes);
    assert(strcmp(move_str, expected_uci) == 0 &&
           "Expected Polyglot castling encoding to be remapped to UCI castling");

    remove(tmp_path);
}

/**
 * Verifies that all four castling encodings (White/Black x kingside/queenside)
 * stored in Polyglot form are correctly played from the opening book.
 */
static void test_castling_book_move(void) {
    printf("Running Test 5: Castling Book Move...\n");

    /* White to move, both castling rights intact. */
    test_castling_scenario("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 0x0107, "e1g1",
                           "White kingside (e1h1 -> e1g1)");
    test_castling_scenario("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 0x0100, "e1c1",
                           "White queenside (e1a1 -> e1c1)");

    /* Black to move, both castling rights intact. */
    test_castling_scenario("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 0x0F3F, "e8g8",
                           "Black kingside (e8h8 -> e8g8)");
    test_castling_scenario("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 0x0F38, "e8c8",
                           "Black queenside (e8a8 -> e8c8)");

    /* Restore the original book path. */
    snprintf(book_file_path, sizeof(book_file_path), "%s", original_book_path);
    printf("  ✓ Test 5 Passed!\n\n");
}

int main(void) {
    init_magics();
    init_evaluation_masks();
    locate_book();

    printf("==========================================\n");
    printf("   Running Opening Book Search Tests      \n");
    printf("==========================================\n\n");

    test_starting_position();
    test_known_book_line();
    test_out_of_book_position();
    test_invalid_missing_book();
    test_castling_book_move();

    printf("==========================================\n");
    printf("   All Opening Book Search Tests Passed!  \n");
    printf("==========================================\n");
    return 0;
}
