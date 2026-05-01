#include "polyglot.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> /* For ntohs/ntohl on Unix-like systems */

/* Endianness helpers for big-endian Polyglot files */
#if defined(__linux__) || defined(__APPLE__)
#include <endian.h>
#define bswap64(x) be64toh(x)
#define bswap32(x) be32toh(x)
#define bswap16(x) be16toh(x)
#else
/* Fallback for other systems */
static uint64_t bswap64(uint64_t x) {
    return ((x & 0x00000000000000FFULL) << 56) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x00000000FF000000ULL) << 8) |
           ((x & 0x000000FF00000000ULL) >> 8) |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0xFF00000000000000ULL) >> 56);
}
#define bswap32(x) ntohl(x)
#define bswap16(x) ntohs(x)
#endif

char book_file_path[256] = "book.bin";
bool use_book = true;

/* TODO: User will add Zobrist keys here */

uint64_t polyglot_hash(Board *board) {
    uint64_t hash = 0ULL;
    
    /* 
     * SKELETON:
     * 1. Iterate over board squares and XOR piece/square keys.
     * 2. XOR castling rights keys.
     * 3. XOR en passant keys (only if capture possible).
     * 4. XOR side to move key.
     */
     
    (void)board; /* Unused for now */
    return hash;
}

static uint32_t convert_polyglot_move(uint16_t pg_move, Board *board) {
    int to_sq = pg_move & 0x3F;
    int from_sq = (pg_move >> 6) & 0x3F;
    int promote = (pg_move >> 12) & 0x7;

    /* Generate all legal moves to find the matching one */
    MoveList list;
    generate_all_moves(board, &list);

    for (int i = 0; i < list.count; i++) {
        uint32_t move = list.moves[i].move;
        if ((int)GET_FROM(move) == from_sq && (int)GET_TO(move) == to_sq) {
            int promoted = GET_PROMOTED(move);
            if (promote == 0 && promoted == EMPTY) return move;
            if (promote == 1 && (promoted == wn || promoted == bn)) return move;
            if (promote == 2 && (promoted == wb || promoted == bb)) return move;
            if (promote == 3 && (promoted == wr || promoted == br)) return move;
            if (promote == 4 && (promoted == wq || promoted == bq)) return move;
        }
    }

    return 0;
}

uint32_t get_polyglot_move(Board *board) {
    if (!use_book) return 0;

    FILE *fp = fopen(book_file_path, "rb");
    if (!fp) return 0;

    uint64_t key = polyglot_hash(board);
    
    /* Binary search in the .bin file */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    long entries = size / sizeof(PolyglotEntry);

    long low = 0;
    long high = entries - 1;
    long first_entry = -1;

    while (low <= high) {
        long mid = low + (high - low) / 2;
        PolyglotEntry entry;
        fseek(fp, mid * sizeof(PolyglotEntry), SEEK_SET);
        if (fread(&entry, sizeof(PolyglotEntry), 1, fp) != 1) break;

        uint64_t entry_key = bswap64(entry.key);

        if (entry_key == key) {
            first_entry = mid;
            high = mid - 1; /* Find the very first occurrence */
        } else if (entry_key < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (first_entry == -1) {
        fclose(fp);
        return 0;
    }

    /* Collect all moves for this hash */
    uint32_t candidate_moves[256];
    int candidate_weights[256];
    int count = 0;
    int total_weight = 0;

    fseek(fp, first_entry * sizeof(PolyglotEntry), SEEK_SET);
    PolyglotEntry entry;
    while (fread(&entry, sizeof(PolyglotEntry), 1, fp) == 1) {
        if (bswap64(entry.key) != key) break;
        
        uint32_t move = convert_polyglot_move(bswap16(entry.move), board);
        if (move) {
            candidate_moves[count] = move;
            candidate_weights[count] = bswap16(entry.weight);
            total_weight += candidate_weights[count];
            count++;
        }
        if (count >= 256) break;
    }

    fclose(fp);

    if (count == 0) return 0;

    /* Select a move randomly based on weights */
    if (total_weight > 0) {
        int r = rand() % total_weight;
        int sum = 0;
        for (int i = 0; i < count; i++) {
            sum += candidate_weights[i];
            if (r < sum) return candidate_moves[i];
        }
    }

    return candidate_moves[0];
}
