#pragma once
#include "MainBoard.hh"
#include <string_view>
#include <array>
#include <stdexcept>

namespace FENParser {
    
    // Applies a FEN string to the MainBoard
    // FEN format: position activeColor castling enpassant halfmove fullmove
    // Example: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    void applyFEN(MainBoard& board, std::string_view fenString);

};