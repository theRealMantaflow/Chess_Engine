// Author: Sreeram S Nair (github: realMantaflow)

#include "MainBoard.hh"

MainBoard::MainBoard(){
    white   = new Bitboard();
    black   = new Bitboard(false);
    isCheck = false;
}

MainBoard::MainBoard(Bitboard& rwhite, Bitboard& rblack, bool isCheck = false) {
    white     = rwhite; 
    black     = rblack;
    this->isCheck   = isCheck;
}

//////////////////////
// THIS IS AN IMPORTANT FUNCTION TO IMPLEMENT
bool MainBoard::verifyCheck(bool isWhite=true) {
    // 
} 

//////////////////////

uint64_t MainBoard::wholeBoard(){
    return white.getAll() | black.getAll();
}

bool MainBoard::capturePiece(Bitboard &attacker, Bitboard &victim, int frow, int fcol, int trow, int tcol) {

    auto k_a  = attacker.getKnights();
    auto b_a  = attacker.getBishops();
    auto r_a  = attacker.getRooks();
    auto kg_a = attacker.getKing();
    auto q_a  = attacker.getQueens();
    auto p_a  = attacker.getPawns();

    auto k_v  = victim.getKnights();
    auto b_v  = victim.getBishops();
    auto r_v  = victim.getRooks();
    auto kg_v = victim.getKing();
    auto q_v  = victim.getQueens();
    auto p_v  = victim.getPawns();

    // moving the "attacking piece"; Make sure to add legality checking (eg. if the move results in a check) later
    if ( *b_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setBishops( moveHelper(*b_a, frow, fcol, trow, tcol) );

    } else if ( *k_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setKnights( moveHelper(*k_a, frow, fcol, trow, tcol) );
        
    } else if ( *r_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setRooks( moveHelper(*r_a, frow, fcol, trow, tcol) );
        
    } else if ( *q_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setQueens( moveHelper(*q_a, frow, fcol, trow, tcol) );

    }  else if ( *kg_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setKing( moveHelper(*kg_a, frow, fcol, trow, tcol) );

    }  else if ( *p_a >> (frow*8+fcol) & 0x1ULL ) {
        attacker.setPawns( moveHelper(*p_a, frow, fcol, trow, tcol) );

    } else {
        return false;
    }

    // deleting the "captured piece"; Make sure to add legality checking (eg. if the move results in a check) later
    if ( *b_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setBishops( removeHelper(*b_v, trow, tcol) );

    } else if ( *k_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setKnights( removeHelper(*k_v, trow, tcol) );
        
    } else if ( *r_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setRooks( removeHelper(*r_v, trow, tcol) );
        
    } else if ( *q_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setQueens( removeHelper(*q_v, trow, tcol) );
        
    }  else if ( *kg_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setKing( removeHelper(*kg_v, trow, tcol) );
        
    }  else if ( *p_v >> (trow*8+tcol) & 0x1ULL ) {
        black.setPawns( removeHelper(*p_v, trow, tcol) );

    } else {
        return false;
    }

    return true;

}

uint64_t MainBoard::removeHelper(uint64_t bboard, int trow, int tcol) {

    bboard ^= 1ULL << (trow*8 + tcol);
    return bboard;

}

std::array<int,4> MainBoard::coordinateParser(std::string_view move) {

    auto to     = move.substr(move.size()-2, 2);
    auto from   = move.substr(1, 2);

    std::array<int,4> out {
        from[1]-'1',
        abs(from[0]-'h'),
        to[1]-'1',
        abs(to[0]-'h')
    };

    return out;

}

uint64_t MainBoard::moveHelper(uint64_t bboard, int frow, int fcol, int trow, int tcol) {
    
    // std::cout << frow << ", " << fcol << "\n" << trow << ", " << tcol << std::endl;
    bboard |= 1ULL << (trow*8 + tcol);
    bboard = removeHelper(bboard, frow, fcol);
    // bboard ^= 1ULL << (frow*8 + fcol);
    return bboard;
    
}

void MainBoard::makeMove(std::string_view move, bool isWhite) {
    
try {
    char piece = move[0];
    auto &turn = (isWhite)?white:black;
    auto &other = (isWhite)?black:white;
    // int sz = move.length();
    
    if ( piece == 'N') {

        // auto k_w = turn.getKnights();
        
        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw "Illegal move or invalid input";

        } else {

            turn.setKnights( moveHelper(*turn.getKnights(), cord[0], cord[1], cord[2], cord[3]) );
        }

    } else if ( piece == 'B' ) {

        // auto b_w  = turn.getBishops();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw "Illegal move or invalid input";

        } else {
            
            turn.setBishops( moveHelper(*turn.getBishops(), cord[0], cord[1], cord[2], cord[3]) );

        }

    } else if ( piece == 'R' ) {

        // auto r_w  = turn.getRooks();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw "Illegal move or invalid input";

        } else {

            turn.setRooks( moveHelper(*turn.getRooks(), cord[0], cord[1], cord[2], cord[3]) );

        }

    } else if ( piece == 'Q' ) {

        // auto q_w  = turn.getQueens();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw "Illegal move or invalid input";

        } else {
            
            turn.setQueens( moveHelper(*turn.getQueens(), cord[0], cord[1], cord[2], cord[3]) );

        }

    } else if ( piece == 'K' ) {

        // auto q_w  = turn.getKing();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw "Illegal move or invalid input";

        } else {
            
            turn.setKing( moveHelper(*turn.getKing(), cord[0], cord[1], cord[2], cord[3]) );

        }

    } else if ( piece == 'OO' ) {
        // prevent castle through check, castle into check, etc.
    } else if ( piece == 'OOO' ) {
        //
    }  else if ( piece >= 'a' || piece <= 'h' ) {
        // Pawn push, including promotions (with and without capture) and enpassant
    } else {
        // 
    }
} catch (std::string_view er) {
    
}

}

std::array<int,8> MainBoard::knightMoves (int row, int col){

    std::array<int,8> positions = {
        ( row+1 <  8 && col+2 <  8 ) ? (row+1)*8 + (col+2) : -1, 
        ( row+1 <  8 && col-2 >= 0 ) ? (row+1)*8 + (col-2) : -1, 
        ( row+2 <  8 && col+1 <  8 ) ? (row+2)*8 + (col+1) : -1, 
        ( row+2 <  8 && col-1 >= 0 ) ? (row+2)*8 + (col-1) : -1, 
        ( row-1 >= 0 && col+2 <  8 ) ? (row-1)*8 + (col+2) : -1, 
        ( row-1 >= 0 && col-2 >= 0 ) ? (row-1)*8 + (col-2) : -1, 
        ( row-2 >= 0 && col+1 <  8 ) ? (row-2)*8 + (col+1) : -1, 
        ( row-2 >= 0 && col-1 >= 0 ) ? (row-2)*8 + (col-1) : -1
    };

    return positions;
}

std::array<int,13> MainBoard::bishopMoves (int row, int col){
    //
}

std::array<int,14> MainBoard::rookMoves (int row, int col){
    //
}

std::array<int,8> MainBoard::kingMoves (int row, int col){
    //
}

std::array<int,27> MainBoard::queenMoves (int row, int col){
    //
}