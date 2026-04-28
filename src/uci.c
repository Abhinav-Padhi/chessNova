#include "defs.h"

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

/** @brief Handles the UCI loop, processing commands from the user. */
void uci_loop() {
    char line[2048];
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    while (1) {
        if (!fgets(line, sizeof(line), stdin)) continue;

        if (line[0] == '\n') continue;

        if (strncmp(line, "uci", 3) == 0) {
            printf("id name chessNova\n");
            printf("id author Mrgreenapple24\n");
            printf("uciok\n");
        }

        else if (strncmp(line, "isready", 7) == 0) {
            printf("readyok\n");
        }

        else if (strncmp(line, "position", 8) == 0) {
            printf("info string Position received\n");
        }

        else if (strncmp(line, "quit", 4) == 0) {
            break;
        }
    }
}
