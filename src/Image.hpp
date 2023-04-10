#pragma once

#include <string>
#include <memory>
#include <cassert>

#include "Color.hpp"
#include "Exception.hpp"

class Image {
public:
    // Still can't get myself to setup the pipeline error handling for some reason :()
    class Exception : public CinqException {
    public:
        Exception(int line, const char* file, std::string msg) : CinqException(line, file), msg(std::move(msg)) {}
        const char* what() const override;
        const char* getType() const override { return "Cinq Image Exception"; }
        const std::string& getMessage() const { return msg; }
    private:
        std::string msg;
    };
public:
    Image(int width, int height, Color* buffer) : width(width), height(height), buffer(buffer) {}
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
    void copyBuffer(const Image& src);
private:
    Color* buffer;
    int width, height;
};

#define CINQ_IMAGE_EXCEPT(msg) \
    Image::Exception(__LINE__, __FILE__, msg)