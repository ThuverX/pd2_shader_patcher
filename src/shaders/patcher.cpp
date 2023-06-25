#pragma once

#include "superblt_flat.h"

#include "file.h"
#include "patcher.h"
#include "compiler.h"

#include <algorithm>
#include <map>

typedef unsigned long long idstring;

Object* GetItemByRef(ShaderPackage* package_ptr, int32_t ref_id)
{
	for (int32_t i = 0; i < package_ptr->count; i++)
	{
		if (package_ptr->headers[i]->ref_id == ref_id)
		{
			return package_ptr->objects[i];
		}
	}

	return NULL;
}

int32_t GetNextRef(int32_t& ref_counter)
{
	return ++ref_counter;
}

void RegisterObject(ShaderPackage* package_ptr, Object* object_ptr)
{
	package_ptr->objects.push_back(object_ptr);
	package_ptr->count++;
}

ObjectHeader* CreateEmptyObjectHeader(int32_t type, int32_t ref_id)
{
	ObjectHeader* header_ptr = new ObjectHeader;

	header_ptr->type = type;
	header_ptr->ref_id = ref_id;
	header_ptr->len = 0;
	header_ptr->pos = 0;

	return header_ptr;
}

bool ApplyShaderPatch(ShaderPackage* package_ptr, ShaderPatch* patch_ptr)
{
	ObjectShaderLibrary* library_ptr = (ObjectShaderLibrary*)GetShaderLibrary(package_ptr);

	library_ptr->header->ref_id = MAX_REF_COUNTER;

	int32_t highest_ref = 0;

	for (int32_t i = 0; i < package_ptr->count; i++)
	{
		if (package_ptr->headers[i]->ref_id > highest_ref && package_ptr->headers[i]->ref_id < MAX_REF_COUNTER)
		{
			highest_ref = package_ptr->headers[i]->ref_id;
		}
	}

    ObjectShader* template_ptr = nullptr;

	if (patch_ptr->mode == SHADER_PATCH_MODE_REPLACE)
	{
	    int64_t namehash = create_hash(patch_ptr->name.c_str());
		int32_t template_id = library_ptr->render_templates[namehash];

        if (template_id == 0)
        {
            std::string msg = "Template not found: " + patch_ptr->name;
            PD2HOOK_LOG_ERROR(msg.c_str());
            return false;
        }

        template_ptr = (ObjectShader*)GetItemByRef(package_ptr, template_id);

        template_ptr->passes.clear();
	}
	else if (patch_ptr->mode == SHADER_PATCH_MODE_APPEND)
	{
        template_ptr = new ObjectShader();

        template_ptr->header = CreateEmptyObjectHeader(OBJECT_TYPE_SHADER, GetNextRef(highest_ref));
	}

    if(template_ptr == nullptr) {
        std::string msg = "Failed to create shader template: " + patch_ptr->name;
        PD2HOOK_LOG_ERROR(msg.c_str());
        return false;
    }
    
    for (auto it = patch_ptr->passes.begin(); it != patch_ptr->passes.end(); ++it)
    {
        std::string pass_name = it->first;
        std::vector<ShaderPassPatch*> pass_patches = it->second;

        std::vector<int32_t> pass_ids;

        for (const auto& pass_patch : pass_patches)
        {
            ObjectShaderPass* pass_ptr = new ObjectShaderPass();

            pass_ptr->header = CreateEmptyObjectHeader(OBJECT_TYPE_SHADER_PASS, GetNextRef(highest_ref));

            for (auto it = pass_patch->render_states.begin(); it != pass_patch->render_states.end(); ++it)
            {
                int32_t state_id = it->first;
                int32_t state_value = it->second;

                StateVar* state_ptr = new StateVar();

                state_ptr->id = state_id;
                state_ptr->b = 0;
                state_ptr->value4 = state_value;
                state_ptr->value8 = 0;

                pass_ptr->render_states.push_back(state_ptr);
            }

            for (const auto& sampler : pass_patch->samplers)
            {
                int32_t sampler_id = sampler->id;
                std::vector<StateVar*> sampler_vars;

                for (auto it = sampler->vars.begin(); it != sampler->vars.end(); ++it)
                {
                    int32_t var_id = it->first;
                    int32_t var_value = it->second;

                    StateVar* var_ptr = new StateVar();

                    var_ptr->id = var_id;
                    var_ptr->b = 0;
                    var_ptr->value4 = var_value;
                    var_ptr->value8 = 0;

                    sampler_vars.push_back(var_ptr);
                }

                SamplerState* sampler_ptr = new SamplerState();
                sampler_ptr->id = sampler_id;
                sampler_ptr->vars = sampler_vars;

                pass_ptr->sampler_states.push_back(sampler_ptr);
            }

            if (pass_patch->vertex_shader != nullptr)
            {
                if (pass_patch->vertex_shader->type == SHADER_PATCH_TYPE_PRECOMPILED) {
                    pass_ptr->vertex_shader = file_read(pass_patch->vertex_shader->path);
                } else {
                    pass_ptr->vertex_shader = compile_shader(pass_patch->vertex_shader->path, pass_patch->vertex_shader->entry_point, true, true);
                }
            }

            if (pass_patch->pixel_shader != nullptr)
            {
                if (pass_patch->pixel_shader->type == SHADER_PATCH_TYPE_PRECOMPILED) {
                    pass_ptr->pixel_shader = file_read(pass_patch->pixel_shader->path);
                } else {
                    pass_ptr->pixel_shader = compile_shader(pass_patch->pixel_shader->path, pass_patch->pixel_shader->entry_point, false, true);
                }
            }

            RegisterObject(package_ptr, pass_ptr);

            pass_ids.push_back(pass_ptr->header->ref_id);
        }

        int64_t pass_namehash = create_hash(pass_name.c_str());
        template_ptr->passes[pass_namehash] = pass_ids;
    }

    if(patch_ptr->mode == SHADER_PATCH_MODE_APPEND)
    {
        RegisterObject(package_ptr, template_ptr);

        int64_t namehash = create_hash(patch_ptr->name.c_str());

        library_ptr->render_templates[namehash] = template_ptr->header->ref_id;
    }

    library_ptr->header->ref_id = GetNextRef(highest_ref);

    std::sort(package_ptr->objects.begin(), package_ptr->objects.end(),
                [](Object* a, Object* b) { return a->header->ref_id < b->header->ref_id; });

    return true;
}

