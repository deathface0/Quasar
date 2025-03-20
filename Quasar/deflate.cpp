#include "deflate.h"

std::vector<unsigned char> Deflate::compressData(const std::vector<unsigned char>& data, int level) {
    z_stream stream{};
    stream.next_in = const_cast<Bytef*>(data.data());
    stream.avail_in = data.size();

    std::vector<unsigned char> compressedData(compressBound(data.size()));
    stream.next_out = compressedData.data();
    stream.avail_out = compressedData.size();

    if (deflateInit(&stream, level) != Z_OK) {
        throw std::runtime_error("Failed to initialize compression.");
    }

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw std::runtime_error("Compression failed.");
    }

    compressedData.resize(stream.total_out);
    deflateEnd(&stream);

    return compressedData;

}

std::vector<unsigned char> Deflate::decompressData(const std::vector<unsigned char>& compressedData, size_t originalSize) {
    z_stream stream{};
    stream.next_in = const_cast<Bytef*>(compressedData.data());
    stream.avail_in = compressedData.size();

    std::vector<unsigned char> decompressedData(originalSize);
    stream.next_out = decompressedData.data();
    stream.avail_out = decompressedData.size();

    if (inflateInit(&stream) != Z_OK) {
        throw std::runtime_error("Failed to initialize decompression.");
    }

    if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
        inflateEnd(&stream);
        throw std::runtime_error("Decompression failed.");
    }

    decompressedData.resize(stream.total_out);
    inflateEnd(&stream);

    return decompressedData;
}
