#include <stdio.h>
#include <time.h>
#include "defs.h"

int main() {
    init_magics();
    init_evaluation_masks();
    
    uci_loop();

    return 0;
}
