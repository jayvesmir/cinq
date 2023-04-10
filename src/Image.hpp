#pragma once

#include <memory>
#include "Color.hpp"

class Image {
public:
    Image(int width, int height) : width(width), height(height), buffer(new Color[width * height]) {}

    Image(Image&& src) : width(src.width), height(src.height), buffer(src.buffer) {}
    Image(const Image&) = delete;

    Image& operator=(Image&& src) {
        width = src.width;
        height = src.height;
        buffer = src.buffer;
        return *this;
    }
    Image& operator=(const Image&) = delete;

    ~Image() {
        delete[] buffer;
    }

    void fill(Color color);
    void setPixel(int x, int y, Color color);
    Color getPixel(int x, int y) const { return buffer[width * y + x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Color* getBuffer() const { return buffer; }
    static Image load(const char* filepath);
    void save(const char* filepath);
    void setBuffer(const Image& src);
private:
    Color* buffer;
    int width, height;
};