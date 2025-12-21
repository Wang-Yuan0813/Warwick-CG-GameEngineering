#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <vector>
#include <string>
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler.lib")

#define DEBUG_MODE 0

class Barrier {
public:
	static void add(ID3D12Resource* res, D3D12_RESOURCE_STATES first, D3D12_RESOURCE_STATES second,
		ID3D12GraphicsCommandList4* commandList) {
		D3D12_RESOURCE_BARRIER rb = {};
		rb.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		rb.Transition.pResource = res;
		rb.Transition.StateBefore = first;
		rb.Transition.StateAfter = second;
		rb.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &rb);
	}
};
class GPUFence {
public:
	ID3D12Fence* fence;
	HANDLE eventHandle;
	UINT64 value = 0;
	void create(ID3D12Device5* device) {
		device->CreateFence(value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	void signal(ID3D12CommandQueue* queue) {
		queue->Signal(fence, ++value);
	}
	void wait() {
		if (fence->GetCompletedValue() < value) {
			fence->SetEventOnCompletion(value, eventHandle);
			WaitForSingleObject(eventHandle, INFINITE);
		}
	}
	~GPUFence() {
		CloseHandle(eventHandle);
		fence->Release();
	}
};
class DescriptorHeap {
public:
	ID3D12DescriptorHeap* heap;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	unsigned int incrementSize;
	int used;

	void init(ID3D12Device5* device, int num) {
		D3D12_DESCRIPTOR_HEAP_DESC uavcbvHeapDesc = {};
		uavcbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavcbvHeapDesc.NumDescriptors = num;
		uavcbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		device->CreateDescriptorHeap(&uavcbvHeapDesc, IID_PPV_ARGS(&heap));
		cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
		incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		used = 0;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE getNextCPUHandle() {
		if (used > 0) {
			cpuHandle.ptr += incrementSize;
		}
		used++;
		return cpuHandle;
	}

};
class Core {
public:
	IDXGIAdapter1* adapter;

	ID3D12Device5* device;
	ID3D12CommandQueue* graphicsQueue;
	ID3D12CommandQueue* copyQueue;
	ID3D12CommandQueue* computeQueue;
	IDXGISwapChain3* swapchain;

	ID3D12CommandAllocator* graphicsCommandAllocator[2];
	ID3D12GraphicsCommandList4* graphicsCommandList[2];

	ID3D12DescriptorHeap* backbufferHeap;
	ID3D12Resource** backbuffers;

	GPUFence graphicsQueueFence[2];

	ID3D12DescriptorHeap* dsvHeap;
	ID3D12Resource* dsv;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;

	ID3D12RootSignature* rootSignature;
	DescriptorHeap srvHeap;

	//MRT
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	ID3D12DescriptorHeap* rtvHeap;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles;
	ID3D12Resource** mrtResources;
	int mrtCount = 3;

	void init(HWND hwnd, int _width, int _height);
	void resetCommandList();
	ID3D12GraphicsCommandList4* getCommandList();
	void runCommandList();
	void flushGraphicsQueue();
	void beginFrame();
	void finishFrame();
	void uploadResource(ID3D12Resource* dstResource, const void* data, unsigned int size,
		D3D12_RESOURCE_STATES targetState, D3D12_PLACED_SUBRESOURCE_FOOTPRINT* texFootprint = NULL);
	void beginRenderPass();
	void beginMRTRenderPass();
	int frameIndex(){
		return swapchain->GetCurrentBackBufferIndex();
	}
	//index helper
	int getRootParameterIndex(std::string name) {
		if (name == "cbvs") return 0;
		if (name == "cbps") return 2;
		if (name == "tex") return 4;
	}
};


