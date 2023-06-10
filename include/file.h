#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

std::vector<unsigned char> file_read(std::string path);
void file_write(std::string path, std::vector<unsigned char> data);