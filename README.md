# chessNova

A high-performance chess engine focused on bitboard-based board representation. 
The project is currently in the early stages of development, focusing on 
low-level bit manipulation and move generation foundations.

## Current Progress
- **Bitboard Core:** 64-bit integer representation of the chess board.
- **Attack Logic:** Fully implemented attack bitboards for all pieces.
    - Pre-calculated tables for Leapers (Knights, Kings).
    - Sliding piece logic (Rooks, Bishops, Queens).
    - Pawn capture and push bitmask generation.
- **Board State:** Initial infrastructure for piece placement and square control.

## Future Focus
- Legal move generation and validation.
- Alpha-Beta search implementation.
- UCI protocol integration.
