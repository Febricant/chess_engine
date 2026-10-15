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

    std::array<std::string, 64> square_board = square_encoding();
    std::cout << square_index_encoding(square_board, 68) << "\n";
    std::cout << square_encoding_index("h5") << "\n";

    std::cout << is_black(piece_board, 35);


    return 0;   
    
}