#include <iostream>
#include <cstdint>
#include <array>
#include <algorithm>
#include "encoding.hpp"
#include "position.hpp"

/*
void position::move_check(std::array<std::uint8_t, 120> board) {
    auto [orig_pos, mod_pos] = std::mismatch(last_board.begin(), last_board.end(), board.begin());
    auto count1_last_move = std::count(last_board.begin(), last_board.end(), target);
    auto count1_this_move = std::count(board.begin(), board.end(), target);
}

void position::half_move_update(std::array<std::uint8_t, 120> board) {
    move_check(board);
    if(orig_pos != board.end()) {
        if(static_cast<int>(*orig_pos) != static_cast<int>(pieces::pawn) && count1_last_move == count1_this_move) {
            half_move += 1;
        }
    }
    last_board = board;
}
*/
/*class position {

    private:
        std::uint8_t target = 1;
        int half_move = 0;
        int full_move = 1;
        int en_passant = -1;
        std::uint8_t castle_rights = 0;
        bool move_side = true;
        std::array<std::uint8_t, 120> board;
    
    public:
        void start_pos();
};*/

void position::start_pos() {
    board = piece_board_initial();
    print_board_120_to_64(board);
    new_line();
    move_side();
    new_line();
    castling_rights();
    new_line();
    std::cout << "En passant: " << en_passant << "\n";
    std::cout << "Halfmove: " << half_move << "\n";
    std::cout << "Fullmove: " << full_move << "\n";
}

void position::move_side() {
    std::cout << "Side to move: ";
    if(full_move % 2 == 1) {
        white_move = true;
        std::cout << "white";
    } else {
        white_move = false;
        std::cout << "black";
    }
}

void position::castling_rights() {
    std::cout << "Side to move: "<< "KQkq";
}