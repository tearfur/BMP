//
// Created by HoYuYat on 8/11/2019.
//

#include "bmp.h"
#include <iostream>
#include <cstdlib>

const uint16_t BM = 'B' + ('M' << 8); // Little-endian

BMP::BMP(): fileHeader({0}), infoHeader({0}) {
	fileHeader.bfType = BM;
	infoHeader.biPlanes = 1;
	infoHeader.biSize = 40;
}

BMP::BMP(const std::string& filename): BMP() {
	std::ifstream f(filename, std::ios::binary);

	// Check if the file exists
	if (!f) {
		std::cerr << "BMP: The file does not exist." << std::endl;
		std::exit(1);
	}

	// Read in headers
	f.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	f.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

	// Check if the format is supported
	if (fileHeader.bfType != BM || infoHeader.biSize != 40) {
		std::cerr << "BMP: This format is not supported." << std::endl;
		std::exit(1);
	}

	// Check if compression option is supported
	if (infoHeader.biCompression != 0 && infoHeader.biCompression != 3) {
		std::cerr << "BMP: This compression type is not supported." << std::endl;
		std::exit(1);
	}

	f.close();
}

BMP::BMP(const int32_t& w, const int32_t& h): BMP() {
	infoHeader.biWidth = w;
	infoHeader.biHeight = h;
}

std::size_t BMP::getRowSize() const {
	return (infoHeader.biBitCount * infoHeader.biWidth + 31) / 32 * 4;
}

bool BMP::save(std::ofstream& f) const {
	// Check if the file has been successfully opened
	if (!f) {
		std::cerr << "BMP: The file location cannot be accessed." << std::endl;
		return false;
	}

	// Write headers
	f.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
	f.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

	return true;
}

std::size_t BMP::getIndex(const uint32_t& x, const uint32_t& y) const {
	if (!validIndex(x, y)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return y * infoHeader.biWidth + x;
}

bool BMP::validIndex(const std::size_t& index) const {
	return index < infoHeader.biWidth * std::abs(infoHeader.biHeight);
}

bool BMP::validIndex(const uint32_t& x, const uint32_t& y) const {
	return x < infoHeader.biWidth && y < std::abs(infoHeader.biHeight);
}

void BMP::assertInvalidIndex() const {
	std::cerr << "BMP: Index out of bounds" << std::endl;
}
