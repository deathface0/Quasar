#include "RLE.h"

void RLE::compressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size) {
    std::ifstream input_file(inputFile, std::ios::binary);
    std::ofstream output_file(outputFile, std::ios::binary);

    if (!input_file || !output_file) {
        throw std::runtime_error("Error opening files.");
    }

    // Process the file in chunks
    std::vector<char> buffer(chunk_size);

    char prev_char = 0;
    unsigned char count = 0;
    bool first_char = true;

    while (input_file) {
        // Read a chunk of data
        input_file.read(buffer.data(), chunk_size);
        size_t bytes_read = input_file.gcount();

        if (bytes_read == 0) break;

        // Process each byte in the buffer
        for (size_t i = 0; i < bytes_read; ++i) {
            char current_char = buffer[i];

            if (first_char) {
                prev_char = current_char;
                count = 1;
                first_char = false;
            }
            else if (current_char == prev_char && count < 255) {
                // Same character, increment count if we haven't reached max
                ++count;
            }
            else {
                // Different character or max count reached, write the run
                output_file.put(prev_char);
                output_file.put(count);

                // Start a new run
                prev_char = current_char;
                count = 1;
            }
        }
    }

    // Write the last run if there is one
    if (!first_char) {
        output_file.put(prev_char);
        output_file.put(count);
    }

    input_file.close();
    output_file.close();
}

void RLE::decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size) {
    std::ifstream input_file(inputFile, std::ios::binary);
    std::ofstream output_file(outputFile, std::ios::binary);

    if (!input_file || !output_file) {
        throw std::runtime_error("Error opening files.");
    }

    // Process the file in chunks (pairs of character and count)
    std::vector<char> buffer(chunk_size * 2);

    while (input_file) {
        // Read a chunk of data
        input_file.read(buffer.data(), chunk_size * 2);
        size_t bytes_read = input_file.gcount();

        if (bytes_read == 0) break;

        // Process each pair of bytes (character and count)
        for (size_t i = 0; i < bytes_read; i += 2) {
            if (i + 1 >= bytes_read) break; // Skip incomplete pair at the end

            char character = buffer[i];
            unsigned char count = static_cast<unsigned char>(buffer[i + 1]);

            // Write the run
            for (unsigned char j = 0; j < count; ++j) {
                output_file.put(character);
            }
        }
    }

    input_file.close();
    output_file.close();
}