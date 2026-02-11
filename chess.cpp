// Author: Sreeram S Nair (github: realMantaflow)

#include <iostream>
#include <bitset>
#include "MainBoard.hh"
#include "FENParser.hh"

using namespace std;
int main () {
    MainBoard chessBoard;
    // cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    
    // chessBoard.makeMove("Ng1f3", true);
    // cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    
    // chessBoard.makeMove("Ng8f6", false);
    // cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    
    // chessBoard.makeMove("e2e4", true);
    // cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    
    // chessBoard.makeMove("Nb8c6", false);
    // cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    
    // cout << chessBoard.generateFEN() << endl;
    
    FENParser::applyFEN(chessBoard, "r1bqkb1r/pppp1ppp/2n2n2/8/4P3/2N5/PPP2PPP/R1BQKBNR b KQkq - 4 5");
    cout << std::bitset<64>(chessBoard.wholeBoard()) << endl;
    cout << chessBoard.generateFEN() << endl;


}