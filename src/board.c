#include <stdio.h>
#include "defs.h"

/** @brief Character representation of each piece type. */
char piece_char[] = "PNBRQKpnbrqk";

/** @brief Prints the current board state to the console. */
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

/** @brief Prints the given bitboard to the console. */
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

/** @brief Resets the board to its initial state. */
void reset_board(Board *board) {
    for (int i = 0; i <= bk; i++) {
        board->bitboards[i] = 0;
    }

    board->occupancies[0] = 0;
    board->occupancies[1] = 0;
    board->occupancies[2] = 0;
    board->side = 0;
    board->enpassant = -1;
    board->castle = 0;
}

/** @brief Parses a FEN string and sets the board state accordingly. */
void parse_fen(const char *fen, Board *board) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file <= 7; file++) {
            int square = rank * 8 + file;

            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                int piece = -1;
                switch (*fen) {
                    case 'P': piece = wp; break;
                    case 'N': piece = wn; break;
                    case 'B': piece = wb; break;
                    case 'R': piece = wr; break;
                    case 'Q': piece = wq; break;
                    case 'K': piece = wk; break;
                    case 'p': piece = bp; break;
                    case 'n': piece = bn; break;
                    case 'b': piece = bb; break;
                    case 'r': piece = br; break;
                    case 'q': piece = bq; break;
                    case 'k': piece = bk; break;
                }

                if (piece != -1) {
                    set_bit(&board->bitboards[piece], square);
                }
                fen++;
            }

            if (*fen >= '0' && *fen <= '9') {
                int offset = *fen - '0';
                file += (offset - 1);
                fen++;
            }

            if (*fen == '/') fen++;
        }
    }

    fen++;
    board->side = (*fen == 'w') ? white : black;
}
