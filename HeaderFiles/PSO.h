#pragma once
#include "Core.h"
#include "GEMLoader.h"
#include "Shader.h"

class PSOManager {
public:
    std::unordered_map<std::string, ID3D12PipelineState*> psos;

    void createPSO(Core* core, std::string name, ID3DBlob* vs, ID3DBlob* ps, D3D12_INPUT_LAYOUT_DESC layout, 
        bool wireFrameMode = false) {
        //Avoid creating extra state
        if (psos.find(name) != psos.end()) return;
        //Configure GPU pipeline with shaders, layout and Root Signature
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout = layout;
        desc.pRootSignature = core->rootSignature;
        desc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };
        desc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };
        //Responsible for configuring the rasterizer
        D3D12_RASTERIZER_DESC rasterDesc = {};
        if(wireFrameMode)
            rasterDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
        else
            rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
        rasterDesc.FrontCounterClockwise = FALSE;
        rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterDesc.DepthClipEnable = TRUE;
        rasterDesc.MultisampleEnable = FALSE;
        rasterDesc.AntialiasedLineEnable = FALSE;
        rasterDesc.ForcedSampleCount = 0;
        rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        desc.RasterizerState = rasterDesc;
        //Responsible for configuring the depth buffer
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        depthStencilDesc.StencilEnable = FALSE;
        desc.DepthStencilState = depthStencilDesc;
        //Blend State
        D3D12_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = FALSE;
        blendDesc.IndependentBlendEnable = FALSE;

        /*D3D12_RENDER_TARGET_BLEND_DESC additiveBlend = {};
        additiveBlend.BlendEnable = TRUE;
        additiveBlend.LogicOpEnable = FALSE;

        additiveBlend.SrcBlend = D3D12_BLEND_ONE;
        additiveBlend.DestBlend = D3D12_BLEND_ONE;
        additiveBlend.BlendOp = D3D12_BLEND_OP_ADD;

        additiveBlend.SrcBlendAlpha = D3D12_BLEND_ZERO;
        additiveBlend.DestBlendAlpha = D3D12_BLEND_ONE;
        additiveBlend.BlendOpAlpha = D3D12_BLEND_OP_ADD;

        additiveBlend.LogicOp = D3D12_LOGIC_OP_NOOP;
        additiveBlend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED |
            D3D12_COLOR_WRITE_ENABLE_GREEN |
            D3D12_COLOR_WRITE_ENABLE_BLUE;
        for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
            blendDesc.RenderTarget[i] = additiveBlend;
        }*/
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlend = {
        FALSE, FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
        };
        for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
            blendDesc.RenderTarget[i] = defaultRenderTargetBlend;
        }
        desc.BlendState = blendDesc;
        //Render Target State + Topology
        desc.SampleMask = UINT_MAX;
        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
#if 1
        //MRT test
		//create 3 render targets
        desc.NumRenderTargets = core->mrtCount;
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//color + roughness
		desc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;//normal + freshness
        desc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;//depth
        //desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//color
        //desc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//normal
#else
        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
#endif
        
        desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count = 1;
        //Create Pipeline State Object
        ID3D12PipelineState* pso;
        HRESULT hr = core->device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pso));
        //Insert into map
        psos.insert({ name, pso });
    }
    void bind(Core* core, std::string name) {
        core->getCommandList()->SetPipelineState(psos[name]);
    }

};
class VertexLayoutCache {
public:
    static const D3D12_INPUT_LAYOUT_DESC& getStaticLayout() {
        static const D3D12_INPUT_ELEMENT_DESC inputLayoutStatic[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        };
        static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutStatic, 5 };
        return desc;
    }
    static const D3D12_INPUT_LAYOUT_DESC& getAnimatedLayout() {
        static const D3D12_INPUT_ELEMENT_DESC inputLayoutAnimated[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        };
        static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutAnimated, 7 };
        return desc;
    }
    static const D3D12_INPUT_LAYOUT_DESC& getInstanceLayout() {
        static const D3D12_INPUT_ELEMENT_DESC inputLayoutStaticInstanced[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        { "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
        { "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
        { "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
        { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
        };
        static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutStaticInstanced, 9 };
        return desc;
    }
};