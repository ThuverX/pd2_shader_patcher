#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <superblt_flat.h>

#include "file.h"
#include "patcher.h"
#include "shaderpack.h"
#include <windows.h>

// void PatchShaders() {
//     std::vector<std::string> mod_folders = get_mod_folders();

//     std::vector<std::string> shadermod_files;

//     for(std::string mod_folder : mod_folders) {
//         std::vector<std::string> files = find_files(mod_folder, "shadermod.xml");

//         for(std::string file : files) {
//             shadermod_files.push_back(file);
//         }
//     }
// }

void PatchShaderFile(std::vector<uint8_t>* file)
{
	std::vector<unsigned char> buffer = file_read("X:/shader_tool_cpp/post.d3d9.modified.shaders");

	file->clear();
	file->insert(file->begin(), buffer.begin(), buffer.end());
}

void Plugin_Init()
{
	PD2HOOK_LOG_LOG("ShaderPatcher Init");

	// PD2_HOOK_ASSET_FILE("core/shaders/post", "shaders", &PatchShaderFile);

	// const char *mod_path = pd2_get_mod_directory();
	// std::string msg = "Mod path: " + std::string(mod_path);

	// PD2HOOK_LOG_LOG(msg.c_str());
}

void Plugin_Update()
{
    
}

void Plugin_Setup_Lua(lua_State* L)
{
	// Deprecated, see this function's documentation (in superblt_flat.h) for more detail
}

int Plugin_PushLua(lua_State* L)
{
	lua_newtable(L);

	lua_pushboolean(L, true);
	lua_setfield(L, -2, "loaded");

	return 1;
}

/* only used for debug, remember to remove when i think stuff actually works */
// int main() {
//     std::vector<unsigned char> buffer = file_read("X:/shader_tool_cpp/post.d3d9.shaders");

//     std::vector<unsigned char> vertexShader = file_read("X:/shader_tool_cpp/test_shader.vert.o");
//     std::vector<unsigned char> fragmentShader = file_read("X:/shader_tool_cpp/test_shader.frag.o");

//     if(vertexShader.size() > 0 && fragmentShader.size() > 0) {
//         ShaderPackage *package_ptr = ReadShaderPackage(buffer);

//         ShaderPatch *patch_ptr = new ShaderPatch();
//         patch_ptr->apply_to = "post";
//         patch_ptr->mode = SHADER_PATCH_MODE_REPLACE;
//         patch_ptr->name = "post_DOF:default";

//         ShaderPassPatch *pass_ptr = new ShaderPassPatch();
//         pass_ptr->vertex_shader = vertexShader;
//         pass_ptr->pixel_shader = fragmentShader;

//         for(int i =0; i < 4; i++) {
//             ShaderPassSamplerPatch *sampler_ptr = new ShaderPassSamplerPatch();
//             sampler_ptr->id = i;
//             sampler_ptr->vars[8] = 827606343;
//             sampler_ptr->vars[11] = 0;
//             sampler_ptr->vars[1] = 3;
//             sampler_ptr->vars[2] = 3;
//             sampler_ptr->vars[5] = 2;
//             sampler_ptr->vars[6] = 2;
//             sampler_ptr->vars[7] = 2;
//             pass_ptr->samplers.push_back(sampler_ptr);
//         }

//         pass_ptr->render_states[0x07] = 0;
//         pass_ptr->render_states[0x17] = 4;
//         pass_ptr->render_states[0x0E] = 0;
//         pass_ptr->render_states[27] = 0;
//         pass_ptr->render_states[0x16] = 1;
//         pass_ptr->render_states[168] = 15;
//         pass_ptr->render_states[190] = 15;
//         pass_ptr->render_states[191] = 15;
//         pass_ptr->render_states[52] = 0;
//         pass_ptr->render_states[56] = 1;
//         pass_ptr->render_states[53] = 1;
//         pass_ptr->render_states[58] = 0;
//         pass_ptr->render_states[55] = 1;
//         pass_ptr->render_states[57] = 0;
//         pass_ptr->render_states[59] = 0;
//         pass_ptr->render_states[54] = 1;
//         pass_ptr->render_states[195] = 0;
//         pass_ptr->render_states[175] = 0;
//         pass_ptr->render_states[28] = 0;
//         pass_ptr->render_states[0x1D] = 0;
//         pass_ptr->render_states[180] = 0;
//         pass_ptr->render_states[182] = 0;
//         pass_ptr->render_states[183] = 0;
//         pass_ptr->render_states[194] = 0;
//         pass_ptr->render_states[206] = 0;
//         pass_ptr->render_states[0x0F] = 0;

//         patch_ptr->passes["ambient"].push_back(pass_ptr);

//         if(ApplyShaderPatch(package_ptr, patch_ptr)) {
//             std::vector<unsigned char> data = std::vector<unsigned char>((1024 * 1024 * 15));

//             WriteShaderPackage(data, package_ptr);

//             file_write("X:/shader_tool_cpp/post.d3d9.modified.shaders", data);
//         }
//     }

//     return 0;
// }
