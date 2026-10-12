#include <iostream>
#include <cstdint>
#include <format>
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

enum class pieces : std::uint8_t {
        off_board = 0,
        empty = 1,
        pawn = 2,
        king = 3,
        knight = 4,
        queen = 5,
        bishop = 6,
        rook = 7
    };

int encoding() {
    //Position board incidices
    std::uint8_t board[120] = {};
    for(std::uint8_t i = 0; i < 120; i++) {
        board[i] = i;
    }

    //position board printing
    std::cout << "Position Board:\n";
    for(std::uint8_t z = 0; z < 12; z++) {
        for(std::uint8_t i = 0; i < 10; i++) {
            std::cout << std::format("{:3} ", board[z * 10 + i]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";

    //Piece board encoding vars
    std::uint8_t piece_board[120] = {};
    constexpr std::uint8_t black_conversion = 0b0000'1000;

    //Piece board encoding indices
    for(std::uint8_t i = 0; i < 120; i++) {
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

        if(i < 21 || i > 98 || (i % 10 == 0) || (i % 10 == 9)) {
            piece_board[i] = static_cast<std::uint8_t>(pieces::off_board);
        } else if(i >= 31 && i <= 38) {
            piece_board[i] = static_cast<std::uint8_t>(pieces::pawn);
        } else if(i >= 81 && i <= 88) {
            piece_board[i] = (static_cast<std::uint8_t>(pieces::pawn) | black_conversion);
        } else {
            piece_board[i] = static_cast<std::uint8_t>(pieces::empty);
        }
    }

    //piece board printing
    std::cout << "Encoding Board:\n";
    for(std::uint8_t z = 0; z < 12; z++) {
        for(std::uint8_t i = 0; i < 10; i++) {
            std::cout << std::format("{:3} ", piece_board[z * 10 + i]) << " ";
        }
        std::cout << "\n";
    }


    return 0;

}