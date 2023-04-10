#include "Image.hpp"

void Image::fill(Color color) {
    memset(buffer, color.data, width * height * sizeof(Color));
}

void Image::setPixel(int x, int y, Color color) {
    buffer[width * y + x] = color;
}

Image Image::load(const char* filepath) {

}

void Image::save(const char* filepath) {
    
}