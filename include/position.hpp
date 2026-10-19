#pragma once
#include <iostream>
#include <cstdint>
#include <array>

class position {

    private:
        int half_move = 0;
        int full_move = 1;
        int en_passant = -1;
        std::uint8_t castle_rights = 0;
        bool white_move;
        std::array<std::uint8_t, 120> board;
    
    public:
        void start_pos();
        void move_side();
        void castling_rights();
};