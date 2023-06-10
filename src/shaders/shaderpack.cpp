#include "shaderpack.h"
#include <iostream>
#include "buffer.h"

bool check_if_finished(std::vector<unsigned char> &buffer, int32_t& offset) {
    return offset >= buffer.size();
}

ShaderPackage* ReadShaderPackage(std::vector<unsigned char> &buffer) {
    ShaderPackage* package_ptr = new ShaderPackage;

    int32_t offset = 0;
    int32_t count = 0;
    int32_t padding = 0;

    count = read_int32_le(buffer, offset);

    if(count == -1) {
        padding = read_int32_le(buffer, offset);
        count = read_int32_le(buffer, offset);
    }

    package_ptr->count = count;
    package_ptr->padding = padding;

    for (int i = 0; i < count; i++) {
        int32_t type = read_int32_le(buffer, offset);

        if(type != OBJECT_TYPE_SHADER && type != OBJECT_TYPE_SHADER_PASS && type != OBJECT_TYPE_SHADER_LIBRARY) {
            std::cout << "Unknown object type: " << type << std::endl;
            return nullptr;
        }

        int32_t ref_id = read_int32_le(buffer, offset);
        int32_t len = read_int32_le(buffer, offset);

        ObjectHeader* header_ptr = new ObjectHeader;

        header_ptr->type = type;
        header_ptr->ref_id = ref_id;
        header_ptr->len = len;
        header_ptr->pos = offset;

        package_ptr->headers.push_back(header_ptr);

        offset = header_ptr->pos + len;
    }
    
    for(const auto& header: package_ptr->headers) {
        int32_t type = header->type;

        std::vector<unsigned char> innerBuffer(header->len);

        for(int i = 0; i < header->len; i++) {
            innerBuffer[i] = buffer[header->pos + i];
        }

        if(type == OBJECT_TYPE_SHADER) {
            ObjectShader* object_ptr = ReadObjectShader(innerBuffer, header);
            package_ptr->objects.push_back(object_ptr);
        } else if(type == OBJECT_TYPE_SHADER_PASS) {
            ObjectShaderPass* object_ptr = ReadObjectShaderPass(innerBuffer, header);
            package_ptr->objects.push_back(object_ptr);
        } else if(type == OBJECT_TYPE_SHADER_LIBRARY) {
            ObjectShaderLibrary* object_ptr = ReadObjectShaderLibrary(innerBuffer, header);
            package_ptr->objects.push_back(object_ptr);
        }
    }

    return package_ptr;
}

ObjectShader* ReadObjectShader(std::vector<unsigned char> &buffer, ObjectHeader* header_ptr) {
    ObjectShader* object_ptr = new ObjectShader;
    object_ptr->header = header_ptr;

    int32_t offset = 0;

    int32_t count = read_int32_le(buffer, offset);

    for (int i = 0; i < count; i++) {
        int64_t id = read_int64_le(buffer, offset);

        int32_t len = read_int32_le(buffer, offset);
        std::vector<int32_t> passes = std::vector<int32_t>(len);

        for(int j = 0; j < len; j++) {
            int32_t ref_id = read_int32_le(buffer, offset);
            passes[j] = ref_id;
        }

        object_ptr->passes[id] = passes;
    }

    if(!check_if_finished(buffer, offset)) {
        std::cout << "Shader object not finished." << std::endl;
    }

    return object_ptr;
}

ObjectShaderPass* ReadObjectShaderPass(std::vector<unsigned char> &buffer, ObjectHeader* header_ptr) {
    ObjectShaderPass* object_ptr = new ObjectShaderPass;
    object_ptr->header = header_ptr;

    int32_t offset = 0;

    int32_t count = read_int32_le(buffer, offset);

    for(int i = 0; i < count; i++) {
        StateVar* state_var_ptr = ReadStateVar(buffer, offset);
        object_ptr->render_states.push_back(state_var_ptr);
    }

    int32_t sampler_count = read_int32_le(buffer, offset);

    for(int i = 0; i < sampler_count; i++) {
        SamplerState* sampler_state_ptr = ReadSamplerState(buffer, offset);
        object_ptr->sampler_states.push_back(sampler_state_ptr);
    }

    int32_t vertex_len = read_int32_le(buffer, offset);
    std::vector<unsigned char> vertex_code = read_buffer(buffer, offset, vertex_len);
    int32_t pixel_len = read_int32_le(buffer, offset);
    std::vector<unsigned char> pixel_code = read_buffer(buffer, offset, pixel_len);

    object_ptr->vertex_shader = vertex_code;
    object_ptr->pixel_shader = pixel_code;

    if(!check_if_finished(buffer, offset)) {
        std::cout << "Shader pass object not finished." << std::endl;
    }

    return object_ptr;
}

ObjectShaderLibrary* ReadObjectShaderLibrary(std::vector<unsigned char> &buffer, ObjectHeader* header_ptr) {
    ObjectShaderLibrary* object_ptr = new ObjectShaderLibrary;
    object_ptr->header = header_ptr;

    int32_t offset = 0;

    int32_t count = read_int32_le(buffer, offset);

    for(int i = 0; i < count; i++) {
        int64_t id = read_int64_le(buffer, offset);
        int32_t ref_id = read_int32_le(buffer, offset);

        object_ptr->render_templates[id] = ref_id;
    }

    if(!check_if_finished(buffer, offset)) {
        std::cout << "Shader library object not finished." << std::endl;
    }

    return object_ptr;
}

ObjectShaderLibrary* GetShaderLibrary(ShaderPackage* package_ptr) {
    for(const auto& object: package_ptr->objects) {
        if(object->header->type == OBJECT_TYPE_SHADER_LIBRARY) {
            return (ObjectShaderLibrary*)object;
        }
    }

    return nullptr;
}

