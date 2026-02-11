// Author: Sreeram S Nair (github: realMantaflow)

#include "MainBoard.hh"

MainBoard::MainBoard(){
    mBlack        = Bitboard(false, true, true);
    mWhite        = Bitboard(true, true, true);
    mTurn         = true;
    mIsCheck      = false;
    mEnPassantLoc = -1;
    mHalfMoves    = 0;
    mFullMoves    = 1;
}

MainBoard::MainBoard(Bitboard& rwhite, Bitboard& rblack, bool isCheck, int enPassantLoc, bool turn, int halfMoves, int fullMoves) 
: mWhite(rwhite), mBlack(rblack), mIsCheck(isCheck), mEnPassantLoc(enPassantLoc), mTurn(turn), mHalfMoves(halfMoves), mFullMoves(fullMoves) {}

bool MainBoard::verifyCheck(bool isWhite) {
    // Get the king's position
    int kingPos = kingCoord(isWhite);
    int kingRow = kingPos / 8;
    int kingCol = kingPos % 8;
    
    // Get opponent's pieces
    auto& opponent = isWhite ? mBlack : mWhite;
    
    // Check if any opponent knight can attack the king
    auto kmoves = knightMoves(kingRow, kingCol, isWhite);
    for (const auto& i : kmoves) {
        if (i != -1 && (*opponent.getKnights() & (1ULL << i))) {
            return true;
        }
    }
    
    // Check if any opponent bishop can attack the king
    auto bmoves = bishopMoves(kingRow, kingCol, isWhite);
    for (const auto& i : bmoves) {
        if (i != -1 && (*opponent.getBishops() & (1ULL << i))) {
            return true;
        }
    }
    
    // Check if any opponent rook can attack the king
    auto rmoves = rookMoves(kingRow, kingCol, isWhite);
    for (const auto& i : rmoves) {
        if (i != -1 && (*opponent.getRooks() & (1ULL << i))) {
            return true;
        }
    }
    
    // Check if any opponent queen can attack the king
    auto qmoves = queenMoves(kingRow, kingCol, isWhite);
    for (const auto& i : qmoves) {
        if (i != -1 && (*opponent.getQueens() & (1ULL << i))) {
            return true;
        }
    }
    
    // Check if any opponent pawn can attack the king
    // Unfortunately, we can't use the pawnMoves() function here, since that returns non-diagonal moves as well
    if (isWhite) {
        // White king: black pawns attack from row+1
        if (kingRow + 1 < 8) {
            if (kingCol - 1 >= 0) {
                int i = (kingRow + 1) * 8 + (kingCol - 1);
                if (*opponent.getPawns() & (1ULL << i)) {
                    return true;
                }
            }
            if (kingCol + 1 < 8) {
                int i = (kingRow + 1) * 8 + (kingCol + 1);
                if (*opponent.getPawns() & (1ULL << i)) {
                    return true;
                }
            }
        }
    } else {
        // Black king: white pawns attack from row-1
        if (kingRow - 1 >= 0) {
            if (kingCol - 1 >= 0) {
                int i = (kingRow - 1) * 8 + (kingCol - 1);
                if (*opponent.getPawns() & (1ULL << i)) {
                    return true;
                }
            }
            if (kingCol + 1 < 8) {
                int i = (kingRow - 1) * 8 + (kingCol + 1);
                if (*opponent.getPawns() & (1ULL << i)) {
                    return true;
                }
            }
        }
    }
    
    // Check if opponent king is adjacent for checking illegal position
    auto kingAttacks = kingMoves(kingRow, kingCol, isWhite);
    for (const auto& i : kingAttacks) {
        if (i != -1 && (*opponent.getKing() & (1ULL << i))) {
            return true;
        }
    }
    
    return false; // King is not in check
}

