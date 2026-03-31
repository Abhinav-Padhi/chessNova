#include "../include/defs.h"

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
