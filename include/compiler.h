#pragma once

#include "superblt_flat.h"

#include <string>
#include <vector>

#include "D3Dcompiler.h"

std::vector<unsigned char> compile_shader(std::string sourcePath, std::string entryPoint, bool isVertex, bool useCache);