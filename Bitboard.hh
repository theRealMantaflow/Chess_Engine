// Author: Sreeram S Nair (github: realMantaflow)

#pragma once

#include <cstdint>

class Bitboard {

    uint64_t mKing;
    uint64_t mPawns;
    uint64_t mRooks;
    uint64_t mQueens;
    uint64_t mKnights;
    uint64_t mBishops;
    
public:
    bool     mIsWhite;
    bool     mCanKCastle;
    bool     mCanQCastle;

    // Default state is white.
    Bitboard();
    Bitboard(bool, bool, bool);
    // Bitboard (bool, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

    // getters
    const uint64_t* getPawns();
    const uint64_t* getKnights();
    const uint64_t* getBishops();
    const uint64_t* getRooks();
    const uint64_t* getKing();
    const uint64_t* getQueens();
    uint64_t getAll();

    // setters
    void setPawns(uint64_t);
    void setKnights(uint64_t);
    void setBishops(uint64_t);
    void setRooks(uint64_t);
    void setKing(uint64_t);
    void setQueens(uint64_t);

};