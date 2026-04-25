#include <stdio.h>
#include "defs.h"
#include "sliding.h"
#include "movegen.h"

void print_move(uint32_t move) {
    int from = GET_FROM(move);
    int to = GET_TO(move);
    
    char from_file = 'a' + (from % 8);
    int from_rank = 1 + (from / 8);
    char to_file = 'a' + (to % 8);
    int to_rank = 1 + (to / 8);
    
    printf("%c%d%c%d", from_file, from_rank, to_file, to_rank);
    
    if (GET_PROMOTED(move) != EMPTY) {
        char piece_char[] = "pnbrqk";
        // Convert piece index to char (e.g., wn -> n)
        int p = GET_PROMOTED(move);
        if (p >= bp) p -= 6;
        printf("%c", piece_char[p]);
    }
    
    if (move & MFLAG_CA) printf(" (Castle)");
    if (move & MFLAG_EP) printf(" (EP)");
    if (move & MFLAG_CAP) printf(" (Capture)");
    printf("\n");
}

void test_position(char *fen) {
    Board board;
    MoveList list;
    
    printf("\n--- Testing Position ---\n");
    printf("FEN: %s\n", fen);
    parse_fen(fen, &board);
    print_board(&board);
    
    printf("Generating moves for %s...\n", (board.side == white) ? "white" : "black");
    generate_all_moves(&board, &list);
    
    printf("Total moves generated: %d\n", list.count);
    for (int i = 0; i < list.count; i++) {
        printf("Move %2d: ", i + 1);
        print_move(list.moves[i].move);
    }
}

int main() {
    init_magics();
    
    // Test White
    test_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    
    // Test Black
    test_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");

    return 0;
}
