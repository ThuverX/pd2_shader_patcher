#pragma once
#include <cstdint>
#include <map>
#include <vector>

struct ObjectHeader
{
	int32_t type;
	int32_t ref_id;
	int32_t len;
	int32_t pos;
};

struct Object
{
	ObjectHeader* header;
};

struct ObjectShader : Object
{
	std::map<uint64_t, std::vector<int32_t>> passes = std::map<uint64_t, std::vector<int32_t>>();
};

struct ObjectShaderLibrary : Object
{
	std::map<uint64_t, int32_t> render_templates = std::map<uint64_t, int32_t>();
};

struct StateVar
{
	int32_t id;
	int32_t b;
	int32_t value4;
	int64_t value8;
};

struct SamplerState
{
	int32_t id;
	std::vector<StateVar*> vars = std::vector<StateVar*>();
};

struct ObjectShaderPass : Object
{
	std::vector<StateVar*> render_states = std::vector<StateVar*>();
	std::vector<SamplerState*> sampler_states = std::vector<SamplerState*>();
	std::vector<unsigned char> vertex_shader = std::vector<unsigned char>();
	std::vector<unsigned char> pixel_shader = std::vector<unsigned char>();
};

const int32_t OBJECT_TYPE_SHADER = 0x7F3552D1;
const int32_t OBJECT_TYPE_SHADER_PASS = 0x214b1aaf;
const int32_t OBJECT_TYPE_SHADER_LIBRARY = 0x12812C1A;

struct ShaderPackage
{
	int32_t padding;
	int32_t count;
	std::vector<ObjectHeader*> headers;
	std::vector<Object*> objects;
};

ShaderPackage* ReadShaderPackage(std::vector<unsigned char>& buffer);
ObjectShader* ReadObjectShader(std::vector<unsigned char>& buffer, ObjectHeader* header_ptr);
ObjectShaderPass* ReadObjectShaderPass(std::vector<unsigned char>& buffer, ObjectHeader* header_ptr);
ObjectShaderLibrary* ReadObjectShaderLibrary(std::vector<unsigned char>& buffer, ObjectHeader* header_ptr);
StateVar* ReadStateVar(std::vector<unsigned char>& buffer, int32_t& offset);
SamplerState* ReadSamplerState(std::vector<unsigned char>& buffer, int32_t& offset);

ObjectShaderLibrary* GetShaderLibrary(ShaderPackage* package_ptr);

void WriteShaderPackage(std::vector<unsigned char>& buffer, ShaderPackage* package_ptr);
void WriteObjectShader(std::vector<unsigned char>& buffer, ObjectShader* object_ptr);
void WriteObjectShaderPass(std::vector<unsigned char>& buffer, ObjectShaderPass* object_ptr);
void WriteObjectShaderLibrary(std::vector<unsigned char>& buffer, ObjectShaderLibrary* object_ptr);
void WriteStateVar(std::vector<unsigned char>& buffer, int32_t& offset, StateVar* state_var_ptr);
void WriteSamplerState(std::vector<unsigned char>& buffer, int32_t& offset, SamplerState* sampler_state_ptr);