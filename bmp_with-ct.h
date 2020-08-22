//
// Created by HoYuYat on 8/11/2019.
//

#ifndef BMP_BMP_WITH_CT_H
#define BMP_BMP_WITH_CT_H

#include "bmp.h"
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

/**
 * \brief Intermediate base class to contain all common functions of BMP formats with a colour table
 */
class BMP_CT: public BMP {
	protected:
		/// Constructor to delegate to BMP class
		explicit BMP_CT(const std::string& filename);

		/// Copy constructor to delegate to BMP class
		BMP_CT(const BMP_CT& n) = default;

		/// Constructor to delegate to BMP class
		BMP_CT(const int32_t& w, const int32_t& h);

		/// Read colour table
		void readClrTable(std::ifstream& f);

		/// Intermediate function to perform some colour table specific operations
		bool save(std::ofstream& f) const;

		/// Given the current conditions, is this colour table size valid?
		bool validClrTableSize(const uint32_t& size) const;

		/// Is this brand new colour table size valid?
		bool validNewClrTableSize(const uint32_t& size) const;

		/// Outputs an error message if the current colour table size is invalid
		void assertInvalidClrTableSize() const;

		/// Outputs an error message if new colour table size is invalid
		void assertInvalidNewClrTableSize() const;

		/// Outputs an error message if the out of range index is tried
		void assertClrTableIndexOutOfRange() const;

		/**
		 * \brief Modify the colour table.
		 *
		 * @param index[in] The colour index you want to modify
		 * @param red[in] Red value
		 * @param green[in] Green value
		 * @param blue[in] Blue value
		 */
		void setColourTable(const uint32_t& index, const uint8_t& r, const uint8_t& g, const uint8_t& b);

		/**
		 * \brief Replace the colour table with the one provided, modifies infoHeader.biClrUsed to match the new size too.
		 *
		 * Will return false and do nothing if size > 2 ^ colour-depth * 4 or size is not divisible by 4.
		 *
		 * @param table[in] Reference to a vector containing the new table
		 * @return Whether the new table was adopted
		 */
		bool setColourTable(const std::vector<uint8_t>& table);

		/**
		 * \brief Copies the colour table to another vector for manipulation.
		 *
		 * @return A copy of the table
		 */
		std::vector<uint8_t> getColourTable() const;

		/**
		 * \brief Copies the colour table at index to 3 variables.
		 *
		 * @param index[in] The colour index you want to access
		 * @param r[out] The red value
		 * @param g[out] The green value
		 * @param b[out] The blue value
		 */
		void getColourTable(const uint32_t& index, uint8_t& r, uint8_t& g, uint8_t& b) const;

		/// Assignment operator
		BMP_CT& operator=(const BMP_CT& n) = default;

		/// Colour table, stored as dynamic array.
		std::vector<uint8_t> colourTable;
};

#endif //BMP_BMP_WITH_CT_H
