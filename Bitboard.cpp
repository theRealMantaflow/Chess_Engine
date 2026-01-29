// Author: Sreeram S Nair (github: realMantaflow)

#include "bitboard.hh"

#pragma region Constructors

Bitboard::Bitboard() : mIsWhite (true), mCanCastle(true) {

    mKing    = 0x8ULL;
    mPawns   = 0xFF00ULL;
    mRooks   = 0x81ULL;
    mQueens  = 0x100ULL;
    mBishops = 0x24ULL;
    mKnights = 0x42ULL;

}

Bitboard::Bitboard(bool isWhite, bool canCastle=true) : mIsWhite (isWhite), mCanCastle(canCastle) {

    if ( mIsWhite ) {
        mQueens     = 0x100ULL;
        mKing       = 0x8ULL;
        mRooks      = 0x81ULL;
        mKnights    = 0x42ULL;
        mBishops    = 0x24ULL;
        mPawns      = 0xFF00ULL;
    } else {
        mQueens     = 0x1000000000000000ULL;
        mKing       = 0x8000000000000000ULL;
        mRooks      = 0x8100000000000000ULL;
        mKnights    = 0x4200000000000000ULL;
        mBishops    = 0x2400000000000000ULL;
        mPawns      = 0xFF00000000000000ULL;
    }

}

#pragma endregion

#pragma region Getters

const uint64_t* Bitboard::getPawns() {
    return &mPawns;
}

const uint64_t* Bitboard::getKnights(){
    return &mKnights;
}

const uint64_t* Bitboard::getBishops(){
    return &mBishops;
}

const uint64_t* Bitboard::getRooks(){
    return &mRooks;
}

const uint64_t* Bitboard::getKing(){
    return &mKing;
}

const uint64_t* Bitboard::getQueens(){
    return &mQueens;
}

uint64_t Bitboard::getAll(){
    return mQueens | mKing | mPawns | mRooks | mKnights | mBishops;
}

#pragma endregion

#pragma region Setters

void Bitboard::setPawns(const uint64_t pawns){
    mPawns = pawns;
}

void Bitboard::setKnights(const uint64_t knights){
    mKnights = knights;
}

void Bitboard::setBishops(const uint64_t bishops){
    mBishops = bishops;
}

void Bitboard::setRooks(const uint64_t rooks){
    mRooks = rooks;
}

void Bitboard::setKing(const uint64_t king){
    mKing = king;
}

void Bitboard::setQueens(const uint64_t queens){
    mQueens = queens;
}

#pragma endregion