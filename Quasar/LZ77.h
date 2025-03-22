#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <zlib/zlib.h> 

namespace LZ77 {
	void compressData(const std::string& inputFile, const std::string& outputFile, int level = Z_DEFAULT_COMPRESSION, size_t chunk_size = 16384);
	void decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size = 16384);
}
