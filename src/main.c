#include <stdio.h>
#include "defs.h"
#include "sliding.h"

int main() {
    init_magics();

    int rook_sq = d4;

    U64 occupancy = 0ULL;
    set_bit(&occupancy, d2);
    set_bit(&occupancy, d7);
    set_bit(&occupancy, b4); // Blocker on b4
    set_bit(&occupancy, f4); // Blocker on f4

    printf("\nOccupancy (Blockers):\n");
    print_bitboard(occupancy);

    // 3. Get Magic Attacks
    U64 rook_attacks = get_rook_attacks(rook_sq, occupancy);

    printf("\nMagic Rook Attacks at d4:\n");
    print_bitboard(rook_attacks);

    return 0;
}
