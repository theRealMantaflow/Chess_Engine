#include "FENParser.hh"
#include <sstream>
#include <cctype>

// Helper function to convert algebraic notation (e.g., e4) to bitboard index (0-63)
static int notationToBitIndex(std::string_view notation) {
    if (notation.length() != 2) {
        throw std::invalid_argument("Invalid square notation");
    }
    int col = -(notation[0] - 'h');  // h-a -> 0-7
    int row = (notation[1] - '1');  // 1-8 -> 0-7
    
    if (col < 0 || col > 7 || row < 0 || row > 7) {
        throw std::invalid_argument("Invalid square notation");
    }
    
    return row * 8 + col;
}

// Helper function to convert bitboard index to algebraic notation
// static std::string bitIndexToNotation(int index) {
//     int col = index % 8;
//     int row = index / 8;
//     return std::string(1, 'h' - col) + std::string(1, '8' - row);
// }

void FENParser::applyFEN(MainBoard& board, std::string_view fenString) {
    std::string fen(fenString);
    std::istringstream iss(fen);
    std::string token;
    
    // Parse board position
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing position");
    }
    
    // Initialize empty bitboards
    uint64_t whitePawns = 0, whiteKnights = 0, whiteBishops = 0, whiteRooks = 0, whiteQueens = 0, whiteKing = 0;
    uint64_t blackPawns = 0, blackKnights = 0, blackBishops = 0, blackRooks = 0, blackQueens = 0, blackKing = 0;
    
    int row = 7, col = 7;
    for (char c : token) {
        if (c == '/') {
            row--;
            col = 7;
        } else if (std::isdigit(c)) {
            col -= (c - '0');
        } else {
            int index = row * 8 + col;
            uint64_t bitMask = 1ULL << index;
            
            switch (c) {
                case 'P': whitePawns    |= bitMask; break;
                case 'N': whiteKnights  |= bitMask; break;
                case 'B': whiteBishops  |= bitMask; break;
                case 'R': whiteRooks    |= bitMask; break;
                case 'Q': whiteQueens   |= bitMask; break;
                case 'K': whiteKing     |= bitMask; break;
                case 'p': blackPawns    |= bitMask; break;
                case 'n': blackKnights  |= bitMask; break;
                case 'b': blackBishops  |= bitMask; break;
                case 'r': blackRooks    |= bitMask; break;
                case 'q': blackQueens   |= bitMask; break;
                case 'k': blackKing     |= bitMask; break;
                default: throw std::invalid_argument(std::string("Invalid piece: ") + c);
            }
            col--;
        }
    }
    
    // Set white bitboard
    Bitboard whiteBB(true, false, false);
    whiteBB.setPawns(whitePawns);
    whiteBB.setKnights(whiteKnights);
    whiteBB.setBishops(whiteBishops);
    whiteBB.setRooks(whiteRooks);
    whiteBB.setQueens(whiteQueens);
    whiteBB.setKing(whiteKing);
    
    // Set black bitboard
    Bitboard blackBB(false, false, false);
    blackBB.setPawns(blackPawns);
    blackBB.setKnights(blackKnights);
    blackBB.setBishops(blackBishops);
    blackBB.setRooks(blackRooks);
    blackBB.setQueens(blackQueens);
    blackBB.setKing(blackKing);
    
    // Parse active color
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing active color");
    }
    bool isWhiteToMove = (token == "w");
    
    // Parse castling rights
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing castling rights");
    }
    
    bool whiteKCastle = false, whiteQCastle = false;
    bool blackKCastle = false, blackQCastle = false;
    
    for (char c : token) {
        if (c == 'K') whiteKCastle = true;
        else if (c == 'Q') whiteQCastle = true;
        else if (c == 'k') blackKCastle = true;
        else if (c == 'q') blackQCastle = true;
        else if (c != '-') {
            throw std::invalid_argument("Invalid castling rights");
        }
    }
    
    whiteBB.mCanKCastle = whiteKCastle;
    whiteBB.mCanQCastle = whiteQCastle;
    blackBB.mCanKCastle = blackKCastle;
    blackBB.mCanQCastle = blackQCastle;
    
    // Parse en passant target square
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing en passant square");
    }
    
    int enPassantLoc = -1;
    if (token != "-") {
        if (token.length() == 2) {
            enPassantLoc = notationToBitIndex(token);
        } else {
            throw std::invalid_argument("Invalid en passant square");
        }
    }
    
    // Parse halfmove clock
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing halfmove clock");
    }
    int halfMoves = 0;
    try {
        halfMoves = std::stoi(token);
    } catch (...) {
        throw std::invalid_argument("Invalid halfmove clock: " + token);
    }
    
    // Parse fullmove number
    if (!std::getline(iss, token, ' ')) {
        throw std::invalid_argument("Invalid FEN: missing fullmove number");
    }
    int fullMoves = 1;
    try {
        fullMoves = std::stoi(token);
    } catch (...) {
        throw std::invalid_argument("Invalid fullmove number: " + token);
    }
    
    // Create new MainBoard with parsed data
    MainBoard newBoard(whiteBB, blackBB, false, enPassantLoc, isWhiteToMove, halfMoves, fullMoves);
    
    // Update the original board reference by swapping
    board = newBoard;
}