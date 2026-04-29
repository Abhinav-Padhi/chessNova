cmake_minimum_required(VERSION 3.10)

# Project Name
project(chess_engine LANGUAGES C)

# Set the C standard (optional, but recommended)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compile flags (Equivalent to CFLAGS)
# Note: CMake handles -MMD -MP automatically for dependency tracking
if(MSVC)
  # Strong warnings for Microsoft Visual C++
  add_compile_options(/W4)
else()
  # Strong warnings for GCC and Clang
  add_compile_options(-Wall -Wextra)
endif()

# Include directories (Equivalent to -Iinclude)
include_directories(include)

# Gather Source Files
file(GLOB ALL_SRCS "src/*.c")

# Sources for the main engine (everything except test_search.c)
set(ENGINE_SRCS ${ALL_SRCS})
list(REMOVE_ITEM ENGINE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/test_search.c")

# Sources for the tests (everything except main.c)
set(TEST_SRCS ${ALL_SRCS})
list(REMOVE_ITEM TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

# Define the executable
add_executable(chess_engine ${ENGINE_SRCS})

# Define the test executable
add_executable(test_search ${TEST_SRCS})

# Output directory configuration
# This keeps the binary in a 'bin' folder within your build directory
set_target_properties(chess_engine test_search PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
