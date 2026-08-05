#ifndef UTILS_H
#define UTILS_H
#include <cstdint>

namespace vimcube::utility {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        float a;

        Color(uint8_t r, uint8_t g, uint8_t b, float a) : r(r), g(g), b(b), a(a) {};
    };
}
#endif