#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* move_to_string(uint32_t move) {
    static char move_str[6];
    int from = GET_FROM(move);
    int to = GET_TO(move);
    int promoted = GET_PROMOTED(move);

    move_str[0] = 'a' + (from % 8);
    move_str[1] = '1' + (from / 8);
    move_str[2] = 'a' + (to % 8);
    move_str[3] = '1' + (to / 8);
    move_str[4] = '\0';

    if (promoted != EMPTY) {
        char pchar = ' ';
        switch (promoted) {
            case wn: case bn: pchar = 'n'; break;
            case wb: case bb: pchar = 'b'; break;
            case wr: case br: pchar = 'r'; break;
            case wq: case bq: pchar = 'q'; break;
        }
        move_str[4] = pchar;
        move_str[5] = '\0';
    }

    return move_str;
}

uint32_t parse_move(char *ptr, Board *board) {
    if (ptr[1] > '8' || ptr[1] < '1') return 0;
    if (ptr[0] > 'h' || ptr[0] < 'a') return 0;
    if (ptr[3] > '8' || ptr[3] < '1') return 0;
    if (ptr[2] > 'h' || ptr[2] < 'a') return 0;

    uint32_t from = (ptr[1] - '1') * 8 + (ptr[0] - 'a');
    uint32_t to = (ptr[3] - '1') * 8 + (ptr[2] - 'a');

    MoveList list;
    generate_all_moves(board, &list);

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;
        if (GET_FROM(move) == from && GET_TO(move) == to) {
            int promoted = GET_PROMOTED(move);
            if (promoted != EMPTY) {
                if (ptr[4] == 'n' && (promoted == wn || promoted == bn)) return move;
                if (ptr[4] == 'b' && (promoted == wb || promoted == bb)) return move;
                if (ptr[4] == 'r' && (promoted == wr || promoted == br)) return move;
                if (ptr[4] == 'q' && (promoted == wq || promoted == bq)) return move;
                continue;
            }
            return move;
        }
    }

    return 0;
}

void parse_position(char *line, Board *board) {
    line += 9; // Skip "position "
    char *ptr = line;

    if (strncmp(line, "startpos", 8) == 0) {
        parse_fen(startFEN, board);
    } else {
        ptr = strstr(line, "fen");
        if (ptr == NULL) {
            parse_fen(startFEN, board);
        } else {
            ptr += 4;
            parse_fen(ptr, board);
        }
    }

    ptr = strstr(line, "moves");
    if (ptr != NULL) {
        ptr += 6;
        while (*ptr) {
            uint32_t move = parse_move(ptr, board);
            if (move == 0) break;
            make_move(board, move);
            while (*ptr && *ptr != ' ') ptr++;
            if (*ptr == ' ') ptr++;
        }
    }
}

static int initialized = 0;

void ensure_initialized() {
    if (!initialized) {
        init_magics();
        init_evaluation_masks();
        initialized = 1;
    }
}

void uci_loop() {
    char line[2048];
    Board board;
    parse_fen(startFEN, &board);

    setbuf(stdout, NULL);

    while (1) {
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0] == '\n') continue;

        if (strncmp(line, "uci", 3) == 0) {
            printf("id name chessNova\n");
            printf("id author Mrgreenapple24\n");
            printf("uciok\n");
        } else if (strncmp(line, "isready", 7) == 0) {
            ensure_initialized();
            printf("readyok\n");
        } else if (strncmp(line, "position", 8) == 0) {
            ensure_initialized();
            parse_position(line, &board);
        } else if (strncmp(line, "ucinewgame", 10) == 0) {
            parse_fen(startFEN, &board);
        } else if (strncmp(line, "go", 2) == 0) {
            ensure_initialized();
            uint32_t move = search_best_move(&board, 5);
            printf("bestmove %s\n", move_to_string(move));
        } else if (strncmp(line, "quit", 4) == 0) {
            break;
        }
    }
}
