#pragma once

#include "patcher.h"
#include <mxml.h>
#include <vector>
#include <string>

std::vector<ShaderPatch*> getPatchesFromXML(std::string file);