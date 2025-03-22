#include "deflate.h"

void Deflate::compressData(const std::string& inputFile, const std::string& outputFile, int level, size_t chunk_size) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);

    if (!in || !out) throw std::runtime_error("Failed to open files");

    z_stream stream{};
    if (deflateInit(&stream, level) != Z_OK) {
        throw std::runtime_error("Failed to initialize compression.");
    }

    std::vector<unsigned char> inBuffer(chunk_size);
    std::vector<unsigned char> outBuffer(chunk_size);

    int ret;
    do {
        in.read(reinterpret_cast<char*>(inBuffer.data()), chunk_size);
        stream.next_in = inBuffer.data();
        stream.avail_in = in.gcount();

        do {
            stream.next_out = outBuffer.data();
            stream.avail_out = outBuffer.size();

            ret = deflate(&stream, in.eof() ? Z_FINISH : Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&stream);
                throw std::runtime_error("Compression failed.");
            }

            out.write(reinterpret_cast<char*>(outBuffer.data()), chunk_size - stream.avail_out);

        } while (stream.avail_out == 0);

    } while (ret != Z_STREAM_END);

    deflateEnd(&stream);
}

void Deflate::decompressData(const std::string& inputFile, const std::string& outputFile, size_t chunk_size) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);

    if (!in || !out) throw std::runtime_error("Failed to open files");

    z_stream strm{};
    if (inflateInit(&strm) != Z_OK) {
        throw std::runtime_error("Failed to initialize decompression.");
    }

    std::vector<unsigned char> inBuffer(chunk_size);
    std::vector<unsigned char> outBuffer(chunk_size);

    int ret;
    do {
        in.read(reinterpret_cast<char*>(inBuffer.data()), chunk_size);
        strm.next_in = inBuffer.data();
        strm.avail_in = in.gcount();

        do {
            strm.next_out = outBuffer.data();
            strm.avail_out = outBuffer.size();

            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                inflateEnd(&strm);
                throw std::runtime_error("Decompression failed.");
            }

            out.write(reinterpret_cast<char*>(outBuffer.data()), chunk_size - strm.avail_out);

        } while (strm.avail_out == 0);

    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);
}