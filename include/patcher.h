#pragma once

#include "shaderpack.h"
#include <string>

struct ShaderPassSamplerPatch
{
	int32_t id;
	std::map<int32_t, int32_t> vars;
};

const int32_t SHADER_PATCH_TYPE_CODE = 0;
const int32_t SHADER_PATCH_TYPE_PRECOMPILED = 1;

struct ShaderPassShaderPatch {
    std::string path;
    std::string entry_point;
    int32_t type = 0; // code = 0, precompiled = 1
};

struct ShaderPassPatch
{
	std::map<int32_t, int32_t> render_states;
	std::vector<ShaderPassSamplerPatch*> samplers;
	ShaderPassShaderPatch* vertex_shader;
	ShaderPassShaderPatch* pixel_shader;
};

const int32_t SHADER_PATCH_MODE_REPLACE = 0;
const int32_t SHADER_PATCH_MODE_APPEND = 1;

const int32_t MAX_REF_COUNTER = 99999;

struct ShaderPatch
{
	std::string apply_to;
	int32_t mode;
	std::string name;
	std::map<std::string, std::vector<ShaderPassPatch*>> passes;
};

Object* GetItemByRef(ShaderPackage* package_ptr, int32_t ref_id);
bool ApplyShaderPatch(ShaderPackage* package_ptr, ShaderPatch* patch_ptr);
void addShaderPatch(ShaderPatch* patch);
void ApplyShaderHooks();