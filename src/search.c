#include "search.h"
#include <stdio.h>

/**
 * Quiescence search to handle the horizon effect by searching captures until the position is quiet.
 */
static int quiescence(Board *board, int alpha, int beta) {
    int stand_pat = evaluate(board);
    if (stand_pat >= beta) return beta;
    if (alpha < stand_pat) alpha = stand_pat;

    MoveList list;
    generate_all_moves(board, &list);

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;
        if (!(move & MFLAG_CAP)) continue;

        if (!make_move(board, move)) continue;
        int score = -quiescence(board, -beta, -alpha);
        unmake_move(board);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

/**
 * Alpha-Beta pruning search.
 */
static int alpha_beta(Board *board, int depth, int alpha, int beta) {
    if (depth == 0) return quiescence(board, alpha, beta);

    MoveList list;
    generate_all_moves(board, &list);

    int legal_moves = 0;
    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;

        if (!make_move(board, move)) continue;
        legal_moves++;
        int score = -alpha_beta(board, depth - 1, -beta, -alpha);
        unmake_move(board);

        if (score >= beta) return beta; // Pruning
        if (score > alpha) alpha = score;
    }

    if (legal_moves == 0) {
        U64 king_bb = board->bitboards[(board->side == white) ? wk : bk];
        if (king_bb) {
            int king_sq = get_lsb(king_bb);
            if (is_square_attacked(board, king_sq, board->side ^ 1)) {
                return -MATE_SCORE + board->ply; // Checkmate
            }
        } else {
             return -MATE_SCORE + board->ply; // King missing is effectively checkmate
        }
        return 0; // Stalemate
    }

    return alpha;
}

uint32_t search_best_move(Board *board, int depth) {
    MoveList list;
    generate_all_moves(board, &list);

    uint32_t best_move = 0;
    int best_score = -INFINITY;

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;

        if (!make_move(board, move)) continue;
        int score = -alpha_beta(board, depth - 1, -INFINITY, INFINITY);
        unmake_move(board);

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    printf("info score cp %d depth %d\n", best_score, depth);
    return best_move;
}
