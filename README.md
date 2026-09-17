# chessNova ♟️

A high-performance chess engine focused on bitboard-based board representation.

## Current Progress

- **Bitboard Core:** 64-bit integer representation of the chess board
- **Attack Logic:** Fully implemented attacks for all pieces (leapers & sliding pieces)
- **Board State:** Piece placement, square control, and Zobrist hashing
- **Move Generation:** Full legal move generation (Perft verified)
- **Magic Bitboards:** High-performance sliding piece attacks
- **Search Engine:** Alpha-Beta with iterative deepening, quiescence search, and move ordering
- **Evaluation:** Tapered evaluation with piece-square tables
- **UCI Protocol:** Full UCI support for GUI integration

## What's Next

See [PLAN.md](./PLAN.md) for the detailed roadmap.

---

## Building

### Prerequisites

| Tool | Required? | Notes |
|---|---|---|
| **CMake** ≥ 3.20 | ✅ Yes | Build system generator |
| **C compiler** | ✅ Yes | GCC, Clang, or MSVC (C11 required) |
| **Ninja** | ⭐ Recommended | Much faster builds; required for `make test` and `make lint` |
| **clang-format** | Optional | For `make format` |
| **clang-tidy** | Optional | For `make lint` / `make lint-fix` |

#### Install on Linux (Debian/Ubuntu)

```bash
sudo apt install build-essential cmake ninja-build clang-format clang-tidy
```

#### Install on macOS (Homebrew)

```bash
brew install cmake ninja llvm
# Homebrew's llvm is keg-only; add to PATH if needed:
# export PATH="$(brew --prefix llvm)/bin:$PATH"
```

#### Install on Windows

Install [CMake](https://cmake.org/download/), [Ninja](https://github.com/ninja-build/ninja/releases), and either MSVC (via Visual Studio) or MinGW.

### Clone

```bash
git clone https://github.com/mrgreenapple24/chessNova.git
cd chessNova
```

### Build

The project uses a top-level `GNUmakefile` that wraps CMake. Just run `make`:

```bash
make                # Build release (CMake's default generator)
make ninja          # Build release with Ninja (recommended — faster)
make debug          # Build debug
make ninja-debug    # Build debug with Ninja
```

Binaries land in `build/<platform>-<config>/bin/chess_engine`.

### Run

```bash
make run            # Build + run release
make run-debug      # Build + run debug
```

Or execute the binary directly:

```bash
./build/linux-release-ninja/bin/chess_engine
```

### Test

Tests are opt-in — they are **not** built by default, only when you run `make test`:

```bash
make test           # Build + run all tests (release)
make test-debug     # Build + run all tests (debug)
```

The test build lives in a separate directory (`build/<platform>-<config>-test-ninja`), so it doesn't pollute your normal build artifacts.

### Format & Lint

```bash
make format         # Run clang-format on src/, include/, tests/
make lint           # Run clang-tidy (uses compile_commands.json from Ninja build)
make lint-fix       # Run clang-tidy with --fix (auto-apply suggestions)
```

> **Note:** Run `make format` before committing. CI also runs clang-format and clang-tidy checks.

### Clean

```bash
make clean          # Remove build dirs for current config
make distclean      # Remove the entire build/ tree
make rebuild        # clean + rebuild (Ninja)
```

### All Commands

Run `make help` to see the full list:

```
chess_engine build system
=========================

Usage: make [target] [VAR=value]

Targets:
  all             Build release (default, CMake generator)
  clean           Remove current build dirs
  debug           Build debug (default generator)
  distclean       Remove the entire build/ tree
  format          Run clang-format on sources
  help            Show this help message
  info            Print current build configuration
  lint            Run clang-tidy on sources
  lint-fix        Run clang-tidy with --fix (auto-apply suggestions)
  ninja           Build with Ninja (release)
  ninja-debug     Build with Ninja (debug)
  rebuild         Clean + rebuild (Ninja)
  release         Build release (default generator)
  run             Build + run chess_engine
  run-debug       Build + run in debug mode
  test            Build + run all tests
  test-debug      Build + run tests in debug mode
```

### Overriding Variables

```bash
JOBS=16 make ninja                   # Use 16 parallel jobs
BUILD_DIR=build/experimental make    # Custom build directory
BUILD_TYPE=RelWithDebInfo make       # Optimized build with debug symbols
```

### Advanced: Manual CMake

If you prefer raw CMake:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# With tests:
cmake -S . -B build-test -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build-test --parallel
ctest --test-dir build-test --output-on-failure
```

### Documentation

```bash
doxygen Doxyfile
# Then open docs/html/index.html
```

---