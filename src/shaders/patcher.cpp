#pragma once

#include "patcher.h"
#include "file.h"
#include "lookup8.h"
#include <algorithm>

Object* GetItemByRef(ShaderPackage* package_ptr, int32_t ref_id) {
    for (int32_t i = 0; i < package_ptr->count; i++) {
        if (package_ptr->headers[i]->ref_id == ref_id) {
            return package_ptr->objects[i];
        }
    }

    return NULL;
}

int32_t GetNextRef(int32_t &ref_counter) {
    return ++ref_counter;
}

void RegisterObject(ShaderPackage* package_ptr, Object* object_ptr) {
    package_ptr->objects.push_back(object_ptr);
    package_ptr->count++;
}

ObjectHeader* CreateEmptyObjectHeader(int32_t type, int32_t ref_id) {
    ObjectHeader* header_ptr = new ObjectHeader;

    header_ptr->type = type;
    header_ptr->ref_id = ref_id;
    header_ptr->len = 0;
    header_ptr->pos = 0;

    return header_ptr;
}

bool ApplyShaderPatch(ShaderPackage* package_ptr, ShaderPatch* patch_ptr) {
    ObjectShaderLibrary* library_ptr = (ObjectShaderLibrary*) GetShaderLibrary(package_ptr);

    library_ptr->header->ref_id = MAX_REF_COUNTER;

    int32_t highest_ref = 0;

    for (int32_t i = 0; i < package_ptr->count; i++) {
        if (package_ptr->headers[i]->ref_id > highest_ref && package_ptr->headers[i]->ref_id < MAX_REF_COUNTER) {
            highest_ref = package_ptr->headers[i]->ref_id;
        }
    }

    int64_t namehash = dieselHashUTF8(patch_ptr->name);

    if(patch_ptr->mode == SHADER_PATCH_MODE_REPLACE) {
        int32_t template_id = library_ptr->render_templates[namehash];

        if(template_id == 0) {
            std::cout << "Template not found." << std::endl;
            return false;
        }

        ObjectShader* template_ptr = (ObjectShader*) GetItemByRef(package_ptr, template_id);

        template_ptr->passes.clear();

        for (auto it = patch_ptr->passes.begin(); it != patch_ptr->passes.end(); ++it) {
            std::string pass_name = it->first;
            std::vector<ShaderPassPatch*> pass_patches = it->second;

            std::vector<int32_t> pass_ids;

            for(const auto& pass_patch: pass_patches) {
                ObjectShaderPass* pass_ptr = new ObjectShaderPass();

                pass_ptr->header = CreateEmptyObjectHeader(OBJECT_TYPE_SHADER_PASS, GetNextRef(highest_ref));

                for (auto it = pass_patch->render_states.begin(); it != pass_patch->render_states.end(); ++it) {
                    int32_t state_id = it->first;
                    int32_t state_value = it->second;

                    StateVar* state_ptr = new StateVar();

                    state_ptr->id = state_id;
                    state_ptr->b = 0;
                    state_ptr->value4 = state_value;
                    state_ptr->value8 = 0;

                    pass_ptr->render_states.push_back(state_ptr);
                }

                for(const auto& sampler : pass_patch->samplers) {
                    int32_t sampler_id = sampler->id;
                    std::vector<StateVar*> sampler_vars;

                    for (auto it = sampler->vars.begin(); it != sampler->vars.end(); ++it) {
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

                pass_ptr->vertex_shader = pass_patch->vertex_shader;
                pass_ptr->pixel_shader = pass_patch->pixel_shader;

                RegisterObject(package_ptr, pass_ptr);

                pass_ids.push_back(pass_ptr->header->ref_id);
            }

            int64_t pass_namehash = dieselHashUTF8(pass_name);
            template_ptr->passes[pass_namehash] = pass_ids;
        }

        library_ptr->header->ref_id = GetNextRef(highest_ref);

        std::sort(package_ptr->objects.begin(), package_ptr->objects.end(), [](Object* a, Object* b) {
            return a->header->ref_id < b->header->ref_id;
        });

        return true;
    } else if(patch_ptr->mode == SHADER_PATCH_MODE_APPEND) {

    }

    return false;
}