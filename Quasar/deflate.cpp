#include "deflate.h"

std::vector<unsigned char> Deflate::compressData(const std::vector<unsigned char>& data, int level) {
    z_stream stream{};

    if (deflateInit(&stream, level) != Z_OK) {
        throw std::runtime_error("Failed to initialize compression.");
    }

    stream.next_in = const_cast<Bytef*>(data.data());
    stream.avail_in = data.size();

    std::vector<unsigned char> compressedData(compressBound(data.size()));
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

std::vector<unsigned char> Deflate::decompressData(const std::vector<unsigned char>& compressedData, size_t chunk_size) {
    std::vector<unsigned char> decompressedData;

    z_stream strm{};
    strm.next_in = const_cast<Bytef*>(compressedData.data());
    strm.avail_in = compressedData.size();

    if (inflateInit(&strm) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib inflate.");
    }

    std::vector<unsigned char> buffer(chunk_size);
    int ret;

    do {
        strm.next_out = buffer.data();
        strm.avail_out = buffer.size();

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            inflateEnd(&strm);
            throw std::runtime_error("Zlib inflate stream error.");
        }

        size_t bytesDecompressed = buffer.size() - strm.avail_out;
        decompressedData.insert(decompressedData.end(), buffer.begin(), buffer.begin() + bytesDecompressed);

    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);
    return decompressedData;
}
