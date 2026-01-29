// Author: Sreeram S Nair (github: realMantaflow)

#include <iostream>
#include <bitset>
#include <cstdint>
#include <string>
#include <array>

using namespace std;

/*
0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0

0   0   0   0   0   0   0   0
*/

std::array<int,8> knightMoves (int row, int col){

    std::array<int,8> positions = {
        ( row+1 <  8 && col+2 <  8 ) ? ((row+1)*8 + (col+2)) : -1, 
        ( row+1 <  8 && col-2 >= 0 ) ? ((row+1)*8 + (col-2)) : -1, 
        ( row+2 <  8 && col+1 <  8 ) ? ((row+2)*8 + (col+1)) : -1, 
        ( row+2 <  8 && col-1 >= 0 ) ? ((row+2)*8 + (col-1)) : -1, 
        ( row-1 >= 0 && col+2 <  8 ) ? ((row-1)*8 + (col+2)) : -1, 
        ( row-1 >= 0 && col-2 >= 0 ) ? ((row-1)*8 + (col-2)) : -1, 
        ( row-2 >= 0 && col+1 <  8 ) ? ((row-2)*8 + (col+1)) : -1, 
        ( row-2 >= 0 && col-1 >= 0 ) ? ((row-2)*8 + (col-1)) : -1
    };

    return positions;
}

uint64_t moveHelper(uint64_t bboard, std::string_view from, std::string_view towards) {
    int fcol = abs(from[0]-'h');
    int frow = from[1]-'1';
    int tcol = abs(towards[0]-'h');
    int trow = towards[1]-'1';
    std::cout << frow << ", " << fcol << "\n" << trow << ", " << tcol << std::endl;
    bboard |= uint64_t(0x0000000000000001) << (trow*8 + tcol);
    bboard ^= uint64_t(0x0000000000000001) << (frow*8 + fcol);
    return bboard;
}

int main() {

    // std::cout << std::string("Test").substr(0,2) << std::endl;
    // std::cout << std::string("Test").find('r') << std::endl;
    if (std::string("Test").find('t') < 4) {
        std::cout << "Found it" << std::endl;
    }
    // char piece = 'i';
    // if (piece >= 'a' && piece <= 'h')
    //     std::cout << "Hello here\n";
    // else
    //     std::cout << "Hello there\n";
    
    uint64_t a = uint64_t(1)<<62 | uint64_t(1)<<15;
    std::cout << std::bitset<64>(a) << "\n";
    a=moveHelper(a, std::string_view("b8"), std::string_view("g8"));
    std::cout << std::bitset<64>(a) << std::endl;

    auto kmove = knightMoves(3,4);

    for (auto& i : kmove) {
        if (i!=-1) {
            cout << (1ULL << i) << "\n";
            // cout << bitset<64>(1ULL << i) << "\n";
        } else {
            continue;
        }
        cout << i << "i val\n";
    }
    // std::cout << a;
    return 0;
}