#include "bmp_8-bit.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

BMP_8bit::BMP_8bit(const std::string& filename): BMP_CT(filename) {
	if (infoHeader.biBitCount != 8) {
		std::cerr << "BMP_8bit: This is not a 8-bit BMP file." << std::endl;
		std::exit(1);
	}

	// Ensure the colour table is valid
//	const uint32_t colourTableSize = fileHeader.bfOffBits - fileHeaderSize - infoHeader.biSize;
//	if (!validClrTableSize(colourTableSize) || colourTableSize > 4u << 8u) {
//		std::cerr << "BMP_8bit: Invalid colour table." << std::endl;
//		std::exit(1);
//	}

	std::ifstream f(filename, std::ios::binary);
	f.seekg(fileHeaderSize + infoHeader.biSize); // Seek to colour table

	// Read colour table.
	readClrTable(f);

	f.seekg(fileHeader.bfOffBits); // Seek to the start of image array

	const size_t& padSize = getRowSize() - infoHeader.biWidth; // Size of padding on each row in bytes

	//Read image data into img
	img.resize(infoHeader.biWidth * std::abs(infoHeader.biHeight));
	if (infoHeader.biHeight < 0) {
		for (size_t y = 0; y < -infoHeader.biHeight; ++y) {
			f.read(reinterpret_cast<char*>(&operator()(0, y)), infoHeader.biWidth);

			f.seekg(padSize, std::ios::cur);
		}
	} else {
		for (size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			f.read(reinterpret_cast<char*>(&operator()(0, y)), infoHeader.biWidth);

			f.seekg(padSize, std::ios::cur);
		}
	}

	f.close();
}

BMP_8bit::BMP_8bit(const int32_t& w, const int32_t& h, const uint8_t& background): BMP_CT(w, h), img(w * std::abs(h), background) {
	// Fill in header values
	infoHeader.biBitCount = 8;
	infoHeader.biClrUsed = 1u << infoHeader.biBitCount;
	fileHeader.bfOffBits = fileHeaderSize + infoHeader.biSize + (infoHeader.biClrUsed << 2u);
	infoHeader.biSizeImage = getRowSize() * std::abs(h);
	fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

	// Fill in colour table
	for (uint32_t i = 0; i < infoHeader.biClrUsed; ++i) {
		colourTable.push_back(i); //b
		colourTable.push_back(i); //g
		colourTable.push_back(i); //r
		colourTable.push_back(0); //a
	}
}

bool BMP_8bit::save(const std::string& filename) const {
	std::ofstream f(filename, std::ios::binary);

	// Pass to base class function
	if (!BMP_CT::save(f)) {
		f.close();
		return false;
	}

	// Seek to offset
	f.seekp(fileHeader.bfOffBits);

	const size_t& padSize = getRowSize() - infoHeader.biWidth; // Size of padding on each row in bytes

	// Write image data
	if (infoHeader.biHeight < 0) {
		for (size_t y = 0; y < -infoHeader.biHeight; ++y) {
			f.write(reinterpret_cast<const char*>(&operator()(0, y)), infoHeader.biWidth);

			for (size_t i = 0; i < padSize; ++i) f.put(0);
		}//Expand vector to required size
	} else {
		for (size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			f.write(reinterpret_cast<const char*>(&operator()(0, y)), infoHeader.biWidth);

			for (size_t i = 0; i < padSize; ++i) f.put(0);
		}
	}

	f.close();
	return true;
}

uint8_t& BMP_8bit::operator[](const size_t& index) {
	assertInvalidIndex(index);

	return img[index];
}

const uint8_t& BMP_8bit::operator[](const size_t& index) const {
	assertInvalidIndex(index);

	return img[index];
}

uint8_t& BMP_8bit::operator()(const int32_t& x, const int32_t& y) {
	return img[getIndex(x, y)];
}

const uint8_t& BMP_8bit::operator()(const int32_t& x, const int32_t& y) const {
	return img[getIndex(x, y)];
}

uint32_t BMP_8bit::toRGB888(const uint8_t& grey) {
	return (grey << 16u) + (grey << 8u) + grey;
}
