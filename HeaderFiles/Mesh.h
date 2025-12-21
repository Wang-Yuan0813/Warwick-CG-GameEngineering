#pragma once

#include "PSO.h"
#include "BasicContainer.h"

struct PRIM_VERTEX
{
    Vec3 position;
    Colour colour;
};
struct STATIC_VERTEX
{
    Vec3 pos;
    Vec3 normal;
    Vec3 tangent;
    float tu;
    float tv;
    Colour color;
};
struct ANIMATED_VERTEX
{
    Vec3 pos;
    Vec3 normal;
    Vec3 tangent;
    float tu;
    float tv;
    unsigned int bonesIDs[4];
    float boneWeights[4];
    Colour color;
};
STATIC_VERTEX addVertex(Vec3 p, Vec3 n, Vec3 t, float tu, float tv) {
    STATIC_VERTEX v;
    v.pos = p;
    v.normal = n;
    v.tangent = t; // For now
    v.tu = tu;
    v.tv = tv;
    return v;
}
class Mesh {
public:
    ID3D12Resource* vertexBuffer;
    ID3D12Resource* indexBuffer;

    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_INDEX_BUFFER_VIEW ibView;
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
    unsigned int numMeshIndices;

    //static init
    void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices) {
        init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getStaticLayout();
    }
    //animate init
    void init(Core* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
    {
        init(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();
    }

    void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices,
        unsigned int* indices, int numIndices) {
        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vbDesc = {};
        vbDesc.Width = numVertices * vertexSizeInBytes;
        vbDesc.Height = 1;
        vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vbDesc.DepthOrArraySize = 1;
        vbDesc.MipLevels = 1;
        vbDesc.SampleDesc.Count = 1;
        vbDesc.SampleDesc.Quality = 0;
        vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
        core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes, 
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vbView.StrideInBytes = vertexSizeInBytes;
        vbView.SizeInBytes = numVertices * vertexSizeInBytes;

        D3D12_RESOURCE_DESC ibDesc;
        memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
        ibDesc.Width = numIndices * sizeof(unsigned int);
        ibDesc.Height = 1;
        ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibDesc.DepthOrArraySize = 1;
        ibDesc.MipLevels = 1;
        ibDesc.SampleDesc.Count = 1;
        ibDesc.SampleDesc.Quality = 0;
        ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        HRESULT hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
        core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
            D3D12_RESOURCE_STATE_INDEX_BUFFER);

        ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = numIndices * sizeof(unsigned int);
        numMeshIndices = numIndices;

    }
    void draw(Core* core) {
        core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
        core->getCommandList()->IASetIndexBuffer(&ibView);
        core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
    }

};
class DynamicMesh {
public:
    ID3D12Resource* vertexBuffer;
    ID3D12Resource* indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_INDEX_BUFFER_VIEW ibView;
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
    unsigned int numMeshIndices;
    uint8_t* vMapped;
    //void* iMapped;
    //static init
    void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices) {
        init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getStaticLayout();
    }
    //animate init
    void init(Core* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices)
    {
        init(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getAnimatedLayout();
    }

    void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices,
        unsigned int* indices, int numIndices) {
        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC vbDesc = {};
        vbDesc.Width = numVertices * vertexSizeInBytes;
        vbDesc.Height = 1;
        vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vbDesc.DepthOrArraySize = 1;
        vbDesc.MipLevels = 1;
        vbDesc.SampleDesc.Count = 1;
        vbDesc.SampleDesc.Quality = 0;
        vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&vertexBuffer));

        vertexBuffer->Map(0, NULL, reinterpret_cast<void**>(&vMapped));//Get pointer to allocated memory on upload heap(Map)
        memcpy(vMapped, vertices, numVertices * vertexSizeInBytes);

        vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vbView.StrideInBytes = vertexSizeInBytes;
        vbView.SizeInBytes = numVertices * vertexSizeInBytes;

        heapprops = {};
        //heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC ibDesc;
        memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
        ibDesc.Width = numIndices * sizeof(unsigned int);
        ibDesc.Height = 1;
        ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibDesc.DepthOrArraySize = 1;
        ibDesc.MipLevels = 1;
        ibDesc.SampleDesc.Count = 1;
        ibDesc.SampleDesc.Quality = 0;
        ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        HRESULT hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
        core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
            D3D12_RESOURCE_STATE_INDEX_BUFFER);

        ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = numIndices * sizeof(unsigned int);
        numMeshIndices = numIndices;

    }
    void copyData(int i, int vertexSizeInBytes, void* data) {
        memcpy(vMapped + i * vertexSizeInBytes, data, vertexSizeInBytes);
    }
    void draw(Core* core) {
        core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
        core->getCommandList()->IASetIndexBuffer(&ibView);
        core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
    }
    ~DynamicMesh() {
        vertexBuffer->Unmap(0, NULL);
        vertexBuffer->Release();
        indexBuffer->Release();
    }
};
struct INSTANCE{
    Matrix w;
};
class InstanceMesh {
public:
    
