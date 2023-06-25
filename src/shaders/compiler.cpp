#include "compiler.h"
#include "file.h"

void store_cache(std::string sourcePath,  std::vector<unsigned char> inData, std::vector<unsigned char> outData)
{
    std::string cachePath = sourcePath + ".cache";

    file_write(cachePath, outData);
};

bool should_load_cache(std::string sourcePath, std::vector<unsigned char> inData)
{
    return false;
};

std::vector<unsigned char> load_cache(std::string sourcePath, std::vector<unsigned char> inData)
{
    std::string cachePath = sourcePath + ".cache";

    if(file_exists(cachePath))
    {
        return file_read(cachePath);
    }

    return std::vector<unsigned char>();
};

// TODO: Figure out how to do caching

std::vector<unsigned char> compile_shader(std::string sourcePath, std::string entryPoint, bool isVertex, bool useCache)
{
    ID3DBlob* shaderBlob = nullptr;

    std::vector<unsigned char> fileData = file_read(sourcePath);

    HRESULT res = D3DCompile(fileData.data(), fileData.size(), sourcePath.c_str(), NULL, NULL, entryPoint.c_str(), isVertex ? "vs_3_0" : "ps_3_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &shaderBlob, nullptr);

    if(res == S_OK)
    {
        std::vector<unsigned char> shaderData;
        shaderData.resize(shaderBlob->GetBufferSize());
        memcpy(shaderData.data(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
        shaderBlob->Release();

        // if(useCache)
        //     store_cache(sourcePath, fileData, shaderData);

        std::string msg = "Compiled shader: " + sourcePath;
        PD2HOOK_LOG_LOG(msg.c_str());

        return shaderData;
    } else {
        std::string error = "Failed to compile shader: " + sourcePath;
        PD2HOOK_LOG_ERROR(error.c_str());
    }

    return std::vector<unsigned char>();
}