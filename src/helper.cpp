#include <cstdint>
#include <array>
#include <string>
#include "encoding.hpp"

int is_black(const std::array<std::uint8_t, 120> &array, int index) {
    if(black_conversion & array[index]) {
        return 1;
    }
    return 0;
}

int is_piece(const std::array<std::uint8_t, 120> &array, int index) {
    if(array[index] > static_cast<int>(pieces::empty)) {
        return 1;
    }
    return 0;
}

int is_white(const std::array<std::uint8_t, 120> &array, int index) {
    if(is_piece(array, index) && !is_black(array, index)){
        return 1;
    }
    return 0;
}

int piece_type(const std::array<std::uint8_t, 120> &array, int index) {
    int piece;
    piece = (array[index] & ~black_conversion);
    return piece;

}


