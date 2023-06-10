#include "buffer.h"

int32_t read_int32_le(std::vector<unsigned char> &buffer, int32_t& offset) {
    int32_t value = 0;

    value |= buffer[offset + 0] << 0;
    value |= buffer[offset + 1] << 8;
    value |= buffer[offset + 2] << 16;
    value |= buffer[offset + 3] << 24;

    offset += 4;

    return value;
}

int8_t read_int8_le(std::vector<unsigned char> &buffer, int32_t& offset) {
    int8_t value = 0;

    value |= buffer[offset + 0] << 0;

    offset += 1;

    return value;
}

int64_t read_int64_le(std::vector<unsigned char> &buffer, int32_t& offset) {
    int64_t value = *reinterpret_cast<int64_t*>(buffer.data() + offset);

    offset += 8;

    return value;
}

std::vector<unsigned char> read_buffer(std::vector<unsigned char> &buffer, int32_t& offset, int32_t size) {
    std::vector<unsigned char> value(size);

    std::copy(buffer.begin() + offset, buffer.begin() + offset + size, value.begin());

    offset += size;

    return value;
}

void write_int32_le(std::vector<unsigned char> &buffer, int32_t& offset, int32_t value) {
    buffer[offset + 0] = (value >> 0) & 0xFF;
    buffer[offset + 1] = (value >> 8) & 0xFF;
    buffer[offset + 2] = (value >> 16) & 0xFF;
    buffer[offset + 3] = (value >> 24) & 0xFF;

    offset += 4;
}

void write_int8_le(std::vector<unsigned char> &buffer, int32_t& offset, int8_t value) {
    buffer[offset + 0] = (value >> 0) & 0xFF;

    offset += 1;
}

void write_int64_le(std::vector<unsigned char> &buffer, int32_t& offset, int64_t value) {
    *reinterpret_cast<int64_t*>(buffer.data() + offset) = value;

    offset += 8;
}

void write_buffer(std::vector<unsigned char> &buffer, int32_t& offset, std::vector<unsigned char> &value) {
    std::copy(value.begin(), value.end(), buffer.begin() + offset);

    offset += value.size();
}

void write_buffer(std::vector<unsigned char> &buffer, int32_t& offset, std::vector<unsigned char> &value, int32_t size) {
    std::copy(value.begin(), value.begin() + size, buffer.begin() + offset);

    offset += size;
}

void shrink(std::vector<unsigned char> &buffer, int32_t& offset) {
    buffer.resize(offset);
}