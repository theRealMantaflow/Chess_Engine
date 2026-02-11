// Author: Sreeram S Nair (github: realMantaflow)

#pragma once

#include "Bitboard.hh"
#include <array>
#include <string_view>
#include <string>
#include <stdexcept>

class MainBoard {
    
    // True -> white
    bool     mTurn;
    Bitboard mWhite;
    Bitboard mBlack;
    bool     mIsCheck;
    int      mHalfMoves;
    int      mFullMoves;
    int      mEnPassantLoc;

    // The function is used to remove pieces from a board;
    uint64_t removeHelper(uint64_t, int, int);

    // from and towards does not expect piece designators. ONLY mention the location and target (eg. a4 -> c5).
    // The function does not care which piece it's trying to move, and DOES NOT check if the target is occuppied.
    uint64_t moveHelper(uint64_t, int, int, int, int);

    // Converts the coordinates from string notation to frow, fcol, trow, tcol
    // Expects the custom notation (not the standard algebraic notation)
    std::array<int,4> coordinateParser(std::string_view move);

    // To see if the specified king is in check
    bool verifyCheck(bool=true); 

    // Checks if the specified color can castle or not.
    // Will forcefully update the mCanCastle series of variables if the castling rook is not present
    bool checkCastle(bool, bool);

    // Provides the squares the knight can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,8> knightMoves(int, int, bool=true);

    // Provides the squares the bishop can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,13> bishopMoves(int, int, bool=true);

    // Provides the squares the rook can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,14> rookMoves(int, int, bool=true);

    // Provides the squares the king can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,8> kingMoves(int, int, bool=true);

    // Provides the squares the queen can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,27> queenMoves(int, int, bool=true);
    
    // Provides the squares the pawn can visit, from a specified square
    // -1 indicates the move in invalid.
    std::array<int,4> pawnMoves(int, int, bool=true);

    // Returns the coordinate of the king
    int kingCoord(bool=true);

    // Captures a piece present on the victim board, using a piece on the attacker board;
    bool capturePiece (Bitboard&, Bitboard&, int, int, int, int);

public:
    MainBoard();
    MainBoard(Bitboard&, Bitboard&, bool, int, bool, int, int);

    // Returns the state of the whole board, just showing if a square is occupied or not
    const uint64_t wholeBoard();

    // Generates a FEN string from the current board state
    std::string generateFEN();
    
    // Operations
    /*
    *
    * This function DOES NOT care about #, and +.
    * It only considers castling, pawn push, captures, promotions, en passants.
    * 
    * `Ng1f3` - Move knight on g1 to f3.
    * `Ng1xf3` - Capture piece on f3 with knight on g1.
    * `Nd4f3` - Move knigh on d4 to f3, delivering check ('+' symbol NOT required).
    * `Nd4f3` - Move knight on d4 to f3, delivering checkmate ('#' symbol NOT required).
    * `e3e4` - Pawn move to e4.
    * `e7e8=Q` - Pawn promotion to queen (also N, B, R for knight, bishop, rook).
    * `OO` - King-side castling.
    * `OOO` - Queen-side castling.
    * 
    */
    void makeMove(std::string_view, bool);
    
};