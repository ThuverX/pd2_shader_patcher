#include "file.h"

std::vector<unsigned char> file_read(std::string path) {
    std::ifstream file(path, std::ios::binary );

    if (file.is_open()) {
        return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
    }

    std::cout << "Failed to open the file: " << path << std::endl;
}

void file_write(std::string path, std::vector<unsigned char> data) {
    std::ofstream outfile (path,std::ofstream::binary);

    outfile.write((char*)data.data(), data.size());

    outfile.close();
}