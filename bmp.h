#ifndef BMP_BMP_H
#define BMP_BMP_H

#include <cstdint>
#include <cstddef>
#include <climits>
#include <string>
#include <fstream>

/**
 * @mainpage tearfur's BMP Library
 *
 * A couple of header files that reads and writes uncompressed 1-bit, 8-bit, 16-bit, 24-bit, and 32-bit BMPv3 files.
 * Code for BMP with bit-masks are written, but not yet tested.
 *
 * It only supports the "BM" format.
 *
 * Written for GNU C++11.
 *
 * Tested bit-depths:
 * - 1-bit
 * - 8-bit
 * - 24-bit
 */

/**
 * @brief An base class for different colour formats to inherit from.
 *
 * Should not be constructed, it does not form a valid BMP object by itself
 */
class BMP {
protected:
    /// Copy constructor
    BMP(const BMP &n) = default;

    /**
     * @brief Reads from a BMP file and sets the header values.
     */
    explicit BMP(const std::string &filename);

    /**
     * @brief Fills in the headers for a newly constructed BMP object.
     *
     * Pass a negative y value for flipped row-order.
     */
    BMP(const int32_t &w, const int32_t &h);

    /// To get the size in bytes for each row in memory.
    size_t getRowSize() const;

    /// Performs all the common functions for save.
    bool save(std::ofstream &f) const;

    /// Returns the index for a certain x, y.
    size_t getIndex(const int32_t &x, const int32_t &y) const;

    /**
     * @{
     * @brief Assert: Invalid img index
     */
    void assertInvalidIndex(const size_t &index) const;

    void assertInvalidIndex(const int32_t &x, const int32_t &y) const;
    ///@}

    /// Assignment operator
    BMP &operator=(const BMP &n) = default;

private:
    /**
     * @brief Default constructor.
     *
     * Sets bfReserved1, bfReserved2, biXPelsPerMeter, biYPelsPerMeter to 0, and biPlanes to 1, as required by standard.
     *
     * Sets bfType to "BM", bfSize to 0, biSize to 40, biClrImportant to 0.
     */
    BMP();

    /// Outputs error message for invalid index
    static void assertInvalidIndex();

public:
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
    struct FileHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1, bfReserved2;
        uint32_t bfOffBits;
    };

    static const uint8_t fileHeaderSize = (16 + 32 + 16 + 16 + 32) / 8;

    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
    struct InfoHeader {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight; // Flipped row-order if negative
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter, biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

protected:
    /// BITMAPFILEHEADER
    FileHeader fileHeader;

    /// BITMAPINFOHEADER
    InfoHeader infoHeader;

public:
    /**
     * @brief Accessor function to get the bitmap file header
     *
     * @return Constant reference to the file header
     */
    const FileHeader &getFileHeader() const;

    /**
     * @brief Accessor function to get the bitmap info header
     *
     * @return Constant reference to the info header
     */
    const InfoHeader &getInfoHeader() const;

    ///@{
    /// Check for valid img index
    bool validIndex(const size_t &index) const;

    bool validIndex(const int32_t &x, const int32_t &y) const;
    ///@}
};

#endif //BMP_BMP_H
