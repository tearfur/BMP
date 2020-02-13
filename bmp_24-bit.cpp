//
// Created by HoYuYat on 8/11/2019.
//

#include "bmp_24-bit.h"
#include <iostream>
#include <cstddef>

BMP_24bit::BMP_24bit(const std::string& filename): BMP(filename), red_accessor(this), green_accessor(this), blue_accessor(this) {
	if (infoHeader.biBitCount != 24) {
		std::cerr << "BMP_24bit: This is not a 24-bit BMP file." << std::endl;
		std::exit(1);
	}

	std::ifstream f(filename, std::ios::binary);

	f.seekg(fileHeader.bfOffBits); // Seek to pixel array

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

	//Read image data into img
	if (infoHeader.biHeight < 0) {
		img.resize(infoHeader.biWidth * -infoHeader.biHeight);

		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.read(reinterpret_cast<char*>(&operator()(x, y)), pixel_size);
			}

			f.seekg(padSize, std::ios::cur);
		}
	} else {
		img.resize(infoHeader.biWidth * infoHeader.biHeight);

		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.read(reinterpret_cast<char*>(&operator()(x, y)), pixel_size);
			}

			f.seekg(padSize, std::ios::cur);
		}
	}

	f.close();
}

BMP_24bit::BMP_24bit(const int32_t& w, const int32_t& h, const uint32_t& background):
		BMP(w, h), img(w * std::abs(h), background < 1u << 24u ? background : (1u << 24u) - 1), red_accessor(this),
		green_accessor(this), blue_accessor(this) {
	// Fill in header values
	infoHeader.biBitCount = 24;
	infoHeader.biClrUsed = 0;
	fileHeader.bfOffBits = sizeof(fileHeader) + infoHeader.biSize;
	infoHeader.biSizeImage = getRowSize() * std::abs(h);
	fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;
}

bool BMP_24bit::save(const std::string& filename) const {
	std::ofstream f(filename, std::ios::binary);

	// Pass to base class function
	if (!BMP::save(f)) {
		f.close();
		return false;
	}

	f.seekp(fileHeader.bfOffBits); // Seek to pixel array

	const std::size_t& padSize = getRowSize() - infoHeader.biWidth * pixel_size; // Size of padding on each row in bytes

	// Write image data
	if (infoHeader.biHeight < 0) {
		for (std::size_t y = 0; y < -infoHeader.biHeight; ++y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.write(reinterpret_cast<const char*>(&operator()(x, y)), pixel_size);
			}

			for (std::size_t i = 0; i < padSize; ++i) f.put(0);
		}
	} else {
		for (std::size_t y = infoHeader.biHeight - 1; y < infoHeader.biHeight; --y) {
			for (std::size_t x = 0; x < infoHeader.biWidth; ++x) {
				f.write(reinterpret_cast<const char*>(&operator()(x, y)), pixel_size);
			}

			for (std::size_t i = 0; i < padSize; ++i) f.put(0);
		}
	}

	f.close();
	return true;
}

