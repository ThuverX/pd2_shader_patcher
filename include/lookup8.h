#include <cstdint>
#include <string>
#include <vector>

int64_t dieselHashUTF8(const std::string str);
int64_t dieselHash(std::vector<unsigned char> buffer);
int64_t lookup8(std::vector<unsigned char> buffer, int64_t level);