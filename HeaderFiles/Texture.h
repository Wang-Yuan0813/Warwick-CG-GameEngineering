#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#include "Core.h"
class Texture {
public:
	ID3D12Resource* tex;
	int heapOffset;

	std::string name;

	void upload(Core* core, void* data, int width, int height, int channels) {
		//Allocate GPU memory
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		D3D12_HEAP_PROPERTIES heapDesc;
		memset(&heapDesc, 0, sizeof(D3D12_HEAP_PROPERTIES));
		heapDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
		D3D12_RESOURCE_DESC textureDesc;
		memset(&textureDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		core->device->CreateCommittedResource(&heapDesc, D3D12_HEAP_FLAG_NONE, &textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&tex));
		//Copy to upload heap, Copy to GPU memory
		D3D12_RESOURCE_DESC desc = tex->GetDesc();
		unsigned long long size;
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
		core->device->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, NULL, NULL, &size);
		unsigned int alignedWidth = ((width * channels) + 255) & ~255;
		core->uploadResource(tex, data, alignedWidth * height,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &footprint);
		//Create shader resource view for texture
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = core->srvHeap.getNextCPUHandle();
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		core->device->CreateShaderResourceView(tex, &srvDesc, srvHandle);
		heapOffset = core->srvHeap.used - 1;
	}
	void load(Core* core, std::string filename) {
		name = filename;
		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* texels = stbi_load(filename.c_str(), &width, &height, &channels, 0);
		if (channels == 3) {
			channels = 4;
			unsigned char* texelsWithAlpha = new unsigned char[width * height * channels];
			for (int i = 0; i < (width * height); i++) {
				texelsWithAlpha[i * 4] = texels[i * 3];
				texelsWithAlpha[(i * 4) + 1] = texels[(i * 3) + 1];
				texelsWithAlpha[(i * 4) + 2] = texels[(i * 3) + 2];
				texelsWithAlpha[(i * 4) + 3] = 255;
			}
			// Initialize texture using width, height, channels, and texelsWithAlpha
			int resizeWidth = (width + 255) & ~255;
			int resizeHeight = (height + 255) & ~255;
			unsigned char* resizeTexels = (unsigned char*)malloc(resizeHeight * resizeWidth * channels);
			stbir_resize_uint8_srgb(texelsWithAlpha, width, height, 0, resizeTexels, resizeWidth, resizeHeight, 0, STBIR_RGBA);
			upload(core, resizeTexels, resizeWidth, resizeHeight, channels);
			//upload(core, texelsWithAlpha, width, height, channels);
			delete[] texelsWithAlpha;
		}
		else {
			// Initialize texture using width, height, channels, and texels
			int resizeWidth = (width + 255) & ~255;
			int resizeHeight = (height + 255) & ~255;
			unsigned char* resizeTexels = (unsigned char*)malloc(resizeHeight * resizeWidth * channels);
			stbir_resize_uint8_srgb(texels, width, height, 0, resizeTexels, resizeWidth, resizeHeight, 0, STBIR_RGBA);

			upload(core, resizeTexels, resizeWidth, resizeHeight, channels);
			//upload(core, texels, width, height, channels);
		}
		stbi_image_free(texels);
	}
};
class TextureManager {
public:
	std::map<std::string, Texture> textures;
	void add(Core* core, std::string filename) {
		auto it = textures.find(filename);
		if (it != textures.end()) return;
		Texture t;
		t.load(core, filename);
		textures.insert({ t.name, t });
	}
	int find(std::string name) {
		return textures[name].heapOffset;
	}
};
#if 1
class RenderTexture {
public:
	static void apply(ShaderManager& sm, Core& core) {
		for (int i = 0; i < sm.shaders["MRTVS"].constantBuffers.size(); i++) {
			int parameterIndex = core.getRootParameterIndex("cbvs") + i;
			core.getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, sm.shaders["MRTVS"].constantBuffers[i].getGPUAddress());
			sm.shaders["MRTVS"].constantBuffers[i].next();
		}
		for (int i = 0; i < sm.shaders["MRTPS"].constantBuffers.size(); i++) {
			int parameterIndex = core.getRootParameterIndex("cbps") + i;
			core.getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, sm.shaders["MRTPS"].constantBuffers[i].getGPUAddress());
			sm.shaders["MRTPS"].constantBuffers[i].next();
		}
	}
	static void draw(Core& core) {
		ID3D12DescriptorHeap* heaps[] = { core.srvHeap.heap };
		core.getCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
		core.getCommandList()->SetGraphicsRootDescriptorTable(core.getRootParameterIndex("tex"), core.srvHeap.heap->GetGPUDescriptorHandleForHeapStart());
		core.getCommandList()->DrawInstanced(3, 1, 0, 0);
	}
	//light
	static float getLightOffset(float time) {
		return sin(time) * 2.0f;
	}
	static void lightsArrayUpdate(float time, Vec4* gLightPosWSArray) {//10 lights , a stupid way to animate lights
		gLightPosWSArray[0] = Vec4(-60.0f + getLightOffset(time + 0.5), 6 + getLightOffset(time), -60.0f + getLightOffset(time * 0.5));
		gLightPosWSArray[1] = Vec4(60 + getLightOffset(time), 6 + getLightOffset(time+0.5), 60 + getLightOffset(time * 0.5));
		gLightPosWSArray[2] = Vec4(60 + getLightOffset(time * 0.5), 6 + getLightOffset(time * 0.5), -60 + getLightOffset(time + 0.5));
		gLightPosWSArray[3] = Vec4(30 + getLightOffset(time + 0.5), 6 + getLightOffset(time + 0.5), -80 + getLightOffset(time));
		gLightPosWSArray[4] = Vec4(90 + getLightOffset(time), 6 + getLightOffset(time * 0.5), 0 + getLightOffset(time));
		gLightPosWSArray[5] = Vec4(-90 + getLightOffset(time + 0.5), 6 + getLightOffset(time), 0 + getLightOffset(time + 0.5));
		gLightPosWSArray[6] = Vec4(0 + getLightOffset(time * 0.5), 6 + getLightOffset(time * 0.5), 90 + getLightOffset(time));
		gLightPosWSArray[7] = Vec4(0 + getLightOffset(time + 0.5), 6 + getLightOffset(time + 0.5), -90 + getLightOffset(time));
		gLightPosWSArray[8] = Vec4(40 + getLightOffset(time), 4 + getLightOffset(time), 70 + getLightOffset(time + 0.5));
		gLightPosWSArray[9] = Vec4(20 + getLightOffset(time + 0.5), 4 + getLightOffset(time), 80 + getLightOffset(time * 0.5));
		/*gLightPosWSArray[0] = Vec3(-60.0f + getLightOffset(time),  getLightOffset(time), -60.0f + getLightOffset(time));
		gLightPosWSArray[1] = Vec3(60.0f + getLightOffset(time), getLightOffset(time), -60.0f + getLightOffset(time));
		gLightPosWSArray[2] = Vec3(-60.0f + getLightOffset(time), getLightOffset(time), 70.0f + getLightOffset(time));
		gLightPosWSArray[3] = Vec3(70.0f + getLightOffset(time),  getLightOffset(time), 50.0f + getLightOffset(time));
		gLightPosWSArray[4] = Vec3(-10.0f + getLightOffset(time), getLightOffset(time), -10.0f + getLightOffset(time));
		gLightPosWSArray[5] = Vec3(10.0f + getLightOffset(time),getLightOffset(time), -10.0f + getLightOffset(time));
		gLightPosWSArray[6] = Vec3(-10.0f + getLightOffset(time), getLightOffset(time), 10.0f + getLightOffset(time));
		gLightPosWSArray[7] = Vec3(10.0f + getLightOffset(time),  getLightOffset(time), 10.0f + getLightOffset(time));
		gLightPosWSArray[8] = Vec3(-20.0f + getLightOffset(time),  getLightOffset(time), 20.0f + getLightOffset(time));
		gLightPosWSArray[9] = Vec3(20.0f + getLightOffset(time), getLightOffset(time), 20.0f + getLightOffset(time));*/
	}
};
#endif