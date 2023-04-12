#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

void Image::fill(Color color) {
    memset(buffer, color.data, width * height * sizeof(Color));
}

void Image::setPixel(int x, int y, Color color) {
    buffer[width * y + x] = color;
}

void Image::copyBuffer(const Image& src) {
    assert(width == src.width);
    assert(height == src.height);
    memcpy(buffer, src.buffer, width * height * sizeof(Color));
}

Image Image::load(const char* filepath) {
    int width, height, comp;
    
    // Check for any errors regarding the image
    if (stbi_info(filepath, &width, &height, &comp) != 1) {
        char buf[64];
        snprintf(buf, 64, "Invalid file format: %s", filepath);
        throw CINQ_IMAGE_EXCEPT(buf);
    } if (comp < 3) {
        throw CINQ_IMAGE_EXCEPT("Unsupported color format");
    }

    // Decode image                                   force RGBA ~|~
    uint8_t* data = stbi_load(filepath, &width, &height, nullptr, 4);
    Color* pixels = new Color[width * height];

    // Fill color buffer with decoded data
    int dataIndex = 0;
    for (int i = 0; i < width * height; i++) {
        Color currentPixel(
            data[dataIndex + 0],
            data[dataIndex + 1],
            data[dataIndex + 2],
            data[dataIndex + 3]
        );
        pixels[i] = currentPixel;

        dataIndex += 4;
    }

    stbi_image_free(data);
    return Image(width, height, pixels, comp);
}

void Image::save(const char* filepath) {
    // STB expect ABGR component order
    Color* imgData = getBufferABGR();
    switch (getImageType(filepath)) {
        case ImageType::PNG:
            stbi_write_png(filepath, width, height, comp, imgData, 0);
            break;
        case ImageType::JPEG:
            stbi_write_jpg(filepath, width, height, comp, imgData, 95);
            break;
        case ImageType::TGA:
            stbi_write_tga(filepath, width, height, comp, imgData);
            break;
        case ImageType::BMP:
            stbi_write_bmp(filepath, width, height, comp, imgData);
            break;
    }
    delete[] imgData;
}

Image::ImageType Image::getImageType(const char* filename) {
    const char* ext = filename + strcspn(filename, ".");
    if (strcmp(ext, ".png") == 0)
        return ImageType::PNG;
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
        return ImageType::JPEG;
    else if (strcmp(ext, ".tga") == 0)
        return ImageType::TGA;
    else if (strcmp(ext, ".bmp") == 0)
        return ImageType::BMP;

    return ImageType::PNG;
}

Color* Image::getBufferABGR() {
    Color* outBuffer = new Color[width * height];
    for (int i = 0; i < width * height; i++)
        outBuffer[i] = Color(buffer[i].abgr());

    return outBuffer;
}

// Exceptions
const char* Image::Exception::what() const {
    char buf[512];
    sprintf(buf, "%s\n\n[Message]\n%s", CinqException::what(), getMessage().c_str());
    whatBuffer = std::string(buf);
    return whatBuffer.c_str();
}