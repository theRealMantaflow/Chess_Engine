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

The engine uses a coordinate-based notation system that simplifies move parsing and eliminates disambiguation issues:

### Move Format
- `Ng1f3` - Move knight from g1 to f3
- `Ng1xf3` - Capture piece on f3 with knight from g1
- `e3e4` - Pawn move from e3 to e4
- `e7e8=Q` - Pawn promotion to queen on e8 (also N, B, R for knight, bishop, rook)
- `OO` - King-side castling
- `OOO` - Queen-side castling

### Special Features
- **En Passant**: Automatically handled in pawn move logic with `mEnPassantLoc` tracking
- **Check/Checkmate**: `+` and `#` symbols are NOT required in notation (handled internally)
- **Disambiguation**: Fully qualified notation (piece origin and destination) eliminates all ambiguity

This notation format simplifies move handling and avoids the complexity of disambiguating moves as described in [Wikipedia's Algebraic Notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Disambiguating_moves).

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
MainBoard board;  // Creates a new game with initial position
board.makeMove("e2e4", true);    // White pawn e2 to e4
board.makeMove("e7e5", false);   // Black pawn e7 to e5
board.makeMove("Ng1f3", true);   // White knight from g1 to f3
board.makeMove("Nb8c6", false);  // Black knight from b8 to c6
```

## Implementation Details

### Move Calculation
Each piece type has dedicated movement calculation methods:
- `knightMoves(row, col, isWhite)` - Returns array of 8 possible knight moves
- `bishopMoves(row, col, isWhite)` - Returns array of 13 possible bishop moves
- `rookMoves(row, col, isWhite)` - Returns array of 14 possible rook moves
- `kingMoves(row, col, isWhite)` - Returns array of 8 possible king moves
- `queenMoves(row, col, isWhite)` - Returns array of 27 possible queen moves
- `pawnMoves(row, col, isWhite)` - Returns array of 4 possible pawn moves (including en passant)

All methods take an `isWhite` parameter (defaults to `true`) to determine which pieces can be moved to.
Invalid moves are marked with `-1` in the returned arrays.

### Board State
- **Move Parsing**: `coordinateParser()` converts coordinate notation to array indices (frow, fcol, trow, tcol)
- **Board Occupancy**: `wholeBoard()` returns a combined bitboard of all occupied squares
- **Check Detection**: `verifyCheck(isWhite)` determines if a king is in check
- **Castling Validation**: `checkCastle(isWhite, kingSide)` validates castling legality and updates castling rights
- **En Passant**: `mEnPassantLoc` tracks en passant capture opportunity (-1 if none available)
- **Piece Capture**: `capturePiece()` handles piece captures with move legality checking

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
