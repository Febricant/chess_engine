#include <iostream>
#include <cstdint>
#include <format>
#include <array>
#include <string>
#include "encoding.hpp"

/*
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 15| 12| 14| 13| 11| 14| 12| 15| 0 |
| 0 | 10| 10| 10| 10| 10| 10| 10| 10| 0 |
| 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 0 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 0 |
| 0 | 7 | 4 | 6 | 5 | 3 | 6 | 4 | 7 | 0 |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
*/

//initializes the positon board 0-119
std::array<std::uint8_t, 120> position_board_() {
    //Position board incidices
    std::array<std::uint8_t, 120> board_120 = {};
    for(std::uint8_t i = 0; i < 120; i++) {
        board_120[i] = i;
    }
    return board_120;
}

//prints boards that are 120 array indicies
void board_print_120(const std::array<std::uint8_t, 120> &board_120) {
    for(std::uint8_t z = 12; z > 0; z--) {
        for(std::uint8_t i = 0; i < 10; i++) {
            std::cout << std::format("{:3} ", board_120[(z-1) * 10 + i]) << " ";
        }
        std::cout << "\n";
    }
}

//initializes the piece board on indicies 0-119
std::array<std::uint8_t, 120> piece_board_initial() {
    //Piece board encoding vars
    std::array<std::uint8_t, 120> piece_board = {};
    constexpr std::uint8_t black_conversion = 0b0000'1000;

    //Piece board encoding indices
        //king
        piece_board[25] = static_cast<std::uint8_t>(pieces::king);
        piece_board[95] = (static_cast<std::uint8_t>(pieces::king) | black_conversion);
        
        //queen
        piece_board[24] = static_cast<std::uint8_t>(pieces::queen);
        piece_board[94] = (static_cast<std::uint8_t>(pieces::queen) | black_conversion);

        //bishops
        piece_board[23] = static_cast<std::uint8_t>(pieces::bishop);
        piece_board[26] = static_cast<std::uint8_t>(pieces::bishop);
        piece_board[93] = (static_cast<std::uint8_t>(pieces::bishop) | black_conversion);
        piece_board[96] = (static_cast<std::uint8_t>(pieces::bishop) | black_conversion);

        //knights
        piece_board[22] = static_cast<std::uint8_t>(pieces::knight);
        piece_board[27] = static_cast<std::uint8_t>(pieces::knight);
        piece_board[92] = (static_cast<std::uint8_t>(pieces::knight) | black_conversion);
        piece_board[97] = (static_cast<std::uint8_t>(pieces::knight) | black_conversion);

        //rooks
        piece_board[21] = static_cast<std::uint8_t>(pieces::rook);
        piece_board[28] = static_cast<std::uint8_t>(pieces::rook);
        piece_board[91] = (static_cast<std::uint8_t>(pieces::rook) | black_conversion);
        piece_board[98] = (static_cast<std::uint8_t>(pieces::rook) | black_conversion);

    for(std::uint8_t i = 0; i < 120; i++) {
        if(i < 21 || i > 98 || (i % 10 == 0) || (i % 10 == 9)) {
            piece_board[i] = static_cast<std::uint8_t>(pieces::off_board);
        } else if(i >= 31 && i <= 38) {
            piece_board[i] = static_cast<std::uint8_t>(pieces::pawn);
        } else if(i >= 81 && i <= 88) {
            piece_board[i] = (static_cast<std::uint8_t>(pieces::pawn) | black_conversion);
        } else if(i >= 41 && i <= 78){
            piece_board[i] = static_cast<std::uint8_t>(pieces::empty);
        }
    }
    return piece_board;
}
int is_black(const std::array<std::uint8_t, 120> array, int index) {
    if(0b0000'1000 & array[index]) {
        return 1;
    }
    return 0;
}


//initializes the square encoding board a1-h8
std::array<std::string, 64> square_encoding() {
    std::array<std::string, 64> squares = {};
    for(std::uint8_t j = 1; j < 9; j++) {
        for(char k = 'a'; k <= 'h'; k++) {
            squares[(static_cast<std::uint8_t >(j) * 8) - static_cast<std::uint8_t >(('h'- k) + 1)] = std::format("{}{}", k, j);
        }
    }
    return squares;
}

//gives square encoding of certain index
std::string square_index_encoding(const std::array<std::string, 64> &squares, std::uint8_t index) {
    if(index < 21 || index > 98 || index % 10 == 0 || index % 10 == 9) {
        return "-";
    }

    std::uint8_t subtract_factor = 21;
    std::uint8_t column_equalize = index / 10;
    column_equalize = (column_equalize - 2) * 2;

    return squares[index - (column_equalize + subtract_factor)];
}

int square_encoding_index(const std::string &coord) {
    //if not valid format return -1
    if((coord.length() != 2) || (coord[0] - 'a') < 0 || (coord[0] - 'a') >= 8 || (coord[1] - '1') < 0 || (coord[1] - '1') >= 8) {
        return -1;
    }
    int file = coord[0] - 'a';
    int rank = coord[1] - '1';
    int index_64 = file + (rank * 8);

    return index_64 + (21 + (rank * 2));
}

//prints a 120 board to its 64 counter part
void print_board_120_to_64(const std::array<std::uint8_t, 120> &board_120) {
    for(std::uint8_t j = 8; j > 0; j--) {
        for(std::uint8_t k = 0; k < 8; k++) {
            std::cout << std::format("{:2} ", board_120[j * 10 + k + 11]) << " ";
        }
        std::cout << "\n";
    }
}