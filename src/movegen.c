/**
 * @file movegen.c
 * @brief Pseudo-legal move generation logic.
 */

#include "movegen.h"
#include "sliding.h"
#include "knights.h"
#include "kings.h"

/**
 * @brief Checks if a square is attacked by a given side.
 * @param pos Pointer to the board.
 * @param sq The square to check.
 * @param side The side that might be attacking the square.
 * @return true if the square is attacked, false otherwise.
 */
bool is_square_attacked(const Board *pos, int sq, int side) {
    // Pawns
    if (side == white) {
        if (arrbPawnAttacks[sq] & pos->bitboards[wp]) return true;
    } else {
        if (arrwPawnAttacks[sq] & pos->bitboards[bp]) return true;
    }

    // Knights
    if (arrKnightAttacks[sq] & ((side == white) ? pos->bitboards[wn] : pos->bitboards[bn])) return true;

    // Kings
    if (arrKingAttacks[sq] & ((side == white) ? pos->bitboards[wk] : pos->bitboards[bk])) return true;

    // Sliders
    U64 occ = pos->occupancies[both];
    U64 rooks_queens = (side == white) ? (pos->bitboards[wr] | pos->bitboards[wq]) : (pos->bitboards[br] | pos->bitboards[bq]);
    if (get_rook_attacks(sq, occ) & rooks_queens) return true;

    U64 bishops_queens = (side == white) ? (pos->bitboards[wb] | pos->bitboards[wq]) : (pos->bitboards[bb] | pos->bitboards[bq]);
    if (get_bishop_attacks(sq, occ) & bishops_queens) return true;

    return false;
}

/**
 * @brief Adds a move to the move list.
 * @param list Pointer to the MoveList.
 * @param move The 32-bit packed move.
 */
static void add_move(MoveList *list, uint32_t move) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

/**
 * @brief Generates all pseudo-legal moves for the current position.
 * @param pos Pointer to the current board.
 * @param list Pointer to the list where moves will be stored.
 */
