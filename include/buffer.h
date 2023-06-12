#pragma once

#include <cstdint>
#include <vector>

int32_t read_int32_le(std::vector<unsigned char>& buffer, int32_t& offset);
int8_t read_int8_le(std::vector<unsigned char>& buffer, int32_t& offset);
int64_t read_int64_le(std::vector<unsigned char>& buffer, int32_t& offset);
std::vector<unsigned char> read_buffer(std::vector<unsigned char>& buffer, int32_t& offset, int32_t size);

void write_int32_le(std::vector<unsigned char>& buffer, int32_t& offset, int32_t value);
void write_int8_le(std::vector<unsigned char>& buffer, int32_t& offset, int8_t value);
void write_int64_le(std::vector<unsigned char>& buffer, int32_t& offset, int64_t value);
void write_buffer(std::vector<unsigned char>& buffer, int32_t& offset, std::vector<unsigned char>& value);
void write_buffer(std::vector<unsigned char>& buffer, int32_t& offset, std::vector<unsigned char>& value, int32_t size);

void shrink(std::vector<unsigned char>& buffer, int32_t& offset);