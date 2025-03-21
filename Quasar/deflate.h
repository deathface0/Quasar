#pragma once

#include <vector>
#include <stdexcept>
#include <zlib/zlib.h>

namespace Deflate {
	std::vector<unsigned char> compressData(const std::vector<unsigned char>& data, int level = Z_DEFAULT_COMPRESSION);
	std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData, size_t chunk_size = 16384);
}