const uint64_t MainBoard::wholeBoard(){
    return mWhite.getAll() | mBlack.getAll();
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

    int frmPos = frow*8+fcol;
    // moving the "attacking piece"; Make sure to add legality checking (eg. if the move results in a check) later
    if ( (*b_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getBishops();

        attacker.setBishops( moveHelper(*b_a, frow, fcol, trow, tcol) );
        
        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setBishops( prev_ );
            return false;
        }
        
    } else if ( (*k_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getKnights();

        attacker.setKnights( moveHelper(*k_a, frow, fcol, trow, tcol) );

        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setKnights( prev_ );
            return false;
        }
        
    } else if ( (*r_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getRooks();

        attacker.setRooks( moveHelper(*r_a, frow, fcol, trow, tcol) );

        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setRooks( prev_ );
            return false;
        }
        
    } else if ( (*q_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getQueens();

        attacker.setQueens( moveHelper(*q_a, frow, fcol, trow, tcol) );

        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setQueens( prev_ );
            return false;
        }

    }  else if ( (*kg_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getKing();
        
        attacker.setKing( moveHelper(*kg_a, frow, fcol, trow, tcol) );
        
        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setKing( prev_ );
            return false;
        }
        
    }  else if ( (*p_a >> frmPos) & 0x1ULL ) {
        auto prev_ = *attacker.getPawns();

        attacker.setPawns( moveHelper(*p_a, frow, fcol, trow, tcol) );
        if ( verifyCheck(attacker.mIsWhite) ) {
            attacker.setPawns( prev_ );
            return false;
        }
        
    } else {
        return false;
    }

    int toPos = trow*8+tcol;
    // deleting the "captured piece"; Make sure to add legality checking (eg. if the move results in a check) later
    if ( (*b_v >> toPos) & 0x1ULL ) {
        victim.setBishops( removeHelper(*b_v, trow, tcol) );

    } else if ( (*k_v >> toPos) & 0x1ULL ) {
        victim.setKnights( removeHelper(*k_v, trow, tcol) );
        
    } else if ( (*r_v >> toPos) & 0x1ULL ) {
        victim.setRooks( removeHelper(*r_v, trow, tcol) );
        
    } else if ( (*q_v >> toPos) & 0x1ULL ) {
        victim.setQueens( removeHelper(*q_v, trow, tcol) );
        
    }  else if ( (*kg_v >> toPos) & 0x1ULL ) {
        victim.setKing( removeHelper(*kg_v, trow, tcol) );
        
    }  else if ( (*p_v >> toPos) & 0x1ULL ) {
        victim.setPawns( removeHelper(*p_v, trow, tcol) );

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

    std::string to;
    std::string from;

    if ( move[0] >= 'a' && move[0] <= 'h' )
        from = move.substr(0, 2);
    else
        from = move.substr(1, 2);
    
    if ( move.find('=') == std::string::npos )
        to = move.substr(move.size()-2, 2);
    else 
        to = move.substr(move.size()-4, 2);

    std::array<int,4> out {
        from[1]-'1',
        -(from[0]-'h'),
        to[1]-'1',
        -(to[0]-'h')
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
    
    char piece = move[0];
    auto &turn = (isWhite)?mWhite:mBlack;
    auto &other = (isWhite)?mBlack:mWhite;
    // int sz = move.length();
    
    if ( piece == 'N') {

        // auto k_w = turn.getKnights();
        
        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw std::runtime_error("Illegal move or invalid input");
            mHalfMoves = 0;  // Reset halfmove clock on capture

        } else {

            turn.setKnights( moveHelper(*turn.getKnights(), cord[0], cord[1], cord[2], cord[3]) );
            ++mHalfMoves;  // Increment halfmove clock
        }
        mEnPassantLoc = -1;

    } else if ( piece == 'B' ) {

        // auto b_w  = turn.getBishops();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw std::runtime_error("Illegal move or invalid input");
            mHalfMoves = 0;  // Reset halfmove clock on capture

        } else {
            
            turn.setBishops( moveHelper(*turn.getBishops(), cord[0], cord[1], cord[2], cord[3]) );
            ++mHalfMoves;  // Increment halfmove clock

        }
        mEnPassantLoc = -1;

    } else if ( piece == 'R' ) {

        // auto r_w  = turn.getRooks();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) {}
            else throw std::runtime_error("Illegal move or invalid input");
            mHalfMoves = 0;  // Reset halfmove clock on capture

        } else {

            turn.setRooks( moveHelper(*turn.getRooks(), cord[0], cord[1], cord[2], cord[3]) );
            ++mHalfMoves;  // Increment halfmove clock

        }

        mEnPassantLoc = -1;

        if ( isWhite) {

            if ( cord[0]==0 && cord[1]==0 ) {
                turn.mCanKCastle = false;
            } else if ( cord[0]==0 && cord[1]==7 ) {
                turn.mCanQCastle = false;
            }

        } else {

            if ( cord[0]==7 && cord[1]==0 ) {
                turn.mCanKCastle = false;
            } else if ( cord[0]==7 && cord[1]==7 ) {
                turn.mCanQCastle = false;
            }

        }

    } else if ( piece == 'Q' ) {

        // auto q_w  = turn.getQueens();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) throw std::runtime_error("Illegal move or invalid input");
            mHalfMoves = 0;  // Reset halfmove clock on capture

        } else {
            
            turn.setQueens( moveHelper(*turn.getQueens(), cord[0], cord[1], cord[2], cord[3]) );
            ++mHalfMoves;  // Increment halfmove clock

        }
        mEnPassantLoc = -1;

    } else if ( piece == 'K' ) {

        // auto q_w  = turn.getKing();

        auto cord = coordinateParser(move);

        if ( move.find('x') != std::string::npos ) { 

            if ( capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) {}
            else throw std::runtime_error("Illegal move or invalid input");
            mHalfMoves = 0;  // Reset halfmove clock on capture

        } else {
            
            turn.setKing( moveHelper(*turn.getKing(), cord[0], cord[1], cord[2], cord[3]) );
            ++mHalfMoves;  // Increment halfmove clock

        }

        mEnPassantLoc = -1;
        turn.mCanKCastle = false;
        turn.mCanQCastle = false;

    } else if ( move == "OO" ) {

        // prevent castle through check, castle into check, etc.

        if ( isWhite && checkCastle(true, true) ) {

            turn.setKing ( moveHelper(*turn.getKing(), 0, 3, 0, 1) );
            turn.setRooks ( moveHelper(*turn.getRooks(), 0, 0, 0, 2) );

            mEnPassantLoc = -1;
            turn.mCanKCastle = false;
            turn.mCanQCastle = false;
            
        } else if ( !isWhite && checkCastle(false, true) ) {
            
            turn.setKing ( moveHelper(*turn.getKing(), 7, 3, 7, 1) );
            turn.setRooks ( moveHelper(*turn.getRooks(), 7, 0, 7, 2) );

            mEnPassantLoc=-1;
            turn.mCanKCastle = false;
            turn.mCanQCastle = false;

        } else {
            throw std::runtime_error("unable to castle kingside");
        }

    } else if ( move == "OOO" ) {
        
        // prevent castle through check, castle into check, etc.

        if ( isWhite && checkCastle(true, false) ) {

            turn.setKing ( moveHelper(*turn.getKing(), 0, 3, 0, 5) );
            turn.setRooks ( moveHelper(*turn.getRooks(), 0, 7, 0, 4) );

            mEnPassantLoc=-1;
            turn.mCanKCastle = false;
            turn.mCanQCastle = false;
            
        } else if ( !isWhite && checkCastle(false, false) ) {
            
            turn.setKing ( moveHelper(*turn.getKing(), 7, 3, 7, 5) );
            turn.setRooks ( moveHelper(*turn.getRooks(), 7, 7, 7, 4) );

            mEnPassantLoc=-1;
            turn.mCanKCastle = false;
            turn.mCanQCastle = false;

        } else {
            throw std::runtime_error("unable to castle queenside");
        }

    }  else if ( piece >= 'a' && piece <= 'h' ) {
        
        // Pawn moves, captures, promotions, en passant
        int prevEnPass = mEnPassantLoc;
        auto cord = coordinateParser(move);
        mEnPassantLoc = -1;
        
        // Check for promotion
        size_t promoPos = move.find('=');
        bool isPromotion = (promoPos != std::string::npos);
        char promoPiece = isPromotion ? move[promoPos + 1] : '\0';
        
        if ( move.find('x') != std::string::npos ) {

            // check en passant
            if ( cord[2]*8 + cord[3] == prevEnPass ){

                // Remove the captured pawn from its actual position
                // The captured pawn is on the same file as target, same rank as attacker
                int capPawnRow = cord[0];  // Same row as attacking pawn
                int capPawnCol = cord[3];  // Same column as target square
                
                if (isWhite) {
                    mBlack.setPawns(removeHelper(*mBlack.getPawns(), capPawnRow, capPawnCol));
                } else {
                    mWhite.setPawns(removeHelper(*mWhite.getPawns(), capPawnRow, capPawnCol));
                }
                
                // Move the attacker to the target
                turn.setPawns(moveHelper(*turn.getPawns(), cord[0], cord[1], cord[2], cord[3]));

            } else {
                // Normal capture
                if ( !capturePiece(turn, other, cord[0], cord[1], cord[2], cord[3]) ) 
                    throw std::runtime_error("Illegal pawn capture");
            }
            mHalfMoves = 0;  // Reset halfmove clock on any pawn capture
            
        } else {
            
            // Check if pawn moved two squares
            int rowDiff = abs(cord[2] - cord[0]);
            if (rowDiff == 2) {
                // Set enpassant target square
                if (isWhite) {
                    mEnPassantLoc = (cord[0] + 1) * 8 + cord[1];  // One square forward from start
                } else {
                    mEnPassantLoc = (cord[0] - 1) * 8 + cord[1];  // One square back from start
                }
            }
            
            turn.setPawns( moveHelper(*turn.getPawns(), cord[0], cord[1], cord[2], cord[3]) );
            mHalfMoves = 0;  // Reset halfmove clock on any pawn move
        }
        
        // promotions
        if ( isPromotion ) {
            // Remove pawn from promotion square
            turn.setPawns( removeHelper(*turn.getPawns(), cord[2], cord[3]) );
            
            // Add promoted piece
            switch(promoPiece) {
                case 'Q':
                    turn.setQueens( *turn.getQueens() | (1ULL << (cord[2]*8 + cord[3])) );
                    break;
                case 'R':
                    turn.setRooks( *turn.getRooks() | (1ULL << (cord[2]*8 + cord[3])) );
                    break;
                case 'B':
                    turn.setBishops( *turn.getBishops() | (1ULL << (cord[2]*8 + cord[3])) );
                    break;
                case 'N':
                    turn.setKnights( *turn.getKnights() | (1ULL << (cord[2]*8 + cord[3])) );
                    break;
                default:
                    throw std::runtime_error("Invalid promotion piece");
            }
        }
        
    } else {
        throw std::runtime_error("Invalid move notation");
    }
    
    // Update turn and fullmove counter for all move types
    if (!isWhite) {
        ++mFullMoves;
    }
    mTurn = !isWhite;

}

