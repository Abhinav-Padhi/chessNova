#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint64_t U64;
#define max 2048;
#define notAFile 0xfefefefefefefefe; // ~0x0101010101010101
#define notHFile 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

enum { white, black, both };
enum { wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk };
enum {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 }; // castling rights

#endif
