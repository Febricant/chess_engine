#include <iostream>
#include <cstdint>
#include <format>
#include <array>
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

std::array<std::uint8_t, 120> position_board() {
    //Position board incidices
    std::array<std::uint8_t, 120> board_120 = {};
    for(std::uint8_t i = 0; i < 120; i++) {
        board_120[i] = i;
    }

    //position board printing
    for(std::uint8_t z = 0; z < 12; z++) {
        for(std::uint8_t i = 0; i < 10; i++) {
            std::cout << std::format("{:3} ", board_120[z * 10 + i]) << " ";
        }
        std::cout << "\n";
    }
    return board_120;
}

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

    //piece board printing
    for(std::uint8_t z = 0; z < 12; z++) {
        for(std::uint8_t i = 0; i < 10; i++) {
            std::cout << std::format("{:3} ", piece_board[z * 10 + i]) << " ";
        }
        std::cout << "\n";
    }

    return piece_board;
}

void square_encoding() {
    for(char i = 'a'; i < 'i'; i++) {
        for(std::uint8_t j = 1; j < 9; j++) {
            std::cout << std::format("{}{}: {}\n", i, j, (j * 10 + (i - 'a' + 1)));
        }
    }
}

void board_120_to_64(std::array<std::uint8_t, 120> board_120) {
    for(std::uint8_t i = 21; i < 99; i++) {
        if(i % 10 != 0 && i % 10 != 9) {
            std::cout << std::format("{:2} ", board_120[i]) << " ";
            if(i % 10 == 8) {
                std::cout << "\n";
            }
        }
    }
}