#include <iostream>
#include <cstdint>
#include "encoding.h"









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

int encoding(std::int8_t a) {
    
    switch(std::int8_t a) {
        case 2:
            return 2;
        case 7:
            return 7;
        case 10:
            return 10;
        case 11:
            return 11;
        case 12:
            return 12;
        case 13:
            return 13;
        case 14:
            return 14;
        case 15:
            return 15;
    }
    return -1; // Return -1 for invalid inputs
}