    ID3D12Resource* vertexBuffer;
    ID3D12Resource* indexBuffer;
    ID3D12Resource* instanceBuffer;

    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_VERTEX_BUFFER_VIEW instanceView;
    D3D12_INDEX_BUFFER_VIEW ibView;
    //D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
    unsigned int numMeshIndices;
    unsigned int numInstances;
    unsigned int instanceSizeInBytes;
    bool needAlphatest = false;
    //static init
    void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, std::vector<INSTANCE> instances) {
        numInstances = instances.size();
        instanceSizeInBytes = sizeof(INSTANCE);
        init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), &instances[0]);
    }
    //animate init
    void init(Core* core, std::vector<ANIMATED_VERTEX> vertices, std::vector<unsigned int> indices, std::vector<INSTANCE> instances){
        numInstances = instances.size();
        instanceSizeInBytes = sizeof(INSTANCE);
        init(core, &vertices[0], sizeof(ANIMATED_VERTEX), vertices.size(), &indices[0], indices.size(), &instances[0]);
    }

    void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices,
        unsigned int* indices, int numIndices, void* instances) {

        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vbDesc = {};
        vbDesc.Width = numVertices * vertexSizeInBytes;
        vbDesc.Height = 1;
        vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vbDesc.DepthOrArraySize = 1;
        vbDesc.MipLevels = 1;
        vbDesc.SampleDesc.Count = 1;
        vbDesc.SampleDesc.Quality = 0;
        vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
        core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vbView.StrideInBytes = vertexSizeInBytes;
        vbView.SizeInBytes = numVertices * vertexSizeInBytes;

        //instance data
        /*heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;*/
        heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC inDesc = {};
        inDesc.Width = instanceSizeInBytes * numInstances;
        inDesc.Height = 1;
        inDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        inDesc.DepthOrArraySize = 1;
        inDesc.MipLevels = 1;
        inDesc.SampleDesc.Count = 1;
        inDesc.SampleDesc.Quality = 0;
        inDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &inDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&instanceBuffer));

        core->uploadResource(instanceBuffer, instances, instanceSizeInBytes * numInstances,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        instanceView.BufferLocation = instanceBuffer->GetGPUVirtualAddress();
        instanceView.StrideInBytes = instanceSizeInBytes;
        instanceView.SizeInBytes = numInstances * instanceSizeInBytes;
        
        D3D12_RESOURCE_DESC ibDesc;
        memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
        ibDesc.Width = numIndices * sizeof(unsigned int);
        ibDesc.Height = 1;
        ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibDesc.DepthOrArraySize = 1;
        ibDesc.MipLevels = 1;
        ibDesc.SampleDesc.Count = 1;
        ibDesc.SampleDesc.Quality = 0;
        ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        HRESULT hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
        core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
            D3D12_RESOURCE_STATE_INDEX_BUFFER);

        ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = numIndices * sizeof(unsigned int);
        numMeshIndices = numIndices;

    }
    void draw(Core* core) {
        D3D12_VERTEX_BUFFER_VIEW bufferViews[2];
        bufferViews[0] = vbView;
        bufferViews[1] = instanceView;
        core->getCommandList()->IASetVertexBuffers(0, 2, bufferViews);
        core->getCommandList()->IASetIndexBuffer(&ibView);
        core->getCommandList()->DrawIndexedInstanced(numMeshIndices, numInstances, 0, 0, 0);

    }

};