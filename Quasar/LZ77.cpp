#include "LZ77.h"


std::vector<unsigned char> LZ77::compressData(const std::vector<unsigned char>& data, int level) {
    z_stream stream{};

    if (deflateInit2(&stream, level, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error("Failed to initialize compression.");
    }

    stream.next_in = const_cast<Bytef*>(data.data());
    stream.avail_in = data.size();

    std::vector<unsigned char> compressedData(data.size() * 1.1 + 12); // Approximate compression size
    stream.next_out = compressedData.data();
    stream.avail_out = compressedData.size();

    int ret = deflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&stream);
        throw std::runtime_error("Compression failed.");
    }

    compressedData.resize(stream.total_out);
    deflateEnd(&stream);

    return compressedData;
}

std::vector<unsigned char> LZ77::decompressData(const std::vector<unsigned char>& compressedData, size_t chunk_size) {
    std::vector<unsigned char> decompressedData;

    z_stream stream{};
    if (inflateInit2(&stream, -MAX_WBITS) != Z_OK) {
        throw std::runtime_error("Failed to initialize decompression.");
    }

    stream.next_in = const_cast<Bytef*>(compressedData.data());
    stream.avail_in = compressedData.size();

    std::vector<unsigned char> buffer(chunk_size);
    int ret;

    do {
        stream.next_out = buffer.data();
        stream.avail_out = buffer.size();

        ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            inflateEnd(&stream);
            throw std::runtime_error("Zlib inflate stream error.");
        }
        if (ret == Z_BUF_ERROR) {
            break;
        }

        size_t bytesDecompressed = buffer.size() - stream.avail_out;
        decompressedData.insert(decompressedData.end(), buffer.begin(), buffer.begin() + bytesDecompressed);

    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    return decompressedData;
}