#pragma once

#include <string>
#include <memory>
#include <cassert>

#include "Color.hpp"
#include "Logger.hpp"
#include "Window/Exception.hpp"

class Image {
public:
    // Still can't get myself to setup the pipeline error handling for some reason :()
    class Exception : public CinqException {
    public:
        Exception(int line, const char* file, std::string msg) : CinqException(line, file), msg(std::move(msg)) {
            Logger::error(std::format("[Cinq Image Exception] {} thrown in {}", getType(), getOriginString()));
        }
        const char* what() const override;
        const char* getType() const override { return "Cinq Image Exception"; }
        const std::string& getMessage() const { return msg; }
    private:
        std::string msg;
    };

    enum ImageType {
        PNG = 0,
        JPEG,
        TGA,
        BMP
    };
public:
    Image(int width, int height, Color* buffer, int comp = 4) : width(width), height(height), buffer(buffer), comp(comp) {}
    Image(int width, int height, int comp = 4) : width(width), height(height), buffer(new Color[width * height]), comp(comp) {}

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
    Color* getBufferABGR();
    static Image load(const char* filepath);
    static ImageType getImageType(const char* filename);
    void save(const char* filepath);
    void copyBuffer(const Image& src);
    const char* filepath = "";
private:
    Color* buffer;
    // Comp is the number of components in the pixels
    // of the ORIGINAL image, not the one that's in memory
    int width, height, comp;
};

#define CINQ_IMAGE_EXCEPT(msg) \
    Image::Exception(__LINE__, __FILE__, msg)