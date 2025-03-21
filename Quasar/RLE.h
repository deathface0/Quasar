#pragma once

#include <fstream>
#include <vector>
#include <stdexcept>

namespace RLE {
	void compressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size = 16384);
	void decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size = 16384);
}
