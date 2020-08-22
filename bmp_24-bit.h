//
// Created by HoYuYat on 8/11/2019.
//

#ifndef BMP_BMP_24_BIT_H
#define BMP_BMP_24_BIT_H

#include "bmp.h"
#include <string>
#include <vector>
#include <cstddef>

/**
 * \brief A class for 24-bit RGB888 BMP images.
 */
class BMP_24bit: public BMP {
	public:
		/**
		 * \brief Constructor for reading from a file.
		 *
		 * @param filename[in] The filename
		 */
		explicit BMP_24bit(const std::string& filename);

		/// Copy constructor
		BMP_24bit(const BMP_24bit& n) = default;

		/**
		 * \brief Constructor for generating a new greyscale BMP object.
		 *
		 * @param x[in] Width, positive only
		 * @param y[in] Height, negative value means flipped row-order
		 * @param background[in] 0 - 0xFFFFFF, RGB888 value
		 */
		BMP_24bit(const int32_t& w, const int32_t& h, const uint32_t& background = 0);

		/**
		 * \brief Saves the object to a BMP file.
		 *
		 * @param filename[in] Output filename
		 * @return Whether the BMP file has been saved successfully
		 */
		bool save(const std::string& filename) const;

		/**
		 * @{
		 *
		 * \brief Pixel accessors function.
		 */
		void setPixel(const std::size_t& index, const uint32_t& colour);
		void setPixel(const int32_t& x, const int32_t& y, const uint32_t& colour);

		uint32_t getPixel(const std::size_t& index) const;
		uint32_t getPixel(const int32_t& x, const int32_t& y) const;
		///@}

		/**
		 * @{
		 *
		 * \brief Pixel access for specific colours
		 */
		uint8_t& red(const std::size_t& index);
		const uint8_t& red(const std::size_t& index) const;

		uint8_t& red(const uint32_t& x, const uint32_t& y);
		const uint8_t& red(const uint32_t& x, const uint32_t& y) const;

		uint8_t& green(const std::size_t& index);
		const uint8_t& green(const std::size_t& index) const;

		uint8_t& green(const uint32_t& x, const uint32_t& y);
		const uint8_t& green(const uint32_t& x, const uint32_t& y) const;

		uint8_t& blue(const std::size_t& index);
		const uint8_t& blue(const std::size_t& index) const;

		uint8_t& blue(const uint32_t& x, const uint32_t& y);
		const uint8_t& blue(const uint32_t& x, const uint32_t& y) const;
		///@}

		/// Size in bytes for 1 pixel
		static const uint8_t pixel_size = 3;

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint8_t> img;

		/**
		 * @{
		 *
		 * \brief Internal index functions
		 */
		std::size_t getInternalIndex(const std::size_t& index) const;
		std::size_t getInternalRedIndex(const std::size_t& index) const;
		std::size_t getInternalGreenIndex(const std::size_t& index) const;
		std::size_t getInternalBlueIndex(const std::size_t& index) const;

		std::size_t getInternalIndex(const int32_t& x, const int32_t& y) const;
		std::size_t getInternalRedIndex(const int32_t& x, const int32_t& y) const;
		std::size_t getInternalGreenIndex(const int32_t& x, const int32_t& y) const;
		std::size_t getInternalBlueIndex(const int32_t& x, const int32_t& y) const;
		///@}
};

#endif //BMP_BMP_24_BIT_H
