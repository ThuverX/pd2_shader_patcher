#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <vector>


std::vector<unsigned char> file_read(std::string path);
void file_write(std::string path, std::vector<unsigned char> data);
std::vector<std::string> get_mod_folders();
std::vector<std::string> find_files(std::string path, std::string name);