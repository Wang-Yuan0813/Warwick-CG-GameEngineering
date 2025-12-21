#include "HeaderFiles/Shader.h"

void ConstantBuffer::init(Core* core, unsigned int sizeInBytes, unsigned int _maxDrawCalls) {
    cbSizeInBytes = (sizeInBytes + 255) & ~255;
    maxDrawCalls = _maxDrawCalls;
    unsigned int cbSizeInBytesAligned = cbSizeInBytes * maxDrawCalls;
    offsetIndex = 0;
    HRESULT hr;
    D3D12_HEAP_PROPERTIES heapprops = {};
    heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapprops.CreationNodeMask = 1;
    heapprops.VisibleNodeMask = 1;
    D3D12_RESOURCE_DESC cbDesc = {};
    cbDesc.Width = cbSizeInBytesAligned;
    cbDesc.Height = 1;
    cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbDesc.DepthOrArraySize = 1;
    cbDesc.MipLevels = 1;
    cbDesc.SampleDesc.Count = 1;
    cbDesc.SampleDesc.Quality = 0;
    cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
        IID_PPV_ARGS(&constantBuffer));
    constantBuffer->Map(0, NULL, (void**)&buffer);
}
void ConstantBuffer::update(std::string name, void* data) {
    ConstantBufferVariable cbVariable = constantBufferData[name];
    unsigned int offset = offsetIndex * cbSizeInBytes;
    memcpy(&buffer[offset + cbVariable.offset], data, cbVariable.size);
}
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::getGPUAddress() const {
    return (constantBuffer->GetGPUVirtualAddress() + (offsetIndex * cbSizeInBytes));
}
void ConstantBuffer::next() {
    offsetIndex++;
    if (offsetIndex >= maxDrawCalls) {
        offsetIndex = 0;
    }
}
std::string Shader::readShaderFile(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void Shader::compileShader(std::string fileName, shaderTypes shaderType, std::string shaderName) {
    ID3DBlob* status;
    std::string shaderStr = readShaderFile(fileName);
    name = shaderName;
    HRESULT hr;
    switch (shaderType) {
    case VS:
        hr = D3DCompile(shaderStr.c_str(), strlen(shaderStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &shader, &status);
        if (FAILED(hr)) {
            (char*)status->GetBufferPointer();
        }
        break;
    case PS:
        hr = D3DCompile(shaderStr.c_str(), strlen(shaderStr.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
        if (FAILED(hr)) {
            (char*)status->GetBufferPointer();
        }
        break;
    }
}

void Shader::getConstantBuffer(Core* core) {
    ID3D12ShaderReflection* reflection;
    D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
    D3D12_SHADER_DESC desc;
    reflection->GetDesc(&desc);
    //read constant buffers in shader (may have more than 1 constant buffers)
    for (int i = 0; i < desc.ConstantBuffers; i++) {
        ConstantBuffer buffer;
        ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
        D3D12_SHADER_BUFFER_DESC cbDesc;
        constantBuffer->GetDesc(&cbDesc);
        unsigned int totalSize = 0;
        for (int j = 0; j < cbDesc.Variables; j++) {
            ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
            D3D12_SHADER_VARIABLE_DESC vDesc;
            var->GetDesc(&vDesc);
            ConstantBufferVariable bufferVariable;
            bufferVariable.offset = vDesc.StartOffset;
            bufferVariable.size = vDesc.Size;
            buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
            totalSize += bufferVariable.size;
        }
        //add
        buffer.name = cbDesc.Name;
        buffer.cbSizeInBytes = totalSize;
        buffer.init(core, totalSize);
		constantBuffers.push_back(buffer);
        //constantBuffers.insert(std::pair<std::string, ConstantBuffer>(buffer.name, buffer));
    }
    for (int i = 0; i < desc.BoundResources; i++) {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        reflection->GetResourceBindingDesc(i, &bindDesc);
        if (bindDesc.Type == D3D_SIT_TEXTURE) {
            textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
        }
    }
    reflection->Release();
}
void ShaderManager::init(Core* core) {
    loadShader(core, "AnimatedVertexShader", VS);
    loadShader(core, "StaticVertexShader", VS);
    loadShader(core, "StaticColourVertexShader", VS);
    loadShader(core, "InstanceTreeVS", VS);
    loadShader(core, "InstanceGrassVS", VS);

    loadShader(core, "PixelShader", PS);
    loadShader(core, "WaterPS", PS);
    loadShader(core, "GroundPS", PS);
    loadShader(core, "TextureTreePS", PS);
    loadShader(core, "TextureBoatPS", PS);
    loadShader(core, "SkyPS", PS);
    loadShader(core, "AnimationPS", PS);

    //MRT
    loadShader(core, "MRTVS", VS);
	loadShader(core, "MRTPS", PS);
}
void ShaderManager::loadShader(Core* core, std::string shaderName, shaderTypes shaderType) {
    Shader shader;
    shader.name = shaderName;
    std::string foldPath = "Shaders/";
    std::string fileName = foldPath + shaderName + ".cso";
    std::wstring wideName = std::wstring(fileName.begin(), fileName.end());
#if DEBUG_MODE
    shader.compileShader(std::string(foldPath + shaderName + ".hlsl"), shaderType, shaderName);
    D3DWriteBlobToFile(shader.shader, wideName.c_str(), false);
#else
    //find and complile
    std::ifstream compiledFile(fileName);
    if (compiledFile.is_open()) {//check if there is compiled shader code
        D3DReadFileToBlob(wideName.c_str(), &shader.shader);
    }
    else {//if not, generate a .cso file
        shader.compileShader(std::string(foldPath + shaderName + ".hlsl"), shaderType, shaderName);
        D3DWriteBlobToFile(shader.shader, wideName.c_str(), false);
    }
#endif
    shader.getConstantBuffer(core);
    shaders.insert(std::pair<std::string, Shader>(shaderName, shader));
}
void ShaderManager::updateConstant(std::string shaderName, std::string cbName, std::string varName, void* data) {
    for(auto & cb : shaders[shaderName].constantBuffers){
        if(cb.name == cbName){
            cb.update(varName, data);
            return;
        }
	}
    //shaders[shaderName].constantBuffers[cbName].update(varName, data);
}