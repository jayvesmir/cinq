#pragma once

#include <cstdint>

// Stored as ARGB
// Constructors take RGBA
class Color {
public:
    uint32_t data;

    Color() : data() {}
    Color(const Color& src) : data(src.data) {}
    Color(uint32_t color) : data(color) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : data((a << 24) | (r << 16) | (g << 8) | (b << 0)) {}
    Color(Color src, uint8_t a) : data((a << 24) | src.data) {}
    Color& operator=(Color src) {
        data = src.data;
        return *this;
    }

    uint8_t r() const { return (data >> 16) & 0xff; }
    uint8_t g() const { return (data >>  8) & 0xff; }
    uint8_t b() const { return (data)       & 0xff; }
    uint8_t a() const { return (data >> 24); }
    uint32_t abgr() const { return ((a() << 24) | (b() << 16) | (g() << 8) | (r() << 0)); }
};