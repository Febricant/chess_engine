#pragma once
#include <cstdint>
#include <array>

std::array<std::uint8_t, 120> position_board();
std::array<std::uint8_t, 120> piece_board_initial();
void square_encoding();
void board_120_to_64(std::array<std::uint8_t, 120> board_120);

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