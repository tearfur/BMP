#ifndef BMP_BMP_32_BIT_H
#define BMP_BMP_32_BIT_H

#include "bmp_with-bm.h"
#include <cstdint>

/**
 * \brief Class for 32-bit BMP images, defaults to RGB888 if bitmask is not used.
 *
 * The bitmask will be used if it is not empty (i.e. empty() returns false), any arbitrary bitmask can be used.
 *
 * Image with invalid bitmasks will be treated as RGB565
 */
class BMP_32bit: public BMP_BM {
	public:
		/**
		 * \brief Constructor for reading from a file.
		 *
		 * @param filename[in] The filename
		 */
		explicit BMP_32bit(const std::string& filename);

		/// Copy constructor
		BMP_32bit(const BMP_32bit& n) = default;

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
		BMP_32bit(const int32_t& w, const int32_t& h, const uint32_t& background = 0, std::vector<uint32_t> bm = std::vector<uint32_t>());

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
		uint32_t& operator[](const std::size_t& index);
		const uint32_t& operator[](const std::size_t& index) const;
		///@}

		///@{
		/**
		 * \brief Access pixel at (x, y), user does not need to handle row-order.
		 *
		 * @param x[in] x
		 * @param y[in] y
		 * @return Reference to the element
		 */
		uint32_t& operator()(const uint32_t& x, const uint32_t& y);
		const uint32_t& operator()(const uint32_t& x, const uint32_t& y) const;
		///@}

		/**
		 * \brief Assignment operator.
		 *
		 * @param n[in] To be copied to the this
		 * @return Reference to this
		 */
		BMP_32bit& operator=(const BMP_32bit& n) = default;

		///@{
		/// Preset bitmask for common pixel formats
		static const std::vector<uint32_t> RGB888_bitmask;
		static const std::vector<uint32_t> RGB101010_bitmask;
		///@}

		/// Size in bytes for 1 pixel
		static const uint8_t pixel_size = 4;

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint32_t> img;
};

#endif //BMP_BMP_32_BIT_H