void generate_all_moves(const Board *pos, MoveList *list) {
    list->count = 0;
    int side = pos->side;
    U64 occ = pos->occupancies[both];
    U64 enemy_occ = (side == white) ? pos->occupancies[black] : pos->occupancies[white];
    U64 empty = ~occ;

    if (side == white) {
        // --- White Pawn Moves ---
        U64 pawns = pos->bitboards[wp];
        
        // Single Push
        U64 single_push = (pawns << 8) & empty;
        U64 temp_push = single_push;
        while (temp_push) {
            int to = pop_lsb(&temp_push);
            int from = to - 8;
            if (to >= a8) { // Promotion
                add_move(list, MOVE(from, to, EMPTY, wn, 0));
                add_move(list, MOVE(from, to, EMPTY, wb, 0));
                add_move(list, MOVE(from, to, EMPTY, wr, 0));
                add_move(list, MOVE(from, to, EMPTY, wq, 0));
            } else {
                add_move(list, MOVE(from, to, EMPTY, EMPTY, 0));
            }
        }

        // Double Push
        U64 double_push = ((single_push & 0x0000000000FF0000ULL) << 8) & empty;
        while (double_push) {
            int to = pop_lsb(&double_push);
            add_move(list, MOVE(to - 16, to, EMPTY, EMPTY, MFLAG_PS));
        }

        // Captures
        U64 caps = (pawns << 7) & enemy_occ & notHFile;
        while (caps) {
            int to = pop_lsb(&caps);
            if (to >= a8) { // Capture + Promotion
                add_move(list, MOVE(to - 7, to, EMPTY, wn, MFLAG_CAP));
                add_move(list, MOVE(to - 7, to, EMPTY, wb, MFLAG_CAP));
                add_move(list, MOVE(to - 7, to, EMPTY, wr, MFLAG_CAP));
                add_move(list, MOVE(to - 7, to, EMPTY, wq, MFLAG_CAP));
            } else {
                add_move(list, MOVE(to - 7, to, EMPTY, EMPTY, MFLAG_CAP));
            }
        }
        caps = (pawns << 9) & enemy_occ & notAFile;
        while (caps) {
            int to = pop_lsb(&caps);
            if (to >= a8) {
                add_move(list, MOVE(to - 9, to, EMPTY, wn, MFLAG_CAP));
                add_move(list, MOVE(to - 9, to, EMPTY, wb, MFLAG_CAP));
                add_move(list, MOVE(to - 9, to, EMPTY, wr, MFLAG_CAP));
                add_move(list, MOVE(to - 9, to, EMPTY, wq, MFLAG_CAP));
            } else {
                add_move(list, MOVE(to - 9, to, EMPTY, EMPTY, MFLAG_CAP));
            }
        }

        // En Passant
        if (pos->enpassant != NO_SQ) {
            U64 ep_targets = arrwPawnAttacks[pos->enpassant]; // Need to ensure these are defined correctly
            // Simplified for now: check squares next to the EP square
            if (test_bit(pawns, pos->enpassant - 7) && (pos->enpassant % 8 != 0)) 
                add_move(list, MOVE(pos->enpassant - 7, pos->enpassant, bp, EMPTY, MFLAG_EP | MFLAG_CAP));
            if (test_bit(pawns, pos->enpassant - 9) && (pos->enpassant % 8 != 7))
                add_move(list, MOVE(pos->enpassant - 9, pos->enpassant, bp, EMPTY, MFLAG_EP | MFLAG_CAP));
        }
    } else {
        // --- Black Pawn Moves ---
        U64 pawns = pos->bitboards[bp];
        
        // Single Push
        U64 single_push = (pawns >> 8) & empty;
        U64 temp_push = single_push;
        while (temp_push) {
            int to = pop_lsb(&temp_push);
            int from = to + 8;
            if (to <= h1) { // Promotion
                add_move(list, MOVE(from, to, EMPTY, bn, 0));
                add_move(list, MOVE(from, to, EMPTY, bb, 0));
                add_move(list, MOVE(from, to, EMPTY, br, 0));
                add_move(list, MOVE(from, to, EMPTY, bq, 0));
            } else {
                add_move(list, MOVE(from, to, EMPTY, EMPTY, 0));
            }
        }

        // Double Push
        U64 double_push = ((single_push & 0x00FF000000000000ULL) >> 8) & empty;
        while (double_push) {
            int to = pop_lsb(&double_push);
            add_move(list, MOVE(to + 16, to, EMPTY, EMPTY, MFLAG_PS));
        }

        // Captures
        U64 caps = (pawns >> 7) & enemy_occ & notAFile;
        while (caps) {
            int to = pop_lsb(&caps);
            if (to <= h1) {
                add_move(list, MOVE(to + 7, to, EMPTY, bn, MFLAG_CAP));
                add_move(list, MOVE(to + 7, to, EMPTY, bb, MFLAG_CAP));
                add_move(list, MOVE(to + 7, to, EMPTY, br, MFLAG_CAP));
                add_move(list, MOVE(to + 7, to, EMPTY, bq, MFLAG_CAP));
            } else {
                add_move(list, MOVE(to + 7, to, EMPTY, EMPTY, MFLAG_CAP));
            }
        }
        caps = (pawns >> 9) & enemy_occ & notHFile;
        while (caps) {
            int to = pop_lsb(&caps);
            if (to <= h1) {
                add_move(list, MOVE(to + 9, to, EMPTY, bn, MFLAG_CAP));
                add_move(list, MOVE(to + 9, to, EMPTY, bb, MFLAG_CAP));
                add_move(list, MOVE(to + 9, to, EMPTY, br, MFLAG_CAP));
                add_move(list, MOVE(to + 9, to, EMPTY, bq, MFLAG_CAP));
            } else {
                add_move(list, MOVE(to + 9, to, EMPTY, EMPTY, MFLAG_CAP));
            }
        }

        // En Passant
        if (pos->enpassant != NO_SQ) {
            if (test_bit(pawns, pos->enpassant + 7) && (pos->enpassant % 8 != 7))
                add_move(list, MOVE(pos->enpassant + 7, pos->enpassant, wp, EMPTY, MFLAG_EP | MFLAG_CAP));
            if (test_bit(pawns, pos->enpassant + 9) && (pos->enpassant % 8 != 0))
                add_move(list, MOVE(pos->enpassant + 9, pos->enpassant, wp, EMPTY, MFLAG_EP | MFLAG_CAP));
        }
    }

    // --- Piece Moves (Knights, King, Sliders) ---
    U64 friendly_occ = pos->occupancies[side];

    // Knights
    U64 knights = (side == white) ? pos->bitboards[wn] : pos->bitboards[bn];
    while (knights) {
        int from = pop_lsb(&knights);
        U64 attacks = arrKnightAttacks[from] & ~friendly_occ;
        while (attacks) {
            int to = pop_lsb(&attacks);
            uint32_t flags = (test_bit(enemy_occ, to)) ? MFLAG_CAP : 0;
            add_move(list, MOVE(from, to, EMPTY, EMPTY, flags));
        }
    }

    // King
    U64 king = (side == white) ? pos->bitboards[wk] : pos->bitboards[bk];
    if (king) {
        int from = get_lsb(king);
        U64 attacks = arrKingAttacks[from] & ~friendly_occ;
        while (attacks) {
            int to = pop_lsb(&attacks);
            uint32_t flags = (test_bit(enemy_occ, to)) ? MFLAG_CAP : 0;
            add_move(list, MOVE(from, to, EMPTY, EMPTY, flags));
        }
        
        // Castling
        if (side == white) {
            if (pos->castle & WKCA) {
                if (!test_bit(occ, f1) && !test_bit(occ, g1)) {
                    if (!is_square_attacked(pos, e1, black) && !is_square_attacked(pos, f1, black)) {
                        add_move(list, MOVE(e1, g1, EMPTY, EMPTY, MFLAG_CA));
                    }
                }
            }
            if (pos->castle & WQCA) {
                if (!test_bit(occ, d1) && !test_bit(occ, c1) && !test_bit(occ, b1)) {
                    if (!is_square_attacked(pos, e1, black) && !is_square_attacked(pos, d1, black)) {
                        add_move(list, MOVE(e1, c1, EMPTY, EMPTY, MFLAG_CA));
                    }
                }
            }
        } else {
            if (pos->castle & BKCA) {
                if (!test_bit(occ, f8) && !test_bit(occ, g8)) {
                    if (!is_square_attacked(pos, e8, white) && !is_square_attacked(pos, f8, white)) {
                        add_move(list, MOVE(e8, g8, EMPTY, EMPTY, MFLAG_CA));
                    }
                }
            }
            if (pos->castle & BQCA) {
                if (!test_bit(occ, d8) && !test_bit(occ, c8) && !test_bit(occ, b8)) {
                    if (!is_square_attacked(pos, e8, white) && !is_square_attacked(pos, d8, white)) {
                        add_move(list, MOVE(e8, c8, EMPTY, EMPTY, MFLAG_CA));
                    }
                }
            }
        }
    }

    // Sliders
    U64 bishops = (side == white) ? pos->bitboards[wb] : pos->bitboards[bb];
    while (bishops) {
        int from = pop_lsb(&bishops);
        U64 attacks = get_bishop_attacks(from, occ) & ~friendly_occ;
        while (attacks) {
            int to = pop_lsb(&attacks);
            uint32_t flags = (test_bit(enemy_occ, to)) ? MFLAG_CAP : 0;
            add_move(list, MOVE(from, to, EMPTY, EMPTY, flags));
        }
    }

    U64 rooks = (side == white) ? pos->bitboards[wr] : pos->bitboards[br];
    while (rooks) {
        int from = pop_lsb(&rooks);
        U64 attacks = get_rook_attacks(from, occ) & ~friendly_occ;
        while (attacks) {
            int to = pop_lsb(&attacks);
            uint32_t flags = (test_bit(enemy_occ, to)) ? MFLAG_CAP : 0;
            add_move(list, MOVE(from, to, EMPTY, EMPTY, flags));
        }
    }

    U64 queens = (side == white) ? pos->bitboards[wq] : pos->bitboards[bq];
    while (queens) {
        int from = pop_lsb(&queens);
        U64 attacks = get_queen_attacks(from, occ) & ~friendly_occ;
        while (attacks) {
            int to = pop_lsb(&attacks);
            uint32_t flags = (test_bit(enemy_occ, to)) ? MFLAG_CAP : 0;
            add_move(list, MOVE(from, to, EMPTY, EMPTY, flags));
        }
    }
}
