#ifndef BMP_BMP_16_BIT_H
#define BMP_BMP_16_BIT_H

#include "bmp_with-bm.h"
#include <cstdint>
#include <string>
#include <vector>

//TODO: Not tested

/**
 * \brief Class for 16-bit BMP images, defaults to RGB555 if bitmask is not used.
 *
 * The bitmask will be used if it is not empty (i.e. empty() returns false), any arbitrary bitmask can be used.
 *
 * Image with invalid bitmasks will be treated as RGB565
 */
class BMP_16bit: public BMP_BM {
	public:
		/**
		 * \brief Constructor for reading from a file.
		 *
		 * @param filename[in] The filename
		 */
		explicit BMP_16bit(const std::string& filename);

		/// Copy constructor
		BMP_16bit(const BMP_16bit& n) = default;

		/**
		 * \brief Constructor for generating a new greyscale BMP object.
		 *
		 * Defails on the formatting of bitmask please refer to the comment on the private bitmask member.
		 *
		 * @param x[in] Width, positive only
		 * @param y[in] Height, negative value means flipped row-order
		 * @param background[in] 16-bit value, format depends on bitmask
		 * @param bitmask[in] Vector containing the bit mask, default to empty bitmask (no bitmask used)
		 */
		BMP_16bit(const int32_t& w, const int32_t& h, const uint16_t& background = 0, std::vector<uint32_t> bm = std::vector<uint32_t>());

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
		uint16_t& operator[](const size_t& index);
		const uint16_t& operator[](const size_t& index) const;
		///@}

		///@{
		/**
		 * \brief Access pixel at (x, y), user does not need to handle row-order.
		 *
		 * @param x[in] x
		 * @param y[in] y
		 * @return Reference to the element
		 */
		uint16_t& operator()(const uint32_t& x, const uint32_t& y);
		const uint16_t& operator()(const uint32_t& x, const uint32_t& y) const;
		///@}

		/**
		 * \brief Assignment operator.
		 *
		 * @param n[in] To be copied to the this
		 * @return Reference to this
		 */
		BMP_16bit& operator=(const BMP_16bit& n) = default;

		///@{
		/// Preset bitmask for common pixel formats
		static const std::vector<uint32_t> RGB565_bitmask;
		static const std::vector<uint32_t> RGB555_bitmask;
		///@}

		/// Size in bytes for 1 pixel
		static const uint8_t pixel_size = 2;

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint16_t> img;
};

#endif //BMP_BMP_16_BIT_H