uint32_t& BMP_24bit::operator[](const std::size_t& index) {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

const uint32_t& BMP_24bit::operator[](const std::size_t& index) const {
	if (!validIndex(index)) {
		assertInvalidIndex();
		std::exit(1);
	}
	return img[index];
}

uint32_t& BMP_24bit::operator()(const uint32_t& x, const uint32_t& y) {
	return img[getIndex(x, y)];
}

const uint32_t& BMP_24bit::operator()(const uint32_t& x, const uint32_t& y) const {
	return img[getIndex(x, y)];
}

BMP_24bit::Accessor::Accessor(BMP_24bit* const& ptr): ptr(ptr), index(0) {}

std::size_t BMP_24bit::Accessor::getIndex() const {
	return index;
}

BMP_24bit::Red::Red(BMP_24bit* const& ptr): Accessor(ptr) {}

BMP_24bit::Red& BMP_24bit::Red::operator[](const std::size_t& index) {
	this->index = index;

	return *this;
}

uint8_t BMP_24bit::Red::operator[](const std::size_t& index) const {
	if (!ptr->validIndex(index)) {
		ptr->assertInvalidIndex();
		std::exit(1);
	}

	return getValue(index);
}

BMP_24bit::Red& BMP_24bit::Red::operator()(const uint32_t& x, const uint32_t& y) {
	index = ptr->getIndex(x, y);

	return *this;
}

uint8_t BMP_24bit::Red::operator()(const uint32_t& x, const uint32_t& y) const {
	return operator[](ptr->getIndex(x, y));
}

BMP_24bit::Red& BMP_24bit::Red::operator=(const uint8_t& value) {
	(*ptr)[index] <<= 8u;
	(*ptr)[index] >>= 8u; // Clear the red bits
	(*ptr)[index] |= value << 16u;

	return *this;
}

BMP_24bit::Red::operator uint8_t() const {
	return getValue();
}

uint8_t BMP_24bit::Red::getValue() const {
	return (*ptr)[index] >> 16u;
}

uint8_t BMP_24bit::Red::getValue(const uint32_t& index) const {
	return (*ptr)[index] >> 16u;
}

BMP_24bit::Green::Green(BMP_24bit* const& ptr): Accessor(ptr) {}

BMP_24bit::Green& BMP_24bit::Green::operator[](const std::size_t& index) {
	this->index = index;

	return *this;
}

uint8_t BMP_24bit::Green::operator[](const std::size_t& index) const {
	if (!ptr->validIndex(index)) {
		ptr->assertInvalidIndex();
		std::exit(1);
	}

	return getValue(index);
}

BMP_24bit::Green& BMP_24bit::Green::operator()(const uint32_t& x, const uint32_t& y) {
	index = ptr->getIndex(x, y);

	return *this;
}

uint8_t BMP_24bit::Green::operator()(const uint32_t& x, const uint32_t& y) const {
	return operator[](ptr->getIndex(x, y));
}

BMP_24bit::Green& BMP_24bit::Green::operator=(const uint8_t& value) {
	(*ptr)[index] &= 0xFF00FFu; // Clear green bits
	(*ptr)[index] |= value << 8u;

	return *this;
}

BMP_24bit::Green::operator uint8_t() const {
	return getValue();
}

uint8_t BMP_24bit::Green::getValue() const {
	return ((*ptr)[index] >> 8u) & 0xFFu;
}

uint8_t BMP_24bit::Green::getValue(const uint32_t& index) const {
	return ((*ptr)[index] >> 8u) & 0xFFu;
}

BMP_24bit::Blue::Blue(BMP_24bit* const& ptr): Accessor(ptr) {}

BMP_24bit::Blue& BMP_24bit::Blue::operator[](const std::size_t& index) {
	this->index = index;

	return *this;
}

uint8_t BMP_24bit::Blue::operator[](const std::size_t& index) const {
	if (!ptr->validIndex(index)) {
		ptr->assertInvalidIndex();
		std::exit(1);
	}

	return getValue(index);
}

BMP_24bit::Blue& BMP_24bit::Blue::operator()(const uint32_t& x, const uint32_t& y) {
	index = ptr->getIndex(x, y);

	return *this;
}

uint8_t BMP_24bit::Blue::operator()(const uint32_t& x, const uint32_t& y) const {
	return operator[](ptr->getIndex(x, y));
}

BMP_24bit::Blue& BMP_24bit::Blue::operator=(const uint8_t& value) {
	(*ptr)[index] >>= 8u;
	(*ptr)[index] <<= 8u; // Clear blue bits
	(*ptr)[index] |= value;

	return *this;
}

BMP_24bit::Blue::operator uint8_t() const {
	return getValue();
}

uint8_t BMP_24bit::Blue::getValue() const {
	return (*ptr)[index] & 0xFFu;
}

uint8_t BMP_24bit::Blue::getValue(const uint32_t& index) const {
	return (*ptr)[index] & 0xFFu;
}

BMP_24bit::Red& BMP_24bit::red(const std::size_t& index) {
	return red_accessor[index];
}

uint8_t BMP_24bit::red(const std::size_t& index) const {
	return red_accessor[index];
}

BMP_24bit::Red& BMP_24bit::red(const uint32_t& x, const uint32_t& y) {
	return red_accessor(x, y);
}

uint8_t BMP_24bit::red(const uint32_t& x, const uint32_t& y) const {
	return red_accessor(x, y);
}

BMP_24bit::Green& BMP_24bit::green(const std::size_t& index) {
	return green_accessor[index];
}

uint8_t BMP_24bit::green(const std::size_t& index) const {
	return green_accessor[index];
}

BMP_24bit::Green& BMP_24bit::green(const uint32_t& x, const uint32_t& y) {
	return green_accessor(x, y);
}

uint8_t BMP_24bit::green(const uint32_t& x, const uint32_t& y) const {
	return green_accessor(x, y);
}

BMP_24bit::Blue& BMP_24bit::blue(const std::size_t& index) {
	return blue_accessor[index];
}

uint8_t BMP_24bit::blue(const std::size_t& index) const {
	return blue_accessor[index];
}

BMP_24bit::Blue& BMP_24bit::blue(const uint32_t& x, const uint32_t& y) {
	return blue_accessor(x, y);
}

uint8_t BMP_24bit::blue(const uint32_t & x, const uint32_t & y) const {
	return blue_accessor(x, y);
}
