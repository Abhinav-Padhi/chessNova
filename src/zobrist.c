#include "zobrist.h"

U64 PieceKeys[12][64];
U64 SideKey;
U64 CastleKeys[16];
U64 EnPassantKeys[8];

static U64 xorshift64(U64* state) {
    U64 x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return *state = x;
}

void init_zobrist(void) {
    U64 seed = 1070372ULL; // Fixed seed for reproducible keys

    for (int p = 0; p < 12; p++) {
        for (int sq = 0; sq < 64; sq++) {
            PieceKeys[p][sq] = xorshift64(&seed);
        }
    }

    SideKey = xorshift64(&seed);

    for (int i = 0; i < 16; i++) {
        CastleKeys[i] = xorshift64(&seed);
    }

    for (int i = 0; i < 8; i++) {
        EnPassantKeys[i] = xorshift64(&seed);
    }
}

U64 generate_pos_key(const Board* board) {
    U64 key = 0ULL;

    // Pieces
    for (int sq = 0; sq < 64; sq++) {
        int piece = board->pieces[sq];
        if (piece != EMPTY && piece >= wp && piece <= bk) {
            key ^= PieceKeys[piece][sq];
        }
    }

    // Side to move
    if (board->side == black) {
        key ^= SideKey;
    }

    // Castling
    key ^= CastleKeys[board->castle & 0xF];

    // En Passant
    if (board->enpassant != NO_SQ) {
        int file = board->enpassant % 8;
        key ^= EnPassantKeys[file];
    }

    return key;
}