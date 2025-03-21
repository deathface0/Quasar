#pragma once

#include <vector>
#include <stdexcept>
#include <zlib/zlib.h>

namespace RLE {
	std::vector<unsigned char> compressData(const std::vector<unsigned char>& data);
	std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData);
}
