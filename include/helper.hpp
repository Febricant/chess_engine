#pragma once
#include <cstdint>
#include <array>
#include <string>
#include "encoding.hpp"

constexpr std::uint8_t black_conversion = 0b0000'1000;

int is_black(const std::array<std::uint8_t, 120> array, int index);
int is_piece(const std::array<std::uint8_t, 120> array, int index);
int is_white(const std::array<std::uint8_t, 120> array, int index);
int piece_type(const std::array<std::uint8_t, 120> array, int index);