//
// Created by HoYuYat on 8/11/2019.
//

#ifndef BMP_BMP_1_BIT_H
#define BMP_BMP_1_BIT_H

#include "bmp_with-ct.h"
#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

/**
 * \brief Class for 1-bit BMP, usually black and white BMP.
 */
class BMP_1bit: public BMP_CT {
	public:
		/**
		 * \brief Constructor for reading from a file.
		 *
		 * @param filename[in] The filename
		 */
		explicit BMP_1bit(const std::string& filename);

		/// Copy constructor
		BMP_1bit(const BMP_1bit& n) = default;

		/**
		 * \brief Constructor for generating a new black and white BMP object.
		 *
		 * @param x[in] Width, positive only
		 * @param y[in] Height, negative value means flipped row-order
		 * @param background[in] True for white background, false (default) for black background
		 */
		BMP_1bit(const int32_t& w, const int32_t& h, bool background = false);

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
		uint8_t& operator()(const uint32_t& x, const uint32_t& y);
		const uint8_t& operator()(const uint32_t& x, const uint32_t& y) const;
		///@}

		/**
		 * \brief Assignment operator.
		 *
		 * @param n[in] To be copied to the this
		 * @return Reference to this
		 */
		BMP_1bit& operator=(const BMP_1bit& n) = default;

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint8_t> img;
};

#endif //BMP_BMP_1_BIT_H
