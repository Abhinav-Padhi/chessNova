#ifndef DEFS_H
#define DEFS_H

#include "board.h"
#include "bitboard.h"
#include "pawns.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
#include "uci.h"
#include "kings.h"
#include "knights.h"
#include "sliding.h"

#define startFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);}
#endif

#endif
