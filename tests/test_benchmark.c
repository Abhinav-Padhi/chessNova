/**
 * @file test_benchmark.c
 * @brief Performance and NPS (Nodes Per Second) benchmark suite for ChessNova.
 */

#include "board.h"
#include "defs.h"
#include "search.h"
#include "tt.h"
#include <stdio.h>
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

typedef struct {
    const char* fen;
    const char* name;
    int search_depth;
} BenchmarkPosition;

static const BenchmarkPosition BENCHMARK_POSITIONS[] = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "StartPos", 7},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", "Kiwipete", 6},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", "Endgame", 8},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", "Pos 4 Tactical", 6},
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", "Pos 5 Fine", 6}};

static void run_nps_benchmark(void) {
    printf("\n+---------------------+-------+--------------+-----------+--------------+\n");
    printf("| Position            | Depth | Nodes        | Time (ms) | NPS          |\n");
    printf("+---------------------+-------+--------------+-----------+--------------+\n");

    init_tt(64);

    U64 total_nodes = 0;
    U64 total_time_ms = 0;

    for (size_t i = 0; i < sizeof(BENCHMARK_POSITIONS) / sizeof(BENCHMARK_POSITIONS[0]); i++) {
        Board board;
        SearchInfo info;

        memset(&info, 0, sizeof(SearchInfo));
        parse_fen(BENCHMARK_POSITIONS[i].fen, &board);

        info.depth = BENCHMARK_POSITIONS[i].search_depth;
        info.starttime = get_time_ms();
        info.timeset = false;

        // Suppress stdout during search_best_move execution
        fflush(stdout);
        int saved_stdout = dup(fileno(stdout));
        FILE* null_out = fopen(NULL_DEV, "w");
        if (null_out) {
            dup2(fileno(null_out), fileno(stdout));
            fclose(null_out);
        }

        U64 start_time = get_time_ms();
        search_best_move(&board, &info);
        U64 end_time = get_time_ms();

        // Restore stdout
        fflush(stdout);
        dup2(saved_stdout, fileno(stdout));
        close(saved_stdout);

        U64 elapsed_ms = end_time - start_time;
        if (elapsed_ms == 0)
            elapsed_ms = 1;

        U64 nodes = info.nodes;
        U64 nps = (nodes * 1000) / elapsed_ms;

        total_nodes += nodes;
        total_time_ms += elapsed_ms;

        printf("| %-19.19s | %5d | %12llu | %9llu | %12llu |\n", BENCHMARK_POSITIONS[i].name,
               BENCHMARK_POSITIONS[i].search_depth, (unsigned long long)nodes,
               (unsigned long long)elapsed_ms, (unsigned long long)nps);
    }

    free_tt();

    if (total_time_ms == 0)
        total_time_ms = 1;
    U64 overall_nps = (total_nodes * 1000) / total_time_ms;

    printf("+---------------------+-------+--------------+-----------+--------------+\n");
    printf("| %-19s | %5s | %12llu | %9llu | %12llu |\n", "TOTAL / OVERALL", "-",
           (unsigned long long)total_nodes, (unsigned long long)total_time_ms,
           (unsigned long long)overall_nps);
    printf("+---------------------+-------+--------------+-----------+--------------+\n\n");
}

int main(void) {
    init_magics();
    init_evaluation_masks();
    run_nps_benchmark();
    return 0;
}