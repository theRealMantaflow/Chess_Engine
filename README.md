# Chess Engine

A C++ chess engine implementation using bitboard representation for efficient move generation and board state management.

## Overview

This project implements a chess engine with the following key components:

- **Bitboard Representation**: Efficient 64-bit representation of piece positions on the chess board
- **Move Generation**: Complete move generation for all chess pieces (pawns, knights, bishops, rooks, queens, kings)
- **Board Management**: Full chess board state management with support for special moves
- **Move Validation**: Comprehensive move validation including check detection and castling rules

## Architecture

### Core Components

#### `Bitboard` Class
- Represents the piece configuration for one side (white or black)
- Uses `uint64_t` for efficient bitwise operations
- Tracks individual piece types: pawns, knights, bishops, rooks, queens, kings
- Manages castling rights (`mCanKCastle`, `mCanQCastle`)
- Supports getters and setters for each piece type

**Key Methods:**
```cpp
- getPawns(), getKnights(), getBishops(), getRooks(), getKing(), getQueens()
- setPawns(), setKnights(), setBishops(), setRooks(), setKing(), setQueens()
- getAll() - Returns combined bitboard of all pieces
```

#### `MainBoard` Class
- Manages the complete chess board state
- Maintains two `Bitboard` objects for white and black pieces
- Handles move generation and validation
- Implements piece movement for all chess pieces
- Supports special moves: castling, en passant (implied), pawn promotion, captures

**Key Features:**
- Check detection and validation
- Castling legality verification
- Move parser supporting algebraic notation
- Piece movement calculations for each piece type

## Supported Move Notation

The engine supports modified standard chess algebraic notation:

### Simple Moves
- `g1Nf3` - Move knight on g1 to f3.
- `g1Nxf3` - Capture piece on f3 with knight on g1.
- `d4Nf3+` - Move knigh on d4 to f3, delivering check.
- `d4Nf3#` - Move knight on d4 to f3, delivering checkmate.
- `e3e4` - Pawn move to e4.
- `e7e8=Q` - Pawn promotion to queen (also N, B, R for knight, bishop, rook).
- `OO` - King-side castling.
- `OOO` - Queen-side castling.

This modified notation is followed to make the moving process much simpler to handle, and also to eleminate the problem of disambiguating moves [Wikipedia: Disambiguating Moves](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Disambiguating_moves).

## Files

- `Bitboard.hh` / `Bitboard.cpp` - Bitboard class implementation
- `MainBoard.hh` / `MainBoard.cpp` - Main board class implementation
- `chess.cpp` - Main entry point
- `test.cpp` - Test suite

## Building

### Requirements
- C++17 or later
- GCC or compatible compiler

### Compilation
```bash
g++ -std=c++17 -o chess chess.cpp Bitboard.cpp MainBoard.cpp
```

<!-- ### Running Tests
```bash
g++ -std=c++17 -o test_chess test.cpp Bitboard.cpp MainBoard.cpp
./test_chess
``` -->

## Usage

The engine provides a `makeMove()` function to update the board state:

```cpp
MainBoard board;
board.makeMove("e4", true);      // White plays e4
board.makeMove("e5", false);     // Black plays e5
board.makeMove("Nf3", true);     // White plays Nf3
```

## Implementation Details

### Move Calculation
Each piece type has dedicated movement calculation methods:
- `knightMoves(row, col)` - Returns array of 8 possible knight moves
- `bishopMoves(row, col)` - Returns array of 13 possible bishop moves
- `rookMoves(row, col)` - Returns array of 14 possible rook moves
- `kingMoves(row, col)` - Returns array of 8 possible king moves
- `queenMoves(row, col)` - Returns array of 27 possible queen moves

Invalid moves are marked with `-1` in the returned arrays.

### Board State
- Uses `coordinateParser()` to convert move notation to board coordinates
- `wholeBoard()` method returns a combined bitboard of all occupied squares
- `verifyCheck()` determines if a king is in check
- `checkCastle()` validates castling legality and updates castling rights

## Author

Sreeram S Nair ([GitHub: realMantaflow](https://github.com/realMantaflow))

## License

GPL 3.0

## Future Enhancements

- Search algorithm implementation (minimax, alpha-beta pruning)
- Evaluation function for position assessment
- Transposition table for caching
- Performance optimization with magic bitboards (after implementing the rest of the above)
- UCI protocol support for GUI integration
- Opening book integration (not anytime soon)
