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
	f.read(reinterpret_cast<char*>(&fileHeader.bfType), sizeof(fileHeader.bfType));
	f.read(reinterpret_cast<char*>(&fileHeader.bfSize), sizeof(fileHeader.bfSize));
	f.read(reinterpret_cast<char*>(&fileHeader.bfReserved1), sizeof(fileHeader.bfReserved1));
	f.read(reinterpret_cast<char*>(&fileHeader.bfReserved2), sizeof(fileHeader.bfReserved2));
	f.read(reinterpret_cast<char*>(&fileHeader.bfOffBits), sizeof(fileHeader.bfOffBits));

	f.read(reinterpret_cast<char*>(&infoHeader.biSize), sizeof(infoHeader.biSize));
	f.read(reinterpret_cast<char*>(&infoHeader.biWidth), sizeof(infoHeader.biWidth));
	f.read(reinterpret_cast<char*>(&infoHeader.biHeight), sizeof(infoHeader.biHeight));
	f.read(reinterpret_cast<char*>(&infoHeader.biPlanes), sizeof(infoHeader.biPlanes));
	f.read(reinterpret_cast<char*>(&infoHeader.biBitCount), sizeof(infoHeader.biBitCount));
	f.read(reinterpret_cast<char*>(&infoHeader.biCompression), sizeof(infoHeader.biCompression));
	f.read(reinterpret_cast<char*>(&infoHeader.biSizeImage), sizeof(infoHeader.biSizeImage));
	f.read(reinterpret_cast<char*>(&infoHeader.biXPelsPerMeter), sizeof(infoHeader.biXPelsPerMeter));
	f.read(reinterpret_cast<char*>(&infoHeader.biYPelsPerMeter), sizeof(infoHeader.biYPelsPerMeter));
	f.read(reinterpret_cast<char*>(&infoHeader.biClrUsed), sizeof(infoHeader.biClrUsed));
	f.read(reinterpret_cast<char*>(&infoHeader.biClrImportant), sizeof(infoHeader.biClrImportant));

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

size_t BMP::getRowSize() const {
	return (infoHeader.biBitCount * infoHeader.biWidth + 31) / 32 * 4;
}

bool BMP::save(std::ofstream& f) const {
	// Check if the file has been successfully opened
	if (!f) {
		std::cerr << "BMP: The file location cannot be accessed." << std::endl;
		return false;
	}

	// Write headers
	f.write(reinterpret_cast<const char*>(&fileHeader.bfType), sizeof(fileHeader.bfType));
	f.write(reinterpret_cast<const char*>(&fileHeader.bfSize), sizeof(fileHeader.bfSize));
	f.write(reinterpret_cast<const char*>(&fileHeader.bfReserved1), sizeof(fileHeader.bfReserved1));
	f.write(reinterpret_cast<const char*>(&fileHeader.bfReserved2), sizeof(fileHeader.bfReserved2));
	f.write(reinterpret_cast<const char*>(&fileHeader.bfOffBits), sizeof(fileHeader.bfOffBits));

	f.write(reinterpret_cast<const char*>(&infoHeader.biSize), sizeof(infoHeader.biSize));
	f.write(reinterpret_cast<const char*>(&infoHeader.biWidth), sizeof(infoHeader.biWidth));
	f.write(reinterpret_cast<const char*>(&infoHeader.biHeight), sizeof(infoHeader.biHeight));
	f.write(reinterpret_cast<const char*>(&infoHeader.biPlanes), sizeof(infoHeader.biPlanes));
	f.write(reinterpret_cast<const char*>(&infoHeader.biBitCount), sizeof(infoHeader.biBitCount));
	f.write(reinterpret_cast<const char*>(&infoHeader.biCompression), sizeof(infoHeader.biCompression));
	f.write(reinterpret_cast<const char*>(&infoHeader.biSizeImage), sizeof(infoHeader.biSizeImage));
	f.write(reinterpret_cast<const char*>(&infoHeader.biXPelsPerMeter), sizeof(infoHeader.biXPelsPerMeter));
	f.write(reinterpret_cast<const char*>(&infoHeader.biYPelsPerMeter), sizeof(infoHeader.biYPelsPerMeter));
	f.write(reinterpret_cast<const char*>(&infoHeader.biClrUsed), sizeof(infoHeader.biClrUsed));
	f.write(reinterpret_cast<const char*>(&infoHeader.biClrImportant), sizeof(infoHeader.biClrImportant));

	return true;
}

size_t BMP::getIndex(const int32_t& x, const int32_t& y) const {
	assertInvalidIndex(x, y);

	return y * infoHeader.biWidth + x;
}

bool BMP::validIndex(const size_t& index) const {
	return index < infoHeader.biWidth * std::abs(infoHeader.biHeight);
}

bool BMP::validIndex(const int32_t& x, const int32_t& y) const {
	return x >= 0 && x < infoHeader.biWidth && y >= 0 && y < std::abs(infoHeader.biHeight);
}

void BMP::assertInvalidIndex(const size_t& index) const {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
}

void BMP::assertInvalidIndex(const int32_t& x, const int32_t& y) const {
	if (!validIndex(x, y)) {
		assertInvalidIndex();
		std::exit(1);
	}
}

void BMP::assertInvalidIndex() {
	std::cerr << "BMP: Index out of bounds" << std::endl;
}

const BMP::FileHeader& BMP::getFileHeader() const {
	return fileHeader;
}

const BMP::InfoHeader& BMP::getInfoHeader() const {
	return infoHeader;
}
