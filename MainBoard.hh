// Author: Sreeram S Nair (github: realMantaflow)

#pragma once

#include "bitboard.hh"
#include <array>
#include <string>

class MainBoard {
    
    Bitboard mWhite;
    Bitboard mBlack;
    bool     mIsCheck;

    // The function is used to remove pieces from a board;
    uint64_t removeHelper(uint64_t, int, int);

    // from and towards does not expect piece designators. ONLY mention the location and target (eg. a4 -> c5).
    // The function does not care which piece it's trying to move, and DOES NOT check if the target is occuppied.
    uint64_t moveHelper(uint64_t, int, int, int, int);

    // Converts the coordinates from string notation to frow, fcol, trow, tcol
    // Expects the custom notation (not the standard algebraic notation)
    std::array<int,4> coordinateParser(std::string_view move);

    // Returns the state of the whole board, just showing if a square is occupied or not
    uint64_t wholeBoard();

    // To see if the specified king is in check
    bool verifyCheck(bool); 

    // Checks if the specified color can castle or not.
    // Updates the mCanCastle variable in Bitboard.
    void checkCastle(bool);

    // Provides the squares the knight can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,8> knightMoves(int, int);

    // Provides the squares the bishop can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,13> bishopMoves(int, int);

    // Provides the squares the rook can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,14> rookMoves(int, int);

    // Provides the squares the king can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,8> kingMoves(int, int);

    // Provides the squares the queen can visit, from a specified square (row, col)
    // -1 indicates the move is invalid
    std::array<int,27> queenMoves(int, int);

    // Captures a piece present on the victim board, using a piece on the attacker board;
    // 
    bool capturePiece (Bitboard&, Bitboard&, int, int, int, int);

public:
    MainBoard();
    MainBoard(Bitboard&, Bitboard&, bool);
    
    // Operations
    /*
    *
    * This function DOES NOT care about #, and +.
    * It only considers castling, pawn push, captures, and promotions.
    *                       Simple ones: 
    * Nf3           -> Move knight to f3.
    * Nxf3          -> Move knight to f3, while cutting the piece that is currently on it.
    * Nf3+          -> Move the knight to f3, while delivering a check.
    * Nf3#          -> Move the knight to f3, while delivering a checkmate.
    * e4            -> Pawn movements
    * e8=Q/N/B/R    -> Promotions to queen/knight/bishop/rook.
    * OOO/OO        -> Castle Queen/King side.
    *                       Complex Ones:
    * Description: This is mainly when two or more of same pieces can move to the same square (eg. 2+ knights, 
    * 2+ bishops of same color, 2+ queens/rooks). 
    * There is a hierarchy to this, which can be learnt on wikipedia https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Disambiguating_moves.
    * 
    */
    void makeMove(std::string_view, bool);
    
};