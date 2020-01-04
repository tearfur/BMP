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
		 * @param background[in] 0 - 2^24-1, RGB888 value
		 */
		BMP_24bit(const int32_t& w, const int32_t& h, const uint32_t& background = 0);

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

		/// Size in bytes for 1 pixel
		static const uint8_t pixel_size = 3;

	private:
		/// Vector for storing image data, stored in row-order.
		std::vector<uint32_t> img;

		/**
		 * @{
		 *
		 * \brief RGB accessors.
		 */
		/// Base class for RGB accessors.
		class Accessor {
			public:
				/// Returns the img index this accessor is bound to.
				std::size_t getIndex() const;

			protected:
				/// Constructor, accepts ptr to parent.
				explicit Accessor(BMP_24bit* const& ptr);

				/// Pointer to parent.
				BMP_24bit* const ptr;

				/// The img index this accessor is currently bound to.
				std::size_t index;
		};

		/// Red accessor
		class Red: public Accessor {
			public:
				/// Constructor, accepts ptr to parent
				explicit Red(BMP_24bit* const& ptr);

				///@{
				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=().
				 *
				 * @param index[in] The new index to be bound to
				 * @return Reference to *this
				 */
				Red& operator[](const std::size_t& index);

				/**
				 * \brief Returns the red value at index.
				 *
				 * @param index[in]
				 * @return The red value
				 */
				uint8_t operator[](const std::size_t& index) const;

				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=().
				 *
				 * @param x[in] x coordinate of the new index to be bound to
				 * @param y[in] y coordinate of the new index to be bound to
				 * @return Reference to *this
				 */
				Red& operator()(const uint32_t& x, const uint32_t& y);

				/**
				 * \brief Returns the red value at (x, y).
				 *
				 * @param x[in] x coordinate
				 * @param y[in] y coordinate
				 * @return The red value
				 */
				uint8_t operator()(const uint32_t& x, const uint32_t& y) const;
				///@}

				/**
				 * \brief Sets the red value of the pixel at index to value.
				 *
				 * @param value[in] New red value
				 * @return *this
				 */
				Red& operator=(const uint8_t& value);

				/**
				 * \brief Conversion operator.
				 *
				 * @return Red value
				 */
				operator uint8_t() const;

			private:
				/// @{
				/// Get red value
				uint8_t getValue() const;
				uint8_t getValue(const uint32_t& index) const;
				/// @}
		} red_accessor;

		/// Green accessor
		class Green: public Accessor {
			public:
				/// Constructor, accepts ptr to parent
				explicit Green(BMP_24bit* const& ptr);

				///@{
				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=()
				 *
				 * @param index[in] The new index to be bound to
				 * @return Reference to *this
				 */
				Green& operator[](const std::size_t& index);

				/**
				 * \brief Returns the green value at index
				 *
				 * @param index[in]
				 * @return The green value
				 */
				uint8_t operator[](const std::size_t& index) const;

				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=()
				 *
				 * @param x[in] x coordinate of the new index to be bound to
				 * @param y[in] y coordinate of the new index to be bound to
				 * @return Reference to *this
				 */
				Green& operator()(const uint32_t& x, const uint32_t& y);

				/**
				 * \brief Returns the green value at (x, y)
				 *
				 * @param x[in] x coordinate
				 * @param y[in] y coordinate
				 * @return The green value
				 */
				uint8_t operator()(const uint32_t& x, const uint32_t& y) const;
				///@}

				/**
				 * \brief Sets the green value of the pixel at index to value.
				 *
				 * @param value[in] New green value
				 * @return *this
				 */
				Green& operator=(const uint8_t& value);

				/**
				 * \brief Conversion operator.
				 *
				 * @return Green value
				 */
				operator uint8_t() const;

			private:
				/// @{
				/// Get green value
				uint8_t getValue() const;
				uint8_t getValue(const uint32_t& index) const;
				/// @}
		} green_accessor;

		class Blue: public Accessor {
			public:
				/// Constructor, accepts ptr to parent
				explicit Blue(BMP_24bit* const& ptr);

				///@{
				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=()
				 *
				 * @param index[in] The new index to be bound to
				 * @return Reference to *this
				 */
				Blue& operator[](const std::size_t& index);

				/**
				 * \brief Returns the blue value at index
				 *
				 * @param index[in]
				 * @return The blue value
				 */
				uint8_t operator[](const std::size_t& index) const;

				/**
				 * \brief Modifies index and returns a reference to *this in order to be used in conjunction with operator=()
				 *
				 * @param x[in] x coordinate of the new index to be bound to
				 * @param y[in] y coordinate of the new index to be bound to
				 * @return Reference to *this
				 */
				Blue& operator()(const uint32_t& x, const uint32_t& y);

				/**
				 * \brief Returns the blue value at (x, y)
				 *
				 * @param x[in] x coordinate
				 * @param y[in] y coordinate
				 * @return The blue value
				 */
				uint8_t operator()(const uint32_t& x, const uint32_t& y) const;
				///@}

				/**
				 * \brief Sets the blue value of the pixel at index to value.
				 *
				 * @param value[in] New blue value
				 * @return *this
				 */
				Blue& operator=(const uint8_t& value);

				/**
				 * \brief Conversion operator.
				 *
				 * @return Blue value
				 */
				operator uint8_t() const;

			private:
				/// @{
				/// Get green value
				uint8_t getValue() const;
				uint8_t getValue(const uint32_t& index) const;
				/// @}
		} blue_accessor;
		///@}

	public:
		/**
		 * @{
		 *
		 * @brief Wrapper functions for accessors
		 */
		Red& red(const std::size_t& index);
		uint8_t red(const std::size_t& index) const;

		Red& red(const uint32_t& x, const uint32_t& y);
		uint8_t red(const uint32_t& x, const uint32_t& y) const;

		Green& green(const std::size_t& index);
		uint8_t green(const std::size_t& index) const;

		Green& green(const uint32_t& x, const uint32_t& y);
		uint8_t green(const uint32_t& x, const uint32_t& y) const;

		Blue& blue(const std::size_t& index);
		uint8_t blue(const std::size_t& index) const;

		Blue& blue(const uint32_t& x, const uint32_t& y);
		uint8_t blue(const uint32_t& x, const uint32_t& y) const;
		///@}
};

#endif //BMP_BMP_24_BIT_H
