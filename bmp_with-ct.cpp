#include "bmp_with-ct.h"
#include "bmp.h"
#include <iostream>

BMP_CT::BMP_CT(const std::string &filename) : BMP(filename) {
    infoHeader.biCompression = 0; // Ignore this field, treat it as uncompressed even if it's 3
}

BMP_CT::BMP_CT(const int32_t &w, const int32_t &h) : BMP(w, h) {
}

void BMP_CT::readClrTable(std::ifstream &f) {
    const uint32_t colourTableSize = 1u << infoHeader.biBitCount << 2u;
    colourTable.resize(colourTableSize);
    f.read(reinterpret_cast<char *>(&colourTable[0]), colourTableSize);
}

bool BMP_CT::save(std::ofstream &f) const {
    if (!BMP::save(f))
        return false;

    if (infoHeader.biCompression == 3) {
        std::cerr << "BMP_CT: This compression method is illegal or unsupported in this colour-depth." << std::endl;
        return false;
    }

    // Write colour table
    f.write(reinterpret_cast<const char *>(&colourTable[0]),
            infoHeader.biClrUsed ? infoHeader.biClrUsed << 2u : 4u << infoHeader.biBitCount);

    return true;
}

bool BMP_CT::validClrTableSize(const uint32_t &size) const {
    return !(size % 4) && ((infoHeader.biClrUsed && size / 4 == infoHeader.biClrUsed) ||
                           (!infoHeader.biClrUsed && size / 4 == 1u << infoHeader.biBitCount));
}

bool BMP_CT::validNewClrTableSize(const uint32_t &size) const {
    return size <= 4u << infoHeader.biBitCount && !(size % 4);
}

void BMP_CT::assertInvalidClrTableSize() {
    std::cerr << "BMP_CT: Array size does not match number of colours." << std::endl;
}

void BMP_CT::assertInvalidNewClrTableSize() {
    std::cerr << "BMP_CT: New colour table size too large or is not a multiple of 4." << std::endl;
}

void BMP_CT::assertClrTableIndexOutOfRange() {
    std::cerr << "BMP_CT: Colour table index out of range, nothing was done." << std::endl;
}

void BMP_CT::setColourTable(const uint32_t &index, const uint8_t &r, const uint8_t &g, const uint8_t &b) {
    const uint16_t offset = 4 * index;
    if (index >= colourTable.size()) {
        assertClrTableIndexOutOfRange();
        return;
    }

    colourTable[offset] = b;
    colourTable[offset + 1] = g;
    colourTable[offset + 2] = r;
}

bool BMP_CT::setColourTable(const std::vector<uint8_t> &table) {
    if (!validNewClrTableSize(table.size())) {
        assertInvalidNewClrTableSize();
        return false;
    }

    colourTable = table;
    infoHeader.biClrUsed = colourTable.size();

    return true;
}

std::vector<uint8_t> BMP_CT::getColourTable() const {
    return colourTable;
}

void BMP_CT::getColourTable(const uint32_t &index, uint8_t &r, uint8_t &g, uint8_t &b) const {
    const uint8_t offset = 4 * index;
    if (offset + 3 >= colourTable.size()) {
        assertClrTableIndexOutOfRange();
        return;
    }

    b = colourTable[offset];
    g = colourTable[offset + 1];
    r = colourTable[offset + 2];
}
