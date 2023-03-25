#include "bmp_16-bit.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

const std::vector<uint32_t> BMP_16bit::RGB565_bitmask = {
        0xF8000000, //r
        0x7E00000, //g
        0x1F0000 //b
};

const std::vector<uint32_t> BMP_16bit::RGB555_bitmask = {
        0xF8000000, //r
        0x7C00000, //g
        0x3E0000 //b
};

BMP_16bit::BMP_16bit(const std::string &filename) : BMP_BM(filename) {
    if (infoHeader.biBitCount != 16) {
        std::cerr << "BMP_16bit: This is not a 16-bit BMP file." << std::endl;
        std::exit(1);
    }

    // Set to RGB565 if cannot read valid bitmask (i.e. bitmask left untouched)
    if (infoHeader.biCompression == 3 && bitmask.empty())
        bitmask = RGB565_bitmask;

    std::ifstream f(filename, std::ios::binary);

    f.seekg(fileHeader.bfOffBits); // Seek to pixel array

    const size_t &padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

    //Read image data into img
    img.resize(infoHeader.biWidth * std::abs(infoHeader.biHeight));
    if (infoHeader.biHeight < 0) {
        for (int32_t y = 0; y < -infoHeader.biHeight; ++y) {
            f.read(reinterpret_cast<char *>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

            f.seekg(padSize, std::ios::cur);
        }
    } else {
        for (int32_t y = infoHeader.biHeight - 1; y >= 0; --y) {
            f.read(reinterpret_cast<char *>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

            f.seekg(padSize, std::ios::cur);
        }
    }

    f.close();
}

BMP_16bit::BMP_16bit(const int32_t &w, const int32_t &h, const uint16_t &background, std::vector<uint32_t> bm) : BMP_BM(
        w, h, std::move(bm)), img(w * std::abs(h), background) {
    // Fill in header values
    infoHeader.biBitCount = 16;
    infoHeader.biClrUsed = 0;
    fileHeader.bfOffBits = fileHeaderSize + infoHeader.biSize + 3 * sizeof(uint32_t);
    infoHeader.biSizeImage = getRowSize() * std::abs(h);
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;
}

bool BMP_16bit::save(const std::string &filename) const {
    std::ofstream f(filename, std::ios::binary);

    // Pass to base class function
    if (!BMP_BM::save(f)) {
        f.close();
        return false;
    }

    f.seekp(fileHeader.bfOffBits); // Seek to pixel array

    const size_t &padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

    // Write image data
    if (infoHeader.biHeight < 0) {
        for (int32_t y = 0; y < -infoHeader.biHeight; ++y) {
            f.write(reinterpret_cast<const char *>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

            for (size_t i = 0; i < padSize; ++i)
                f.put(0);
        }
    } else {
        for (int32_t y = infoHeader.biHeight - 1; y >= 0; --y) {
            f.write(reinterpret_cast<const char *>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

            for (size_t i = 0; i < padSize; ++i)
                f.put(0);
        }
    }

    f.close();
    return true;
}

uint16_t &BMP_16bit::operator[](const size_t &index) {
    assertInvalidIndex(index);

    return img[index];
}

const uint16_t &BMP_16bit::operator[](const size_t &index) const {
    assertInvalidIndex(index);

    return img[index];
}

uint16_t &BMP_16bit::operator()(const int32_t &x, const int32_t &y) {
    return img[getIndex(x, y)];
}

const uint16_t &BMP_16bit::operator()(const int32_t &x, const int32_t &y) const {
    return img[getIndex(x, y)];
}
