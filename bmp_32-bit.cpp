//
// Created by HoYuYat on 8/11/2019.
//

#include "bmp_32-bit.h"
#include <iostream>

const std::vector<uint32_t> BMP_32bit::RGB888_bitmask = {
		0xFF000000, //r
		0xFF0000, //g
		0xFF00 //b
};

const std::vector<uint32_t> BMP_32bit::RGB101010_bitmask = {
		0xFFC00000, //r
		0x3FF000, //g
		0xFFC //b
};

BMP_32bit::BMP_32bit(const std::string& filename): BMP_BM(filename) {
	if (infoHeader.biBitCount != 32) {
		std::cerr << "BMP_32bit: This is not a 32-bit BMP file." << std::endl;
		std::exit(1);
	}

	// Set to RGB888 if cannot read valid bitmask (i.e. bitmask left untouched)
	if (infoHeader.biCompression == 3 && bitmask.empty()) bitmask = RGB888_bitmask;

	std::ifstream f(filename, std::ios::binary);

	f.seekg(fileHeader.bfOffBits); // Seek to pixel array

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * sizeof(uint32_t); // Size of padding on each row in bytes

	//Read image data into img
	if (infoHeader.biHeight < 0) {
		img.resize(infoHeader.biWidth * -infoHeader.biHeight);

		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.read(reinterpret_cast<char*>(&operator()(x, y)), sizeof(uint32_t));
			}

			f.seekg(padSize, std::ios::cur);
		}
	} else {
		img.resize(infoHeader.biWidth * infoHeader.biHeight);

		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.read(reinterpret_cast<char*>(&operator()(x, y)), sizeof(uint32_t));
			}

			f.seekg(padSize, std::ios::cur);
		}
	}

	f.close();
}

BMP_32bit::BMP_32bit(const int32_t& w, const int32_t& h, const uint32_t& background, std::vector<uint32_t> bm):
		BMP_BM(w, h, std::move(bm)), img(w * std::abs(h), background) {
	// Fill in header values
	infoHeader.biBitCount = 32;
	infoHeader.biClrUsed = 0;
	fileHeader.bfOffBits = sizeof(fileHeader) + infoHeader.biSize + 3 * sizeof(uint32_t);
	while (fileHeader.bfOffBits % 4) ++fileHeader.bfOffBits; // Ensure the image array is aligned to 4-byte word
	infoHeader.biSizeImage = getRowSize() * std::abs(h);
	fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;
}

bool BMP_32bit::save(const std::string& filename) const {
	std::ofstream f(filename, std::ios::binary);

	// Pass to base class function
	if (!BMP_BM::save(f)) {
		f.close();
		return false;
	}

	f.seekp(fileHeader.bfOffBits); // Seek to pixel array

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * sizeof(uint32_t); // Size of padding on each row in bytes

	// Write image data
	if (infoHeader.biHeight < 0) {
		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.write(reinterpret_cast<const char*>(&operator()(x, y)), sizeof(uint32_t));
			}

			f.seekp(padSize, std::ios::cur);
		}
	} else {
		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.write(reinterpret_cast<const char*>(&operator()(x, y)), sizeof(uint32_t));
			}

			f.seekp(padSize, std::ios::cur);
		}
	}

	f.close();
	return true;
}

uint32_t& BMP_32bit::operator[](const std::size_t& index) {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

const uint32_t& BMP_32bit::operator[](const std::size_t& index) const {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

uint32_t& BMP_32bit::operator()(const uint32_t& x, const uint32_t& y) {
	return img[getIndex(x, y)];
}

const uint32_t& BMP_32bit::operator()(const uint32_t& x, const uint32_t& y) const {
	return img[getIndex(x, y)];
}