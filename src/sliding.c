#include "../include/sliding.h"

U64 rayAttacks[64][8];
void calculate_ray_attacks() {
    U64 nort = (0x0101010101010100);
    U64 noea = (0x8040201008040200);
    U64 east = (0x00000000000000FE);
    U64 nowe = (0x0102040810204080) & ~(1 << h1);
    U64 sout = (0x0080808080808080);
    U64 west = (0x00000000000000FF) & ~(1 << h1);
    U64 smth_west = ~((uint64_t)1 << h8);
    U64 smth_east = ~((uint64_t)1 << a8);
    U64 sowe = (0x8040201008040201) & smth_west;
    U64 soea = (0x0102040810204080) & smth_east;

    for (int sq=0; sq < 64; sq++, nort <<= 1)
       rayAttacks[sq][Nort] = nort;

    for (int f = 0; f < 8; f++) {
        U64 ne = east;
        for (int r8 = 0; r8 < 8; r8++, ne = shiftEast(ne))
           rayAttacks[r8+f*8][East] = ne;

        east = shiftNorth(east);
    }

    for (int f = 0; f < 8; f++) {
        U64 nw = nowe;
        for (int r8 = 7; r8 >= 0; r8--, nw = shiftWest(nw))
           rayAttacks[r8+f*8][NoWe] = nw;

        nowe = shiftNorth(nowe);
    }

    for (int f = 7; f >= 0; f--) {
        U64 sw = sowe;
        for (int r8 = 7; r8 >= 0; r8--, sw = shiftWest(sw))
           rayAttacks[r8+f*8][SoWe] = sw;

        sowe = shiftSouth(sowe);
    }

    for (int f = 7; f >= 0; f--) {
        U64 se = soea;
        for (int r8 = 0; r8 < 8; r8++, se = shiftEast(se))
           rayAttacks[r8+f*8][SoEa] = se;

        soea = shiftSouth(soea);
    }

    for (int f = 0; f < 8; f++) {
        U64 nw = west;
        for (int r8 = 7; r8 >= 0; r8--, nw = shiftWest(nw))
           rayAttacks[r8+f*8][West] = nw;

        west = shiftNorth(west);
    }


    for (int f=0; f < 8; f++, noea = shiftEast(noea)) {
       U64 ne = noea;
       for (int r8 = 0; r8 < 8*8; r8 += 8, ne <<= 8)
          rayAttacks[r8+f][NoEa] = ne;
    }

    for (int sq=63; sq >= 0; sq--, sout >>= 1)
       rayAttacks[sq][Sout] = sout;
}
