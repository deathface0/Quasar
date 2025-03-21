#include "RLE.h"

std::vector<unsigned char> RLE::compressData(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> compressedData;

    size_t i = 0;
    while (i < input.size()) {
        unsigned char currentByte = input[i];
        size_t count = 1;

        while (i + count < input.size() && input[i + count] == currentByte && count < 255) {
            count++;
        }

        compressedData.push_back(currentByte);
        compressedData.push_back(static_cast<unsigned char>(count));

        i += count;
    }

    return compressedData;
}

std::vector<unsigned char> RLE::decompressData(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> decompressedData;

    size_t i = 0;
    while (i < input.size()) {
        unsigned char currentByte = input[i];
        unsigned char count = input[i + 1];

        for (size_t j = 0; j < count; ++j) {
            decompressedData.push_back(currentByte);
        }

        i += 2;
    }

    return decompressedData;
}