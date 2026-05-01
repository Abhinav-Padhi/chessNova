#include <stdio.h>
#include <time.h>
#include "defs.h"
#include "sliding.h"
#include "movegen.h"

int main() {
    init_magics();
    uci_loop();

    return 0;
}
