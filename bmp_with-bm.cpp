//
// Created by HoYuYat on 8/11/2019.
//

#include "bmp_with-bm.h"
#include "bmp.h"
#include <fstream>
#include <iostream>
#include <algorithm>

BMP_BM::BMP_BM(const std::string& filename): BMP(filename) {
	std::ifstream f(filename, std::ios::binary);

	// Read bitmask if applicable
	if (infoHeader.biCompression == 3) {
		const auto invalidBitmask = [&]() -> void {
			std::cerr << "BMP_BM: Invalid bitmasks. For 16-bit, the pixels will be treated as RGB565. For 32-bit, the pixels will be treated as RGB888" << std::endl;
		};

		if (fileHeader.bfOffBits - fileHeaderSize - infoHeader.biSize != 32 * 3 / 8) invalidBitmask();
		else {
			// Read bitmask
			std::vector<uint32_t> temp(3);

			f.seekg(fileHeaderSize + infoHeader.biSize); // Seek to bitmask
			f.read(reinterpret_cast<char*>(&temp[0]), sizeof(uint32_t) * 3); // Read bitmask

			// Use input bitmask if valid
			if (convertBitmask(temp)) bitmask = temp;
			else invalidBitmask();
		}
	}

	f.close();
}

BMP_BM::BMP_BM(const int32_t& w, const int32_t& h, std::vector<uint32_t> bm): BMP(w, h), bitmask(std::move(bm)) {}

bool BMP_BM::save(std::ofstream& f) const {
	if (!BMP::save(f)) return false;

	// Write bitmask
	if (infoHeader.biCompression == 3)
		f.write(reinterpret_cast<const char*>(&bitmask[0]), sizeof(uint32_t) * bitmask.size());

	return true;
}

bool BMP_BM::convertBitmask(std::vector<uint32_t>& bm) {
	if (bm.size() != 3) return false;

	// Check if it is ordered from the most significant bit
	for (uint8_t i = 0; i < 3; ++i) {
		const uint32_t msb = 1u << 31u;
		if (i < 2 && bm[i] & msb) break;
		if (i == 2 && !(bm[i] & msb)) return false;
	}

	// Check for overlapping
	for (uint8_t i = 0; i < 3; ++i) {
		for (uint8_t j = i; j < 3; ++j) { // Check every possible combination for overlapping
			if (bm[i] & bm[j]) return false;
		}
	}

	std::sort(bm.rbegin(), bm.rend()); //The value of the RGB bitmasks are in descending order

	// Check if they are contiguous
	for (uint8_t i = 0; i < 2; ++i) {
		// Shift the next bitmask left by 1 bit, if still no overlap, not contiguous
		if (!(bm[i] & bm[i + 1] << 1u)) return false;
	}

	return true;
}