std::vector<idstring> patchedFiles = std::vector<idstring>();
std::map<idstring, std::vector<ShaderPatch*>> patches = std::map<idstring, std::vector<ShaderPatch*>>();

void PatchShaderFile(FileData* file)
{
	if (patches.find(file->name) == patches.end())
	{
		return;
	}

	if (std::find(patchedFiles.begin(), patchedFiles.end(), file->name) != patchedFiles.end())
	{
		return;
	}

	std::vector<ShaderPatch*> shaderPatches = patches[file->name];

	std::string msg = "Patching shader file with " + std::to_string(patches.size()) + " patches";

	PD2HOOK_LOG_LOG(msg.c_str());

	FileData ogFile = db_read_file(file->name, file->ext);

	std::vector<unsigned char> buffer = std::vector<unsigned char>(ogFile.data, ogFile.data + ogFile.size);
	ShaderPackage* package_ptr = ReadShaderPackage(buffer);

	std::vector<unsigned char> outBuffer = std::vector<unsigned char>((1024 * 1024 * 15));

	for (ShaderPatch* patch : shaderPatches)
	{
		if (!ApplyShaderPatch(package_ptr, patch))
		{
			PD2HOOK_LOG_ERROR("Failed to apply shader patch");
		}
	}

	WriteShaderPackage(outBuffer, package_ptr);

	db_free_file(ogFile);

	uint8_t* outData = new uint8_t[outBuffer.size()];
	memcpy(outData, outBuffer.data(), outBuffer.size());

	msg = "Patched file with size: " + std::to_string(outBuffer.size());
	PD2HOOK_LOG_LOG(msg.c_str());

	file->data = outData;
	file->size = outBuffer.size();

	patchedFiles.push_back(file->name);

	msg = "Shader file patched with " + std::to_string(patches.size()) + " patches";

	PD2HOOK_LOG_LOG(msg.c_str());
}

void addShaderPatch(ShaderPatch* patch)
{
	std::string name = patch->apply_to;
	idstring hash = create_hash(name.c_str());

	if (patches.find(hash) == patches.end())
	{
		patches[hash] = std::vector<ShaderPatch*>();
	}

	patches[hash].push_back(patch);
}

void ApplyShaderHooks()
{
	idstring extids = create_hash("shaders");
	for (auto it = patches.begin(); it != patches.end(); ++it)
	{
		idstring name = it->first;
		std::vector<ShaderPatch*> patch_list = it->second;

		db_hook_asset_file(name, extids, &PatchShaderFile);
	}
}