bool MainBoard::checkCastle(bool isWhite, bool kingSide) {
    /* 
    First check if any pieces are blocking the path of castling and if the king is in check.
    Then check if the king appears on the vision of any piece, on the castling squares. 
    */
    if ( isWhite ) {

        if ( kingSide && mWhite.mCanKCastle ) {

            // *mWhite.getRooks()&1ULL to ensure that rooks exist
            if ( !( *mWhite.getRooks() & 0x1ULL ) ) {
                mWhite.mCanKCastle = false;
                return false;
            }

            auto board = wholeBoard();
            
            // 0x6 = 0000 0110
            if ( !verifyCheck(true) && !(board&0x6ULL ) ) {

                // Check if opp.'s knight has vision on castling squares.
                // we achieve this by assuming white knights are present on the squares (since opp. is black)
                auto k1 = knightMoves(0, 1, true);
                auto k2 = knightMoves(0, 2, true);

                for ( const auto &i : k1 ) {
                    if ( i!=-1 && *mBlack.getKnights() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : k2 ) {
                    if ( i!=-1 && *mBlack.getKnights() & (1ULL<<i) )
                        return false;
                }

                // check if opp.'s bishop has vision on castling squares.
                // we achieve this by assuming white bishops are present on the squares (since opp. is black)
                auto b1 = bishopMoves(0, 1, true);
                auto b2 = bishopMoves(0, 2, true);

                for ( const auto &i : b1 ) {
                    if ( i!=-1 && *mBlack.getBishops() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : b2 ) {
                    if ( i!=-1 && *mBlack.getBishops() & (1ULL<<i) )
                        return false;
                }

                // check if opp.'s rook has vision on castling squares.
                // we achieve this by assuming white rooks are present on the squares (since opp. is black)
                auto r1 = rookMoves(0, 1, true);
                auto r2 = rookMoves(0, 2, true);

                for ( const auto &i : r1 ) {
                    if ( i!=-1 && *mBlack.getRooks() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : r2 ) {
                    if ( i!=-1 && *mBlack.getRooks() & (1ULL<<i) )
                        return false;
                }

                // check if opp.'s queen has vision on castling squares.
                // we achieve this by assuming white queens are present on the squares (since opp. is black)
                auto q1 = queenMoves(0, 1, true);
                auto q2 = queenMoves(0, 2, true);
                
                for ( const auto &i : q1 ) {
                    if ( i!=-1 && *mBlack.getQueens() & (1ULL<<i) )
                    return false;
                }
                for ( const auto &i : q2 ) {
                    if ( i!=-1 && *mBlack.getQueens() & (1ULL<<i) )
                    return false;
                }
                
                // check if opp.'s pawn has vision on castling squares.
                // we achieve this by assuming white pawns are present on the squares (since opp. is black)
                auto p1 = pawnMoves(0, 1, true);
                auto p2 = pawnMoves(0, 2, true);

                for ( const auto &i : p1 ) {
                    if ( i!=-1 && *mBlack.getPawns() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : p2 ) {
                    if ( i!=-1 && *mBlack.getPawns() & (1ULL<<i) )
                        return false;
                }

                return true;

            } else return false;
            
        } else if ( !kingSide && mWhite.mCanQCastle ) {

            // to ensure that rooks exist
            if ( !( *mWhite.getRooks() & 0x80ULL ) ) {
                mWhite.mCanQCastle = false;
                return false;
            }
            
            auto board = wholeBoard();
            
            // 0x70 = 01110 0000
            if ( !verifyCheck(true) && !(board&0x70ULL) ) {
                
                auto k1 = knightMoves(0, 4, true);
                auto k2 = knightMoves(0, 5, true);

                for ( const auto &i : k1 ) {
                    if ( i!=-1 && *mBlack.getKnights() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : k2 ) {
                    if ( i!=-1 && *mBlack.getKnights() & (1ULL<<i) )
                        return false;
                }

                auto b1 = bishopMoves(0, 4, true);
                auto b2 = bishopMoves(0, 5, true);

                for ( const auto &i : b1 ) {
                    if ( i!=-1 && *mBlack.getBishops() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : b2 ) {
                    if ( i!=-1 && *mBlack.getBishops() & (1ULL<<i) )
                        return false;
                }

                auto r1 = rookMoves(0, 4, true);
                auto r2 = rookMoves(0, 5, true);

                for ( const auto &i : r1 ) {
                    if ( i!=-1 && *mBlack.getRooks() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : r2 ) {
                    if ( i!=-1 && *mBlack.getRooks() & (1ULL<<i) )
                        return false;
                }

                auto q1 = queenMoves(0, 4, true);
                auto q2 = queenMoves(0, 5, true);
                
                for ( const auto &i : q1 ) {
                    if ( i!=-1 && *mBlack.getQueens() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : q2 ) {
                    if ( i!=-1 && *mBlack.getQueens() & (1ULL<<i) )
                        return false;
                }

                auto p1 = pawnMoves(0, 4, true);
                auto p2 = pawnMoves(0, 5, true);

                for ( const auto &i : p1 ) {
                    if ( i!=-1 && *mBlack.getPawns() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : p2 ) {
                    if ( i!=-1 && *mBlack.getPawns() & (1ULL<<i) )
                        return false;
                }
                
                return true;

            } else return false;
            
        } else {
            return false;
        }

    } else {

        if ( kingSide && mBlack.mCanKCastle ) {

            // to ensure that rooks exist
            if ( !( *mBlack.getRooks() & 0x100000000000000ULL ) ) {
                mBlack.mCanKCastle = false;
                return false;
            }

            auto board = wholeBoard();

            if ( !verifyCheck(false) && !(board&0x600000000000000ULL) ) {
                
                auto k1 = knightMoves(7, 1, false);
                auto k2 = knightMoves(7, 2, false);

                for ( const auto &i : k1 ) {
                    if ( i!=-1 && *mWhite.getKnights() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : k2 ) {
                    if ( i!=-1 && *mWhite.getKnights() & (1ULL<<i) )
                        return false;
                }

                auto b1 = bishopMoves(7, 1, false);
                auto b2 = bishopMoves(7, 2, false);

                for ( const auto &i : b1 ) {
                    if ( i!=-1 && *mWhite.getBishops() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : b2 ) {
                    if ( i!=-1 && *mWhite.getBishops() & (1ULL<<i) )
                        return false;
                }

                auto r1 = rookMoves(7, 1, false);
                auto r2 = rookMoves(7, 2, false);

                for ( const auto &i : r1 ) {
                    if ( i!=-1 && *mWhite.getRooks() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : r2 ) {
                    if ( i!=-1 && *mWhite.getRooks() & (1ULL<<i) )
                        return false;
                }

                auto q1 = queenMoves(7, 1, false);
                auto q2 = queenMoves(7, 2, false);
                
                for ( const auto &i : q1 ) {
                    if ( i!=-1 && *mWhite.getQueens() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : q2 ) {
                    if ( i!=-1 && *mWhite.getQueens() & (1ULL<<i) )
                        return false;
                }

                auto p1 = pawnMoves(7, 1, false);
                auto p2 = pawnMoves(7, 2, false);

                for ( const auto &i : p1 ) {
                    if ( i!=-1 && *mWhite.getPawns() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : p2 ) {
                    if ( i!=-1 && *mWhite.getPawns() & (1ULL<<i) )
                        return false;
                }

                return true;

            } else return false;
            
        } else if ( !kingSide && mBlack.mCanQCastle ) {

            // to ensure that rooks exist
            if ( !( *mBlack.getRooks() & 0x8000000000000000ULL ) ) {
                mBlack.mCanQCastle = false;
                return false;
            }
            
            auto board = wholeBoard();

            if ( !verifyCheck(false) && !(board&0x7000000000000000ULL) ) {
                
                auto k1 = knightMoves(7, 4, false);
                auto k2 = knightMoves(7, 5, false);

                for ( const auto &i : k1 ) {
                    if ( i!=-1 && *mWhite.getKnights() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : k2 ) {
                    if ( i!=-1 && *mWhite.getKnights() & (1ULL<<i) )
                        return false;
                }

                auto b1 = bishopMoves(7, 4, false);
                auto b2 = bishopMoves(7, 5, false);

                for ( const auto &i : b1 ) {
                    if ( i!=-1 && *mWhite.getBishops() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : b2 ) {
                    if ( i!=-1 && *mWhite.getBishops() & (1ULL<<i) )
                        return false;
                }

                auto r1 = rookMoves(7, 4, false);
                auto r2 = rookMoves(7, 5, false);

                for ( const auto &i : r1 ) {
                    if ( i!=-1 && *mWhite.getRooks() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : r2 ) {
                    if ( i!=-1 && *mWhite.getRooks() & (1ULL<<i) )
                        return false;
                }

                auto q1 = queenMoves(7, 4, false);
                auto q2 = queenMoves(7, 5, false);
                
                for ( const auto &i : q1 ) {
                    if ( i!=-1 && *mWhite.getQueens() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : q2 ) {
                    if ( i!=-1 && *mWhite.getQueens() & (1ULL<<i) )
                        return false;
                }

                auto p1 = pawnMoves(7, 4, false);
                auto p2 = pawnMoves(7, 5, false);

                for ( const auto &i : p1 ) {
                    if ( i!=-1 && *mWhite.getPawns() & (1ULL<<i) )
                        return false;
                }
                for ( const auto &i : p2 ) {
                    if ( i!=-1 && *mWhite.getPawns() & (1ULL<<i) )
                        return false;
                }
                
                return true;

            } else return false;
            
        } else {
            return false;
        }

    }

}

#pragma region "Piece Legal Moves"
std::array<int,8> MainBoard::knightMoves(int row, int col, bool isWhite){

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

    for ( auto &i : positions ) {
        if (isWhite) {
            if ( i!=-1 && mWhite.getAll() & (1ULL << i) )
                i=-1;
        } else {
            if ( i!=-1 && mBlack.getAll() & (1ULL << i) )
                i=-1;
        }
    }

    return positions;
}

std::array<int,13> MainBoard::bishopMoves(int row, int col, bool isWhite){
    std::array<int, 13> out;
    out.fill(-1); 

    int index = 0;
    
    // NW, NE, SW, SE
    int directions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    
    for (auto &dir : directions) {
        for (int dist = 1; dist <= 7; dist++) {
            int nr = row + dir[0] * dist;
            int nc = col + dir[1] * dist;
            
            // Edge check
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8)
                break;

            auto t = nr*8 + nc;
            uint64_t occ = wholeBoard();

            if (occ & (1ULL<<t)) { 
                if ((isWhite && (mWhite.getAll() & (1ULL<<t))) || (!isWhite && (mBlack.getAll() & (1ULL<<t))))
                     break; 
                out[index++] = t;
                break; 
            } else {
                out[index++] = t;
            }

        }
    }
    return out;
}

std::array<int,14> MainBoard::rookMoves(int row, int col, bool isWhite){

    std::array<int, 14> out;
    out.fill(-1); 

    int index = 0;
    
    // N, S, W, E
    int directions[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    
    for (auto &dir : directions) {
        for (int dist = 1; dist <= 7; dist++) {
            int nr = row + dir[0] * dist;
            int nc = col + dir[1] * dist;
            
            // Edge check
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8)
                break;

            auto t = nr*8 + nc;
            uint64_t occ = wholeBoard();

            if (occ & (1ULL<<t)) { 
                if ((isWhite && (mWhite.getAll() & (1ULL<<t))) || (!isWhite && (mBlack.getAll() & (1ULL<<t))))
                     break; 
                out[index++] = t;
                break; 
            } else {
                out[index++] = t;
            }
        }
    }
    
    return out;
}

std::array<int,8> MainBoard::kingMoves(int row, int col, bool isWhite){

    std::array<int,8> out;
    out.fill(-1);
    
    int index = 0;

    // N, NW, NE, W, E, S, SW, SE
    int directions[8][2] = { {1,0}, {1,1}, {1,-1}, {0,1}, {0,-1}, {-1,0}, {-1,1}, {-1,-1} };

    for ( auto &dir : directions ) {

        int nr = row+dir[0];
        int nc = dir[1]+col;
        
        if ( nr < 0 || nr >= 8 || nc < 0 || nc >= 8 )
            continue;
        
        auto t = nr*8 + nc;
        if (isWhite) {
            if ( mWhite.getAll() & (1ULL << t) )
                continue;
        } else {
            if ( mBlack.getAll() & (1ULL << t) )
                continue;
        }
        
        out[index++] = t;
    }

    return out;
}

std::array<int,27> MainBoard::queenMoves(int row, int col, bool isWhite){
    
    std::array<int,27> out;
    out.fill(-1);
    
    int index = 0;
    
    // N, NW, NE, W, E, S, SW, SE
    int directions[8][2] = { {1,0}, {1,1}, {1,-1}, {0,1}, {0,-1}, {-1,0}, {-1,1}, {-1,-1} };
    
    for (auto &dir : directions) {
        for (int dist = 1; dist <= 7; dist++) {

            int nr = row + dir[0] * dist;
            int nc = col + dir[1] * dist;
            
            // Edge check
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8)
                break;
            
            auto t = nr*8 + nc;
            uint64_t occ = wholeBoard();

            if (occ & (1ULL<<t)) { 
                if ((isWhite && (mWhite.getAll() & (1ULL<<t))) || (!isWhite && (mBlack.getAll() & (1ULL<<t))))
                     break; 
                out[index++] = t;
                break; 
            } else {
                out[index++] = t;
            }
        }
    }
    
    return out;
}

std::array<int,4> MainBoard::pawnMoves(int row, int col, bool isWhite){
    
    std::array<int,4> out;
    out.fill(-1);
    
    int index = 0;
    auto board = wholeBoard();
    
    if (isWhite) {
        
        // white pawn moves
        if (row + 1 < 8) {
            int t = (row + 1) * 8 + col;
            if (!(board & (1ULL << t))) {  
                out[index++] = t;
                
                if (row == 1 && index > 0) {
                    int t2 = (row + 2) * 8 + col;
                    if (!(board & (1ULL << t2))) {
                        out[index++] = t2;
                    }
                }
            }
        }
        
        // Diagonal captures (with en passant)
        if (row + 1 < 8) {
            if (col - 1 >= 0) {
                int t = (row + 1) * 8 + (col - 1);
                if ((mBlack.getAll() & (1ULL << t)) || t == mEnPassantLoc) { 
                    out[index++] = t;
                }
            }
            if (col + 1 < 8) {
                int t = (row + 1) * 8 + (col + 1);
                if ((mBlack.getAll() & (1ULL << t)) || t == mEnPassantLoc) { 
                    out[index++] = t;
                }
            }
        }
        
    } else {
        // Black moves DOWN the board
        
        // Black pawn moves
        if (row - 1 >= 0) {
            int t = (row - 1) * 8 + col;
            if (!(board & (1ULL << t))) {
                out[index++] = t;
                
                if (row == 6 && index > 0) {
                    int t2 = (row - 2) * 8 + col;
                    if (!(board & (1ULL << t2))) {
                        out[index++] = t2;
                    }
                }
            }
        }
        
        // Diagonal captures (with en passant)
        if (row - 1 >= 0) {
            if (col - 1 >= 0) {
                int t = (row - 1) * 8 + (col - 1);
                if ((mWhite.getAll() & (1ULL << t)) || t == mEnPassantLoc) { 
                    out[index++] = t;
                }
            }
            if (col + 1 < 8) {
                int t = (row - 1) * 8 + (col + 1);
                if ((mWhite.getAll() & (1ULL << t)) || t == mEnPassantLoc) { 
                    out[index++] = t;
                }
            }
        }
    }
    
    return out;
}

#pragma endregion

int MainBoard::kingCoord(bool isWhite){

    int out=0;
    
    if ( isWhite ) {
        auto t_ = *mWhite.getKing();
        while ( t_&1ULL != 0 ){
            t_ >>= 1;
            ++out;
        }
    } else {
        auto t_ = *mBlack.getKing();
        while ( t_&1ULL != 0 ){
            t_ >>= 1;
            ++out;
        }
    }

    return out;
}

std::string MainBoard::generateFEN() {
    std::string fen;
    
    // Generate board position (iterate from rank 7 down to rank 0, left to right)
    for (int row = 7; row >= 0; --row) {
        int emptyCount = 0;
        
        for (int col = 7; col >= 0; --col) {
            int index = row * 8 + col;
            uint64_t mask = 1ULL << index;
            char piece = ' ';
            
            // Check white pieces
            if (*mWhite.getKing() & mask) piece = 'K';
            else if (*mWhite.getQueens() & mask) piece = 'Q';
            else if (*mWhite.getRooks() & mask) piece = 'R';
            else if (*mWhite.getBishops() & mask) piece = 'B';
            else if (*mWhite.getKnights() & mask) piece = 'N';
            else if (*mWhite.getPawns() & mask) piece = 'P';
            // Check black pieces
            else if (*mBlack.getKing() & mask) piece = 'k';
            else if (*mBlack.getQueens() & mask) piece = 'q';
            else if (*mBlack.getRooks() & mask) piece = 'r';
            else if (*mBlack.getBishops() & mask) piece = 'b';
            else if (*mBlack.getKnights() & mask) piece = 'n';
            else if (*mBlack.getPawns() & mask) piece = 'p';
            
            if (piece != ' ') {
                // Flush empty count if any
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += piece;
            } else {
                emptyCount++;
            }
        }
        
        // Flush remaining empty count
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        
        // Add rank separator (except after last rank)
        if (row > 0) {
            fen += '/';
        }
    }
    
    // Active color
    fen += ' ';
    fen += (mTurn) ? 'w' : 'b';
    
    // Castling rights
    fen += ' ';
    bool hasCastling = false;
    if (mWhite.mCanKCastle) { fen += 'K'; hasCastling = true; }
    if (mWhite.mCanQCastle) { fen += 'Q'; hasCastling = true; }
    if (mBlack.mCanKCastle) { fen += 'k'; hasCastling = true; }
    if (mBlack.mCanQCastle) { fen += 'q'; hasCastling = true; }
    if (!hasCastling) { fen += '-'; }
    
    // En passant target square
    fen += ' ';
    if (mEnPassantLoc == -1) {
        fen += '-';
    } else {
        int col = mEnPassantLoc % 8;
        int row = mEnPassantLoc / 8;
        fen += (char)('h' - col);
        fen += (char)('1' + row);
    }
    
    // Halfmove clock
    fen += std::string(" ") + std::to_string(mHalfMoves);
    // Fullmove number
    fen += std::string(" ") + std::to_string(mFullMoves);
    
    return fen;
}