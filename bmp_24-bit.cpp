#include "bmp_24-bit.h"
#include <iostream>
#include <cstddef>

BMP_24bit::BMP_24bit(const std::string &filename) : BMP(filename) {
    if (infoHeader.biBitCount != 24) {
        std::cerr << "BMP_24bit: This is not a 24-bit BMP file." << std::endl;
        std::exit(1);
    }

    std::ifstream f(filename, std::ios::binary);

    f.seekg(fileHeader.bfOffBits); // Seek to pixel array

    const size_t &padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

    //Read image data into img
    img.resize(infoHeader.biWidth * std::abs(infoHeader.biHeight) * pixel_size);
    if (infoHeader.biHeight < 0) {
        for (int32_t y = 0; y < -infoHeader.biHeight; ++y) {
            // Read row
            f.read(reinterpret_cast<char *>(&img[getInternalIndex(0, y)]), pixel_size * infoHeader.biWidth);

            f.seekg(padSize, std::ios::cur);
        }
    } else {
        for (int32_t y = infoHeader.biHeight - 1; y >= 0; --y) {
            // Read row
            f.read(reinterpret_cast<char *>(&img[getInternalIndex(0, y)]), pixel_size * infoHeader.biWidth);

            f.seekg(padSize, std::ios::cur);
        }
    }

    f.close();
}

BMP_24bit::BMP_24bit(const int32_t &w, const int32_t &h, const uint32_t &background) : BMP(w, h), img(w * std::abs(h) *
                                                                                                      pixel_size) {
    // Fill in header values
    infoHeader.biBitCount = 24;
    infoHeader.biClrUsed = 0;
    fileHeader.bfOffBits = fileHeaderSize + infoHeader.biSize;
    infoHeader.biSizeImage = getRowSize() * std::abs(h);
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    for (int32_t y = 0; y < std::abs(h); ++y) {
        for (int32_t x = 0; x < w; ++x) {
            setPixel(x, y, background);
        }
    }
}

bool BMP_24bit::save(const std::string &filename) const {
    std::ofstream f(filename, std::ios::binary);

    // Pass to base class function
    if (!BMP::save(f)) {
        f.close();
        return false;
    }

    f.seekp(fileHeader.bfOffBits); // Seek to pixel array

    const size_t &padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

    // Write image data
    if (infoHeader.biHeight < 0) {
        for (int32_t y = 0; y < -infoHeader.biHeight; ++y) {
            // Write row
            f.write(reinterpret_cast<const char *>(&img[getInternalIndex(0, y)]), pixel_size * infoHeader.biWidth);

            for (size_t i = 0; i < padSize; ++i)
                f.put(0);
        }
    } else {
        for (int32_t y = infoHeader.biHeight - 1; y >= 0; --y) {
            // Write row
            f.write(reinterpret_cast<const char *>(&img[getInternalIndex(0, y)]), pixel_size * infoHeader.biWidth);

            for (size_t i = 0; i < padSize; ++i)
                f.put(0);
        }
    }

    f.close();
    return true;
}

void BMP_24bit::setPixel(const size_t &index, const uint32_t &colour) {
    red(index) = colour >> 16u;
    green(index) = colour >> 8u;
    blue(index) = colour;
}

void BMP_24bit::setPixel(const int32_t &x, const int32_t &y, const uint32_t &colour) {
    red(x, y) = colour >> 16u;
    green(x, y) = colour >> 8u;
    blue(x, y) = colour;
}

uint32_t BMP_24bit::getPixel(const size_t &index) const {
    return (red(index) << 16u) + (green(index) << 8u) + blue(index);
}

uint32_t BMP_24bit::getPixel(const int32_t &x, const int32_t &y) const {
    return (red(x, y) << 16u) + (green(x, y) << 8u) + blue(x, y);
}

uint8_t &BMP_24bit::red(const size_t &index) {
    return img[getInternalRedIndex(index)];
}

const uint8_t &BMP_24bit::red(const size_t &index) const {
    return img[getInternalRedIndex(index)];
}

uint8_t &BMP_24bit::red(const int32_t &x, const int32_t &y) {
    return img[getInternalRedIndex(x, y)];
}

const uint8_t &BMP_24bit::red(const int32_t &x, const int32_t &y) const {
    return img[getInternalRedIndex(x, y)];
}

uint8_t &BMP_24bit::green(const size_t &index) {
    return img[getInternalGreenIndex(index)];
}

const uint8_t &BMP_24bit::green(const size_t &index) const {
    return img[getInternalGreenIndex(index)];
}

uint8_t &BMP_24bit::green(const int32_t &x, const int32_t &y) {
    return img[getInternalGreenIndex(x, y)];
}

const uint8_t &BMP_24bit::green(const int32_t &x, const int32_t &y) const {
    return img[getInternalGreenIndex(x, y)];
}

uint8_t &BMP_24bit::blue(const size_t &index) {
    return img[getInternalBlueIndex(index)];
}

const uint8_t &BMP_24bit::blue(const size_t &index) const {
    return img[getInternalBlueIndex(index)];
}

uint8_t &BMP_24bit::blue(const int32_t &x, const int32_t &y) {
    return img[getInternalBlueIndex(x, y)];
}

const uint8_t &BMP_24bit::blue(const int32_t &x, const int32_t &y) const {
    return img[getInternalBlueIndex(x, y)];
}

size_t BMP_24bit::getInternalIndex(const size_t &index) const {
    assertInvalidIndex(index);

    return index * pixel_size;
}

size_t BMP_24bit::getInternalRedIndex(const size_t &index) const {
    return getInternalIndex(index) + 2;
}

size_t BMP_24bit::getInternalGreenIndex(const size_t &index) const {
    return getInternalIndex(index) + 1;
}

size_t BMP_24bit::getInternalBlueIndex(const size_t &index) const {
    return getInternalIndex(index);
}

size_t BMP_24bit::getInternalIndex(const int32_t &x, const int32_t &y) const {
    return getIndex(x, y) * pixel_size;
}

size_t BMP_24bit::getInternalRedIndex(const int32_t &x, const int32_t &y) const {
    return getInternalIndex(x, y) + 2;
}

size_t BMP_24bit::getInternalGreenIndex(const int32_t &x, const int32_t &y) const {
    return getInternalIndex(x, y) + 1;
}

size_t BMP_24bit::getInternalBlueIndex(const int32_t &x, const int32_t &y) const {
    return getInternalIndex(x, y);
}