StateVar* ReadStateVar(std::vector<unsigned char> &buffer, int32_t &offset) {
    StateVar* state_var_ptr = new StateVar;
    
    int32_t id = read_int32_le(buffer, offset);
    state_var_ptr->id = id;

    int32_t b = read_int8_le(buffer, offset);
    state_var_ptr->b = b;

    if(b == 0) {
        int32_t val4 = read_int32_le(buffer, offset);
        state_var_ptr->value4 = val4;
    } else {
        int64_t val8 = read_int64_le(buffer, offset);
        state_var_ptr->value8 = val8;
    }

    return state_var_ptr;
}

SamplerState* ReadSamplerState(std::vector<unsigned char> &buffer, int32_t &offset){
    SamplerState* sampler_state_ptr = new SamplerState;

    int32_t id = read_int32_le(buffer, offset);
    int32_t len = read_int32_le(buffer, offset);

    std::vector<StateVar*> state_vars;

    for(int i = 0; i < len; i++) {
        StateVar* state_var_ptr = ReadStateVar(buffer, offset);
        state_vars.push_back(state_var_ptr);
    }

    state_vars.shrink_to_fit();

    sampler_state_ptr->id = id;
    sampler_state_ptr->vars = state_vars;

    return sampler_state_ptr;
}

void WriteShaderPackage(std::vector<unsigned char> &buffer, ShaderPackage* package_ptr) {
    int32_t offset = 0;
    write_int32_le(buffer, offset, package_ptr->objects.size());

    std::vector<unsigned char> innerBuffer = std::vector<unsigned char>(1024 * 1024 * 15);

    for(int i = 0; i < package_ptr->objects.size(); i++) {
        Object* object = package_ptr->objects[i];

        innerBuffer.empty();
        innerBuffer.resize(1024 * 1024 * 15);

        if(object->header->type == OBJECT_TYPE_SHADER_LIBRARY) {
            WriteObjectShaderLibrary(innerBuffer, (ObjectShaderLibrary*)object);
        } else if(object->header->type == OBJECT_TYPE_SHADER) {
            WriteObjectShader(innerBuffer, (ObjectShader*)object);
        } else if(object->header->type == OBJECT_TYPE_SHADER_PASS) {
            WriteObjectShaderPass(innerBuffer, (ObjectShaderPass*)object);
        }

        write_int32_le(buffer, offset, object->header->type);
        write_int32_le(buffer, offset, object->header->ref_id);

        write_int32_le(buffer, offset, innerBuffer.size());

        write_buffer(buffer, offset, innerBuffer);
    }

    shrink(buffer, offset);
}

void WriteObjectShader(std::vector<unsigned char> &buffer, ObjectShader* object_ptr) {
    int32_t offset = 0;

    write_int32_le(buffer, offset, object_ptr->passes.size());

    for (auto it = object_ptr->passes.begin(); it != object_ptr->passes.end(); ++it) {
        int64_t pass_name = it->first;
        std::vector<int32_t> ref_ids = it->second;

        write_int64_le(buffer, offset, pass_name);
        write_int32_le(buffer, offset, ref_ids.size());

        for(const auto& ref_id: ref_ids) {
            write_int32_le(buffer, offset, ref_id);
        }
    }

    shrink(buffer, offset);
}

void WriteObjectShaderPass(std::vector<unsigned char> &buffer, ObjectShaderPass* object_ptr) {
    int32_t offset = 0;

    write_int32_le(buffer, offset, object_ptr->render_states.size());

    for(int i = 0; i < object_ptr->render_states.size(); i++) {
        WriteStateVar(buffer, offset, object_ptr->render_states[i]);
    }

    write_int32_le(buffer, offset, object_ptr->sampler_states.size());

    for(int i = 0; i < object_ptr->sampler_states.size(); i++) {
        WriteSamplerState(buffer, offset, object_ptr->sampler_states[i]);
    }

    write_int32_le(buffer, offset, object_ptr->vertex_shader.size());
    write_buffer(buffer, offset, object_ptr->vertex_shader);

    write_int32_le(buffer, offset, object_ptr->pixel_shader.size());
    write_buffer(buffer, offset, object_ptr->pixel_shader);

    shrink(buffer, offset);
}

void WriteObjectShaderLibrary(std::vector<unsigned char> &buffer, ObjectShaderLibrary* object_ptr) {
    int32_t offset = 0;
    write_int32_le(buffer, offset, object_ptr->render_templates.size());


    for (auto it = object_ptr->render_templates.begin(); it != object_ptr->render_templates.end(); ++it) {
        int64_t id = it->first;
        int32_t ref_id = it->second;

        write_int64_le(buffer, offset, id);
        write_int32_le(buffer, offset, ref_id);
    }

    shrink(buffer, offset);
}

void WriteStateVar(std::vector<unsigned char> &buffer, int32_t &offset, StateVar* state_var_ptr) {
    write_int32_le(buffer, offset, state_var_ptr->id);
    write_int8_le(buffer, offset, state_var_ptr->b);

    if(state_var_ptr->b == 0) {
        write_int32_le(buffer, offset, state_var_ptr->value4);
    } else {
        write_int64_le(buffer, offset, state_var_ptr->value8);
    }
}

void WriteSamplerState(std::vector<unsigned char> &buffer, int32_t &offset, SamplerState* sampler_state_ptr) {
    write_int32_le(buffer, offset, sampler_state_ptr->id);
    write_int32_le(buffer, offset, sampler_state_ptr->vars.size());

    for(const auto& state_var_ptr: sampler_state_ptr->vars) {
        WriteStateVar(buffer, offset, state_var_ptr);
    }
}