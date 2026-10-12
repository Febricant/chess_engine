#include <iostream>
#include "encoding.hpp"


int main() {

    std::array<std::uint8_t, 120> position_board;
    std::array<std::uint8_t, 120> piece_board;

    std::cout << "Position Board 120:\n";
    position_board = position_board();
    std::cout << "\n";
    std::cout << "Posotion Board 64:\n";
    board_120_to_64(position_board);
    std::cout << "\n";

    std::cout << "Piece Board 120:\n";
    piece_board = piece_board_initial();
    std::cout << "\n";
    std::cout << "Piece Board 64:\n";
    board_120_to_64(piece_board);
    std::cout << "\n";

    std::cout << "Square Encoding:\n";
    square_encoding();
    return 0;   
    
}