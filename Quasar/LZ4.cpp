#include "LZ4.h"

void LZ4::compressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size)
{
    std::ifstream input_file(inputFile, std::ios::binary);
    std::ofstream output_file(outputFile, std::ios::binary);

    if (!input_file || !output_file) {
        throw std::runtime_error("Error opening files.");
    }

    std::vector<char> input_buffer(chunk_size);
    std::vector<char> compressed_buffer(LZ4_compressBound(chunk_size));

    while (input_file.read(input_buffer.data(), chunk_size) || input_file.gcount() > 0) {
        int input_size = input_file.gcount();
        int max_compressed_size = LZ4_compressBound(input_size);

        compressed_buffer.resize(max_compressed_size);

        int compressed_size = LZ4_compress_default(input_buffer.data(), compressed_buffer.data(), input_size, max_compressed_size);

        if (compressed_size < 0) {
            throw std::runtime_error("Compression failed.");
        }

        // Write compressed data size
        output_file.write(reinterpret_cast<char*>(&compressed_size), sizeof(int));

        // Write compressed data
        output_file.write(compressed_buffer.data(), compressed_size);
    }

    input_file.close();
    output_file.close();
}


void LZ4::decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size)
{
    std::ifstream input_file(inputFile, std::ios::binary);
    std::ofstream output_file(outputFile, std::ios::binary);

    if (!input_file || !output_file) {
        throw std::runtime_error("Error opening files.");
    }

    std::vector<char> compressed_buffer(chunk_size);
    std::vector<char> output_buffer(chunk_size);

    int compressed_size;

    while (input_file.read(reinterpret_cast<char*>(&compressed_size), sizeof(int))) {
        if (compressed_size > static_cast<int>(compressed_buffer.size())) {
            compressed_buffer.resize(compressed_size);
        }

        input_file.read(compressed_buffer.data(), compressed_size);

        int decompressed_size = LZ4_decompress_safe(compressed_buffer.data(), output_buffer.data(), compressed_size, chunk_size);

        if (decompressed_size < 0) {
            throw std::runtime_error("Decompression failed.");
        }

        output_file.write(output_buffer.data(), decompressed_size);
    }

    input_file.close();
    output_file.close();
}
