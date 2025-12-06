#pragma once

#include "HeaderFiles/PSO.h"
#include "HeaderFiles/Core.h"
#include "HeaderFiles/GEMLoader.h"
#include "HeaderFiles/Shader.h"
#include "HeaderFiles/BasicContainer.h"

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
};

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