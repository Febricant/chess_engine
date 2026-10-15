#pragma once
#include <cstdint>
#include <array>
#include <string>

std::array<std::uint8_t, 120> position_board_();
std::array<std::uint8_t, 120> piece_board_initial();
void board_print_120(const std::array<std::uint8_t, 120> &board_120);
std::array<std::string, 64> square_encoding();
std::string square_encoding_index(const std::array<std::string, 64> &squares, std::uint8_t index);
void print_board_120_to_64(const std::array<std::uint8_t, 120> &board_120);

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