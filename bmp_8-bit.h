//
// Created by HoYuYat on 8/11/2019.
//

#ifndef BMP_BMP_8_BIT_H
#define BMP_BMP_8_BIT_H

#include "bmp_with-ct.h"
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

/**
 * \brief Class for 8-bit BMP images, usually greyscale
 */
class BMP_8bit: public BMP_CT {
	public:
		/**
		 * \brief Constructor for reading from a file.
		 *
		 * @param filename[in] The filename
		 */
		explicit BMP_8bit(const std::string& filename);

		/// Copy constructor
		BMP_8bit(const BMP_8bit& n) = default;

		/**
		 * \brief Constructor for generating a new greyscale BMP object.
		 *
		 * @param x[in] Width, positive only
		 * @param y[in] Height, negative value means flipped row-order
		 * @param background[in] 0-255, defaulted to 0
		 */
		BMP_8bit(const int32_t& w, const int32_t& h, const uint8_t& background = 0);

		/**
		 * \brief Saves the object to a BMP file.
		 *
		 * @param filename[in] Output filename
		 * @return Whether the BMP file has been saved successfully
		 */
		bool save(const std::string& filename) const;

		///@{
		/**
		 * \brief Operator[] for accessing img elements.
		 *
		 * @param index[in] img[index]
		 * @return Reference to the element
		 */
		uint8_t& operator[](const std::size_t& index);
		const uint8_t& operator[](const std::size_t& index) const;
		///@}

		///@{
		/**
		 * \brief Access pixel at (x, y), user does not need to handle row-order.
		 *
		 * @param x[in] x
		 * @param y[in] y
		 * @return Reference to the element
		 */
		uint8_t& operator()(const int32_t& x, const int32_t& y);
		const uint8_t& operator()(const int32_t& x, const int32_t& y) const;
		///@}

		/**
		 * \brief Assignment operator.
		 *
		 * @param n[in] To be copied to the this
		 * @return Reference to this
		 */
		BMP_8bit& operator=(const BMP_8bit& n) = default;

		/**
		 * \brief Converts a greyscale value to an RGB888 value using the default colour table.
		 * @param grey[in] Greyscale value
		 * @return RGB888 value, hex format: XX RR GG BB
		 */
		static uint32_t toRGB888(const uint8_t& grey);

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint8_t> img;
};

#endif //BMP_BMP_8_BIT_H
