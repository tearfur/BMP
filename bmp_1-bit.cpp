//
// Created by HoYuYat on 8/11/2019.
//

#include "bmp_1-bit.h"
#include "bmp.h"
#include <fstream>
#include <cstddef>
#include <cstdlib>

BMP_1bit::BMP_1bit(const std::string& filename): BMP_CT(filename) {
	if (infoHeader.biBitCount != 1) {
		std::cerr << "BMP_1bit: This is not a 1-bit BMP file." << std::endl;
		std::exit(1);
	}

	// Ensure the colour table is valid
	const uint32_t colourTableSize = fileHeader.bfOffBits - sizeof(fileHeader) - infoHeader.biSize;
	if (!validColourTableSize(colourTableSize) || colourTableSize > 8) {
		std::cerr << "BMP_1bit: Invalid colour table." << std::endl;
		std::exit(1);
	}

	std::ifstream f(filename, std::ios::binary);
	f.seekg(sizeof(fileHeader) + infoHeader.biSize); // Seek to colour table

	// Read colour table.
	colourTable.resize(colourTableSize);
	f.read(reinterpret_cast<char*>(&colourTable[0]), colourTableSize);

	f.seekg(fileHeader.bfOffBits); // Seek to the start of image array

	const std::size_t& padSize = getRowSize() - (infoHeader.biWidth + 7) / 8; // Size of padding on each row in bytes

	// Read image data into img
	uint8_t buf = 0; // For storing each byte read
	if (infoHeader.biHeight < 0) { // Check if height is positive or negative
		img.resize(infoHeader.biWidth * -infoHeader.biHeight); //Expand vector to required size

		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				// If divisible by 8, read the new byte (8-bits in 1 byte)
				const uint8_t& remainder = x % 8;
				if (!remainder) {
					f.read(reinterpret_cast<char*>(&buf), sizeof(buf));
				}

				operator()(x, y) = buf >> (8u - 1u - remainder) & 0b1u; // Extract the bit
			}

			f.seekg(padSize, std::ios::cur); // Skip pad bytes
		}
	} else {
		img.resize(infoHeader.biWidth * infoHeader.biHeight); //Expand vector to required size

		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				// If divisible by 8, read the new byte (8-bits in 1 byte)
				const uint8_t& remainder = x % 8;
				if (!remainder)
					f.read(reinterpret_cast<char*>(&buf), sizeof(buf));

				operator()(x, y) = buf >> (8u - 1u - remainder) & 0b1u; // Extract the bit
			}

			f.seekg(padSize, std::ios::cur); // Skip pad bytes
		}
	}

	f.close();
}

BMP_1bit::BMP_1bit(const int32_t& w, const int32_t& h, bool background): BMP_CT(w, h), img(w * std::abs(h), background) {
	// Fill in header values
	infoHeader.biBitCount = 1;
	infoHeader.biClrUsed = 1u << infoHeader.biBitCount;
	fileHeader.bfOffBits = sizeof(fileHeader) + infoHeader.biSize + (infoHeader.biClrUsed << 2u);
	while (fileHeader.bfOffBits % 4) ++fileHeader.bfOffBits; // Ensure the image array is aligned to 4-byte word
	infoHeader.biSizeImage = getRowSize() * std::abs(h);
	fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

	// Set 0 as black and 1 as white
	colourTable.resize(8);
	colourTable[0] = colourTable[1] = colourTable[2] = colourTable[3] = colourTable[7] = 0;
	colourTable[4] = colourTable[5] = colourTable[6] = 255;
}

bool BMP_1bit::save(const std::string& filename) const {
	std::ofstream f(filename, std::ios::binary);

	// Pass to base class function
	if (!BMP_CT::save(f)) {
		f.close();
		return false;
	}

	// Seek to offset
	f.seekp(fileHeader.bfOffBits);

	const std::size_t& padSize = getRowSize() - (infoHeader.biWidth + 7) / 8; // Size of padding on each row in bytes

	// Write image data
	uint8_t buf = 0; // Buffer for storing each byte
	if (infoHeader.biHeight < 0) {
		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				const uint8_t remainder = x % 8;
				if (!remainder) buf = 0;

				buf |= (operator()(x, y) != 0) << (8 - 1 - remainder); // Ensure the value is either 1 or 0

				// Write whenever a byte is complete
				if (remainder == 7 || x == infoHeader.biWidth - 1) f.write(reinterpret_cast<char*>(&buf), sizeof(buf));
			}

			for (std::size_t i = 0; i < padSize; ++i) f.put(0); // Skip padding
		}
	} else {
		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				const uint8_t remainder = x % 8;
				if (!remainder) buf = 0;

				buf |= (operator()(x, y) != 0) << (8 - 1 - remainder); // Ensure the value is either 1 or 0

				// Write whenever a byte is complete
				if (remainder == 7 || x == infoHeader.biWidth - 1) f.write(reinterpret_cast<char*>(&buf), sizeof(buf));
			}

			for (std::size_t i = 0; i < padSize; ++i) f.put(0); // Skip padding
		}
	}

	f.close();
	return true;
}

uint8_t& BMP_1bit::operator[](const std::size_t& index) {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

const uint8_t& BMP_1bit::operator[](const std::size_t& index) const {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

uint8_t& BMP_1bit::operator()(const uint32_t& x, const uint32_t& y) {
	return img[getIndex(x, y)];
}

const uint8_t& BMP_1bit::operator()(const uint32_t& x, const uint32_t& y) const {
	return img[getIndex(x, y)];
}
