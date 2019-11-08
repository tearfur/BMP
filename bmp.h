//
// Created by HoYuYat on 8/11/2019.
//

#ifndef BMP_BMP_H
#define BMP_BMP_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <fstream>

/**
 * \mainpage Yat's BMP Library
 *
 * A couple of header files that reads and writes uncompressed 1-bit, 8-bit, 16-bit, 24-bit, and 32-bit BMPv3 files.
 * BMP with bit-masks are supported.
 *
 * It only supports the "BM" format.
 *
 * Ensured to be working with GCC, should be working with MSVC too. Does not support polymorphism.
 */

/**
 * \brief An base class for different colour formats to inherit from.
 *
 * Should not be constructed, it does not form a valid BMP object by itself
 */
class BMP {
	protected:
		/// Copy constructor
		BMP(const BMP& n) = default;

		/**
		 * \brief Reads from a BMP file and sets the header values.
		 */
		explicit BMP(const std::string& filename);

		/**
		 * \brief Fills in the headers for a newly constructed BMP object.
		 *
		 * Pass a negative y value for flipped row-order.
		 */
		BMP(const int32_t& w, const int32_t& h);

		/// To get the size in bytes for each row in memory.
		std::size_t getRowSize() const;

		/// Performs all the common functions for save.
		bool save(std::ofstream& f) const;

		/// Returns the index for a certain x, y.
		std::size_t getIndex(const uint32_t& x, const uint32_t& y) const;

		///@{
		/// Check for valid img index
		bool validIndex(const std::size_t& index) const;
		bool validIndex(const uint32_t& x, const uint32_t& y) const;
		///@}

		/// Assert: Invalid img index
		void assertInvalidIndex() const;

		/// Assignment operator
		BMP& operator=(const BMP& n) = default;

	private:
		/**
		 * \brief Default constructor.
		 *
		 * Sets bfReserved1, bfReserved2, biXPelsPerMeter, biYPelsPerMeter to 0, and biPlanes to 1, as required by standard.
		 *
		 * Sets bfType to "BM", bfSize to 0, biSize to 40, biClrImportant to 0.
		 */
		BMP();

	protected:
#pragma pack(push, 1)
		/// BITMAPFILEHEADER
		struct {
			uint16_t bfType;
			uint32_t bfSize;
			uint16_t bfReserved1, bfReserved2;
			uint32_t bfOffBits;
		} fileHeader;

		///BITMAPINFOHEADER
		struct {
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
		} infoHeader;
#pragma pack(pop)
};

#endif //BMP_BMP_H