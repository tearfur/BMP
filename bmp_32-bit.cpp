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

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

	//Read image data into img
	img.resize(infoHeader.biWidth * std::abs(infoHeader.biHeight));
	if (infoHeader.biHeight < 0) {
		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			f.read(reinterpret_cast<char*>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

			f.seekg(padSize, std::ios::cur);
		}
	} else {
		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			f.read(reinterpret_cast<char*>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

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
	fileHeader.bfOffBits = fileHeaderSize + infoHeader.biSize + 3 * sizeof(uint32_t);
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

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

	// Write image data
	if (infoHeader.biHeight < 0) {
		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			f.write(reinterpret_cast<const char*>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

			for (std::size_t i = 0; i < padSize; ++i) f.put(0);
		}
	} else {
		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			f.write(reinterpret_cast<const char*>(&operator()(0, y)), pixel_size * infoHeader.biWidth);

			for (std::size_t i = 0; i < padSize; ++i) f.put(0);
		}
	}

	f.close();
	return true;
}

uint32_t& BMP_32bit::operator[](const std::size_t& index) {
	assertInvalidIndex(index);

	return img[index];
}

const uint32_t& BMP_32bit::operator[](const std::size_t& index) const {
	assertInvalidIndex(index);

	return img[index];
}

uint32_t& BMP_32bit::operator()(const uint32_t& x, const uint32_t& y) {
	return img[getIndex(x, y)];
}

const uint32_t& BMP_32bit::operator()(const uint32_t& x, const uint32_t& y) const {
	return img[getIndex(x, y)];
}
