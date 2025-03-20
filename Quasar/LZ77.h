#pragma once

#include <vector>
#include <stdexcept>
#include <includes/zlib/zlib.h>

namespace LZ77 {
	std::vector<unsigned char> compressData(const std::vector <unsigned char>& data, int level = Z_BEST_COMPRESSION);
	std::vector<unsigned char> decompressData(const std::vector <unsigned char>& compressedData, size_t originalSize);
}
