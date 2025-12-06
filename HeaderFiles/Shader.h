#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "HeaderFiles/Core.h"
struct ConstantBufferVariable
{
    unsigned int offset;
    unsigned int size;
};
class ConstantBuffer {
public:
    ID3D12Resource* constantBuffer;
    unsigned char* buffer;
    unsigned int cbSizeInBytes;

    std::string name;
    std::map<std::string, ConstantBufferVariable> constantBufferData;

    unsigned int maxDrawCalls;
    unsigned int offsetIndex;

    void init(Core* core, unsigned int sizeInBytes, unsigned int _maxDrawCalls = 1024);
    void update(std::string name, void* data);
    D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const;
    void next();
};
enum shaderTypes { VS, PS };
class Shader {
public:
    std::string name;
    ID3DBlob* shader;

    std::map<std::string, ConstantBuffer> constantBuffers;//map is better

    std::string readShaderFile(std::string fileName);
    void compileShader(std::string fileName, shaderTypes shaderType, std::string shaderName);
    void getConstantBuffer(Core* core);
};
class ShaderManager {
public:
    std::map<std::string, Shader> shaders;
    void init(Core* core);
    void loadShader(Core* core, std::string shaderName, shaderTypes shaderType);
    void updateConstant(std::string shaderName, std::string cbName, std::string varName, void* data);
};