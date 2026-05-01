#include "search.h"
#include <stdio.h>

/**
 * Checks if the search should be stopped due to time or other conditions.
 */
static void check_up(SearchInfo *info) {
    if (info->timeset && get_time_ms() > info->stoptime) {
        info->stopped = 1;
    }
}

/**
 * Quiescence search to handle the horizon effect by searching captures until the position is quiet.
 */
static int quiescence(Board *board, SearchInfo *info, int alpha, int beta) {
    if ((info->nodes & 2047) == 0) {
        check_up(info);
    }
    info->nodes++;

    int stand_pat = evaluate(board);
    if (stand_pat >= beta) return beta;
    if (alpha < stand_pat) alpha = stand_pat;

    MoveList list;
    generate_all_moves(board, &list);

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;
        if (!(move & MFLAG_CAP)) continue;

        if (!make_move(board, move)) continue;
        int score = -quiescence(board, info, -beta, -alpha);
        unmake_move(board);

        if (info->stopped) return 0;

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

/**
 * Alpha-Beta pruning search.
 */
static int alpha_beta(Board *board, SearchInfo *info, int depth, int alpha, int beta) {
    if (depth == 0) return quiescence(board, info, alpha, beta);

    if ((info->nodes & 2047) == 0) {
        check_up(info);
    }
    info->nodes++;

    MoveList list;
    generate_all_moves(board, &list);

    int legal_moves = 0;
    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;

        if (!make_move(board, move)) continue;
        legal_moves++;
        int score = -alpha_beta(board, info, depth - 1, -beta, -alpha);
        unmake_move(board);

        if (info->stopped) return 0;

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
             return -MATE_SCORE + board->ply;
        }
        return 0; // Stalemate
    }

    return alpha;
}

uint32_t search_best_move(Board *board, SearchInfo *info) {
    uint32_t best_move = 0;
    int best_score = -INFINITY;
    int current_depth = 1;

    // Iterative deepening
    for (current_depth = 1; current_depth <= info->depth; current_depth++) {
        MoveList list;
        generate_all_moves(board, &list);

        uint32_t depth_best_move = 0;
        int depth_best_score = -INFINITY;

        for (int i = 0; i < list.count; i++) {
            uint32_t move = list.moves[i].move;

            if (!make_move(board, move)) continue;
            int score = -alpha_beta(board, info, current_depth - 1, -INFINITY, INFINITY);
            unmake_move(board);

            if (info->stopped) break;

            if (score > depth_best_score) {
                depth_best_score = score;
                depth_best_move = move;
            }
        }

        if (info->stopped) break;

        best_move = depth_best_move;
        best_score = depth_best_score;

        printf("info score cp %d depth %d nodes %lu time %lu\n", 
               best_score, current_depth, info->nodes, get_time_ms() - info->starttime);
        
        if (best_score > MATE_SCORE - 100 || best_score < -MATE_SCORE + 100) break;
    }

    return best_move;
}
