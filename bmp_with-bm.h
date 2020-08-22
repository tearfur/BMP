#ifndef BMP_BMP_WITH_BM_H
#define BMP_BMP_WITH_BM_H

#include "bmp.h"
#include <vector>
#include <cstdint>

/**
 * \brief A base class for colour formats that uses bitmask.
 *
 * Should not be constructed by itself, as it is an incomplete object.
 */
class BMP_BM: public BMP {
	protected:
		/// Constructor to delegate to BMP class
		explicit BMP_BM(const std::string& filename);

		/// Copy constructor to delegate to BMP class
		BMP_BM(const BMP_BM& n) = default;

		/// Constructor to delegate to BMP class
		BMP_BM(const int32_t& w, const int32_t& h, std::vector<uint32_t> bm = std::vector<uint32_t>());

		/// Intermediate function to perform some colour table specific operations
		bool save(std::ofstream& f) const;

		/**
		 * Validate a bitmask and convert it to the the library's format if it is valid
		 * @param bm[in, out] The bitmask object
		 * @return Whether it is valid
		 */
		static bool convertBitmask(std::vector<uint32_t>& bm);

		/**
		 * \brief Bitmask vector, ignored if empty (i.e. empty() returns true)
		 *
		 * The 1-bits indicate that those bits will be used for those colour. The must be contiguous and non-overlapping.
		 *
		 * These bits follow the order RGB. i.e. The MSBs will be R, followed by G, then lastly B.
		 *
		 * 0: Red mask
		 * 1: Green mask
		 * 2. Blue mask
		 */
		std::vector<uint32_t> bitmask;
};

#endif //BMP_BMP_WITH_BM_H
