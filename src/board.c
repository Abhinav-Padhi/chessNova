#include <stdio.h>
#include "../include/board.h"

char piece_char[] = "PNBRQKpnbrqk";

void print_board(Board *board) {
    printf("\n");
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file <= 7; file++) {
            int square = rank * 8 + file;
            int piece = -1;

            for (int bb_piece = wp; bb_piece <= bk; bb_piece++) {
                if ((board->bitboards[bb_piece] >> square) & 1ULL) {
                    piece = bb_piece;
                    break;
                }
            }

            if (piece == -1) printf(" . ");
            else printf(" %c ", piece_char[piece]);
        }
        printf("  %d\n", rank + 1);
    }
    printf("\n a  b  c  d  e  f  g  h\n\n");
}

void print_bitboard(U64 bitboard) {
    printf("\n");
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file <= 7; file++) {
            int square = rank * 8 + file;

            if ((bitboard >> square) & 1ULL)
                printf(" 1 ");
            else
                printf(" . ");
        }
        printf("  %d\n", rank + 1);
    }
    printf("\n a  b  c  d  e  f  g  h\n\n");
    printf(" Bitboard: 0x%lx\n\n", bitboard);
}
