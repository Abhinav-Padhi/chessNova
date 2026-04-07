#include <stdio.h>
#include "defs.h"

/** @brief Character representation of each piece type. */
char piece_char[] = "PNBRQKpnbrqk";
int FilesBrd[64];
int RanksBrd[64];

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

U64 GeneratePosKey(const Board *pos) {

	int sq = 0;
	U64 finalKey = 0;
	int piece = EMPTY;

	for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if(piece != NO_SQ && piece != OFFBOARD) {
			ASSERT(piece>=wp && piece<=bk);
			finalKey ^= PieceKeys[piece][sq];
		}
	}

	if(pos->side == white) {
		finalKey ^= SideKey;
	}

	if(pos->enpassant != NO_SQ) {
		ASSERT(pos->enpassant >=0 && pos->enpassant< 64);
		ASSERT(SqOnBoard(pos->enpassant));
		ASSERT(RanksBrd[pos->enpassant] == RANK_3 || RanksBrd[pos->enpassant] == RANK_6);
		finalKey ^= PieceKeys[EMPTY][pos->enpassant];
	}

	ASSERT(pos->castle>=0 && pos->castle<=15);

	finalKey ^= CastleKeys[pos->castle];

	return finalKey;
}

void init_board(Board *board) {
	for(index = 0; index < 64; ++index) {
		FilesBrd[index] = OFFBOARD;
		RanksBrd[index] = OFFBOARD;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = rank*8 + file;
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}
}
