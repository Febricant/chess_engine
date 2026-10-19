#include <cstdint>
#include <array>
#include <string>
#include "encoding.hpp"

bool is_black(const std::array<std::uint8_t, 120> &array, int index) {
    if(black_conversion & array[index]) {
        return true;
    }
    return false;
}

bool is_piece(const std::array<std::uint8_t, 120> &array, int index) {
    if(array[index] > static_cast<int>(pieces::empty)) {
        return true;
    }
    return false;
}

bool is_white(const std::array<std::uint8_t, 120> &array, int index) {
    if(is_piece(array, index) && !is_black(array, index)){
        return true;
    }
    return false;
}

int piece_type(const std::array<std::uint8_t, 120> &array, int index) {
    int piece;
    piece = (array[index] & ~black_conversion);
    return piece;

}


