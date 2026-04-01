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

# Building the Project

This project uses **CMake** as its build system. Follow the steps below to compile the engine from source.

### 1. Prerequisites
Ensure you have the following installed on your system:
* **CMake** (version 3.10 or higher)
* **C Compiler** (GCC or Clang)
* **Make** or another build generator (like Ninja)

### 2. Build Instructions
The recommended way to build is using an "out-of-source" build. This keeps the generated build artifacts separate from the source code.

```bash
# 1. Clone the repository (if you haven't already)
git clone <your-repo-link>
cd <repo-folder-name>

# 2. Create and enter a build directory
mkdir build
cd build

# 3. Generate the build files
# For maximum performance (Release mode), use:
cmake -DCMAKE_BUILD_TYPE=Release ..

# 4. Compile the project
cmake --build .

# 5. Running the engine
./bin/chess_engine

# 6. Cleaning the build
rm -rf build
