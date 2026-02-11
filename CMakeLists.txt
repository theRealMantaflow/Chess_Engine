# Note: AI GENERATED CMakeLists

cmake_minimum_required(VERSION 3.31.3)
project(Chess_Engine LANGUAGES CXX)

# Require at least C++17 (allows newer compilers too)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Public include directory
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Build Bitboard as a library and expose its include directory
add_library(Bitboard STATIC
    Bitboard.cpp
)

target_include_directories(Bitboard
    PUBLIC
        ${PROJECT_INCLUDE_DIR}
)

# Add the executable target
add_executable(Chess_Engine
    chess.cpp
    FENParser.cpp
    MainBoard.cpp
)

# The executable needs the library
target_link_libraries(Chess_Engine
    PRIVATE
        Bitboard
)

# If the executable needs the include dir directly (usually not required
# because Bitboard exports it), keep it private:
target_include_directories(Chess_Engine
    PRIVATE
        ${PROJECT_INCLUDE_DIR}
)

# Optional: control build type defaults
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()