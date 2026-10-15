#include <iostream>
#include "encoding.hpp"


int main() {

    std::array<std::uint8_t, 120> position_board = position_board_();
    std::cout << "Position Board 120:\n";
    board_print_120(position_board);
    std::cout << "\n";
    std::cout << "Position Board 64:\n";
    print_board_120_to_64(position_board);
    std::cout << "\n";

    std::array<std::uint8_t, 120> piece_board = piece_board_initial();
    std::cout << "Piece Board 120:\n";
    board_print_120(piece_board);
    std::cout << "\n";
    std::cout << "Piece Board 64:\n";
    print_board_120_to_64(piece_board);
    std::cout << "\n";

    std::cout << square_encoding(71);


    return 0;   
    
}