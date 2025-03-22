#pragma once

#pragma once

#include <vector>
#include <stdexcept>
#include <fstream>
#include <lz4/lz4.h>

namespace LZ4 {
	void compressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size = 16384);
	void decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size = 16384);
}
