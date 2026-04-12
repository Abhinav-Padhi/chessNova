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

# Gather Source Files (Equivalent to wildcard)
file(GLOB SRCS "src/*.c")

# Define the executable (Equivalent to TARGET)
add_executable(chess_engine ${SRCS})

# Output directory configuration
# This keeps the binary in a 'bin' folder within your build directory
set_target_properties(chess_engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
