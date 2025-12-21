#include "HeaderFiles/Core.h"

void Core::init(HWND hwnd, int _width, int _height) {
	//init debug
	ID3D12Debug1* debug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	debug->Release();
	//Enumerate adapters
	IDXGIAdapter1* adapterf;
	std::vector<IDXGIAdapter1*> adapters;
	IDXGIFactory6* factory = NULL;
	CreateDXGIFactory(__uuidof(IDXGIFactory6), (void**)&factory);
	int i = 0;
	while (factory->EnumAdapters1(i, &adapterf) != DXGI_ERROR_NOT_FOUND){
		adapters.push_back(adapterf);
		i++;
	}
	//Find the best adapter
	long long maxVideoMemory = 0;
	int useAdapterIndex = 0;
	for (int i = 0; i < adapters.size(); i++){
		DXGI_ADAPTER_DESC desc;
		adapters[i]->GetDesc(&desc);
		if (desc.DedicatedVideoMemory > maxVideoMemory){
			maxVideoMemory = desc.DedicatedVideoMemory;
			useAdapterIndex = i;
		}
	}
	adapter = adapters[useAdapterIndex];
	//device initialize
	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
	//commend queue initialize
	D3D12_COMMAND_QUEUE_DESC graphicsQueueDesc = {};
	graphicsQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	device->CreateCommandQueue(&graphicsQueueDesc, IID_PPV_ARGS(&graphicsQueue));
	D3D12_COMMAND_QUEUE_DESC copyQueueDesc = {};
	copyQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	device->CreateCommandQueue(&copyQueueDesc, IID_PPV_ARGS(&copyQueue));
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	device->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&computeQueue));
	//buffer initialize
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Width = _width;
	scDesc.Height = _height;
	scDesc.SampleDesc.Count = 1; // MSAA here
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//swapchain initialize
	IDXGISwapChain1* swapChain1;
	factory->CreateSwapChainForHwnd(graphicsQueue, hwnd, &scDesc, NULL, NULL, &swapChain1);
	swapChain1->QueryInterface(&swapchain);
	swapChain1->Release();
	factory->Release();
	//Create Command Allocators and Command Lists
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&graphicsCommandAllocator[0]));
	device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&graphicsCommandList[0]));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&graphicsCommandAllocator[1]));
	device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&graphicsCommandList[1]));
	//Create Heap in init
	D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc = {};
	renderTargetViewHeapDesc.NumDescriptors = scDesc.BufferCount;
	renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	device->CreateDescriptorHeap(&renderTargetViewHeapDesc, IID_PPV_ARGS(&backbufferHeap));
	//Allocate memory for Backbuffer array
	backbuffers = new ID3D12Resource * [scDesc.BufferCount];
	//Get backbuffers and create views on heap
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = backbufferHeap->GetCPUDescriptorHandleForHeapStart();
	unsigned int renderTargetViewDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (unsigned int i = 0; i < 2; i++) {
		swapchain->GetBuffer(i, IID_PPV_ARGS(&backbuffers[i]));
		device->CreateRenderTargetView(backbuffers[i], NULL, renderTargetViewHandle);
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	/*for (unsigned int i = 0; i < 2; i++) {
		swapchain->GetBuffer(i, IID_PPV_ARGS(&backbuffers[i]));
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(backbuffers[i], &rtvDesc, renderTargetViewHandle);
		renderTargetViewHandle.ptr += renderTargetViewDescriptorSize; 
	}*/
	//Create fences
	graphicsQueueFence[0].create(device);
	graphicsQueueFence[1].create(device);
	//Create Descriptor Heap
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	memset(&dsvHeapDesc, 0, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//Create Depth Buffer
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;
	D3D12_HEAP_PROPERTIES heapprops = {};
	heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapprops.CreationNodeMask = 1;
	heapprops.VisibleNodeMask = 1;
	//Specify resource information - Create Depth Buffer
	D3D12_RESOURCE_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Width = _width;
	dsvDesc.Height = _height;
	dsvDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsvDesc.DepthOrArraySize = 1;
	dsvDesc.MipLevels = 1;
	dsvDesc.SampleDesc.Count = 1;
	dsvDesc.SampleDesc.Quality = 0;
	dsvDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	dsvDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//Allocate memory for Resource - Create Depth Buffer
	device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &dsvDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&dsv));
	//Create DepthStencilView to Resource on descriptor heap - Create Depth Buffer
	device->CreateDepthStencilView(dsv, &depthStencilDesc, dsvHandle);
	//Define viewport and scissor rect
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)_width;
	viewport.Height = (float)_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = _width;
	scissorRect.bottom = _height;

	//NOTES: DONT FORGET TO UPDATE getRootParameterIndex() FUNCTION IN Core.h !!!
	//CBVS0
	std::vector<D3D12_ROOT_PARAMETER> parameters;
	D3D12_ROOT_PARAMETER rootParameterCBVS;
	rootParameterCBVS.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameterCBVS.Descriptor.ShaderRegister = 0; // Register(b0)
	rootParameterCBVS.Descriptor.RegisterSpace = 0;
	rootParameterCBVS.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	parameters.push_back(rootParameterCBVS);
	//CBVS1
	D3D12_ROOT_PARAMETER rootParameterCBVS1;
	rootParameterCBVS1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameterCBVS1.Descriptor.ShaderRegister = 1; // Register(b1)
	rootParameterCBVS1.Descriptor.RegisterSpace = 0;
	rootParameterCBVS1.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	parameters.push_back(rootParameterCBVS1);
	//CBPS
	D3D12_ROOT_PARAMETER rootParameterCBPS;
	rootParameterCBPS.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameterCBPS.Descriptor.ShaderRegister = 0; // Register(b0)
	rootParameterCBPS.Descriptor.RegisterSpace = 0;
	rootParameterCBPS.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	parameters.push_back(rootParameterCBPS);
	D3D12_ROOT_PARAMETER rootParameterCBPS1;
	rootParameterCBPS1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameterCBPS1.Descriptor.ShaderRegister = 1; // Register(b1)
	rootParameterCBPS1.Descriptor.RegisterSpace = 0;
	rootParameterCBPS1.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	parameters.push_back(rootParameterCBPS1);
	//update root signature Add range of textures
	D3D12_DESCRIPTOR_RANGE srvRange = {};
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.NumDescriptors = 8; // number of SRVs (t0–t7)
	srvRange.BaseShaderRegister = 0; // starting at t0
	srvRange.RegisterSpace = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	D3D12_ROOT_PARAMETER rootParameterTex;
	rootParameterTex.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameterTex.DescriptorTable.NumDescriptorRanges = 1;
	rootParameterTex.DescriptorTable.pDescriptorRanges = &srvRange;
	rootParameterTex.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	parameters.push_back(rootParameterTex);
	//Add sampler
	D3D12_STATIC_SAMPLER_DESC staticSampler = {};
	staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.MipLODBias = 0;
	staticSampler.MaxAnisotropy = 1;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	staticSampler.MinLOD = 0.0f;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister = 0;
	staticSampler.RegisterSpace = 0;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//Update Root Signature Description
	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.NumParameters = parameters.size();
	desc.pParameters = &parameters[0];
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//Add to desc
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = &staticSampler;
	ID3DBlob* serialized;
	ID3DBlob* error;
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error);
	device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	serialized->Release();
	srvHeap.init(device, 16384);
#if 1
//MRT test
	D3D12_DESCRIPTOR_HEAP_DESC MRTrtvHeapDesc = {};
	MRTrtvHeapDesc.NumDescriptors = mrtCount;//3 render targets
	MRTrtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	device->CreateDescriptorHeap(&MRTrtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	mrtResources = new ID3D12Resource * [MRTrtvHeapDesc.NumDescriptors];
	for (unsigned int i = 0; i < mrtCount; i++) {
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = _width;
		textureDesc.Height = _height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_HEAP_PROPERTIES defaultHeap = {};
		defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
		defaultHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		defaultHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		defaultHeap.CreationNodeMask = 1;
		defaultHeap.VisibleNodeMask = 1;

		ID3D12Resource* renderTarget;
		device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &textureDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET, NULL, IID_PPV_ARGS(&renderTarget));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHeapHandle.ptr += i * renderTargetViewDescriptorSize;
		device->CreateRenderTargetView(renderTarget, &rtvDesc, rtvHeapHandle);
		mrtResources[i] = renderTarget;
		rtvHandles.push_back(rtvHeapHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHeapHandle = srvHeap.getNextCPUHandle();
		device->CreateShaderResourceView(renderTarget, &srvDesc, srvHeapHandle);

	}
	//MRT end
#endif
}
void Core::resetCommandList() {
	unsigned int frameIndex = swapchain->GetCurrentBackBufferIndex();
	graphicsCommandAllocator[frameIndex]->Reset();
	graphicsCommandList[frameIndex]->Reset(graphicsCommandAllocator[frameIndex], NULL);
}
ID3D12GraphicsCommandList4* Core::getCommandList() {
	unsigned int frameIndex = swapchain->GetCurrentBackBufferIndex();
	return graphicsCommandList[frameIndex];
}

void Core::runCommandList() {
	getCommandList()->Close();
	ID3D12CommandList* lists[] = { getCommandList() };
	graphicsQueue->ExecuteCommandLists(1, lists);
}

void Core::flushGraphicsQueue() {
	graphicsQueueFence[0].signal(graphicsQueue);
	graphicsQueueFence[0].wait();
}
void Core::beginFrame() {
	unsigned int frameIndex = swapchain->GetCurrentBackBufferIndex();

	graphicsQueueFence[frameIndex].wait();

	//D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = backbufferHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewHandle = backbufferHeap->GetCPUDescriptorHandleForHeapStart();
	unsigned int renderTargetViewDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	renderTargetViewHandle.ptr += frameIndex * renderTargetViewDescriptorSize;

	resetCommandList();
	
	Barrier::add(backbuffers[frameIndex], D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET, getCommandList());

#if 1
	//MRT test
	getCommandList()->OMSetRenderTargets(mrtCount, rtvHandles.data(), FALSE, &dsvHandle);
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < mrtCount; i++) {
		getCommandList()->ClearRenderTargetView(rtvHandles[i], clearColor, 0, NULL);
	}
	getCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);
	//MRT test end
#else
	getCommandList()->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, &dsvHandle);
	float color[4];
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 0.0;
	color[3] = 1.0;
	getCommandList()->ClearRenderTargetView(renderTargetViewHandle, color, 0, NULL);
	getCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);
#endif
}
void Core::finishFrame(){
	unsigned int frameIndex = swapchain->GetCurrentBackBufferIndex();
	Barrier::add(backbuffers[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT, getCommandList());
	runCommandList();
	graphicsQueueFence[frameIndex].signal(graphicsQueue);
	swapchain->Present(1, 0);
}
void Core::beginRenderPass() {
	getCommandList()->SetDescriptorHeaps(1, &srvHeap.heap);
	getCommandList()->RSSetViewports(1, &viewport);
	getCommandList()->RSSetScissorRects(1, &scissorRect);
	getCommandList()->SetGraphicsRootSignature(rootSignature);
}
void Core::beginMRTRenderPass() {
	for (int i = 0; i < mrtCount; i++) {
		Barrier::add(mrtResources[i], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, getCommandList());
	}

	getCommandList()->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);
	float color[4];
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 0.0;
	color[3] = 1.0;
	getCommandList()->ClearRenderTargetView(renderTargetViewHandle, color, 0, NULL);
	getCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);

}
void Core::uploadResource(ID3D12Resource* dstResource, const void* data, unsigned int size,
	D3D12_RESOURCE_STATES targetState, D3D12_PLACED_SUBRESOURCE_FOOTPRINT* texFootprint) {

	ID3D12Resource* uploadBuffer;
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = size;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));

	void* mappeddata = NULL;
	uploadBuffer->Map(0, NULL, &mappeddata);//Get pointer to allocated memory on upload heap(Map)
	memcpy(mappeddata, data, size);
	uploadBuffer->Unmap(0, NULL);//finish

	resetCommandList();

	if (texFootprint != NULL){
		D3D12_TEXTURE_COPY_LOCATION src = {};
		src.pResource = uploadBuffer;
		src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		src.PlacedFootprint = *texFootprint;
		D3D12_TEXTURE_COPY_LOCATION dst = {};
		dst.pResource = dstResource;
		dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dst.SubresourceIndex = 0;
		getCommandList()->CopyTextureRegion(&dst, 0, 0, 0, &src, NULL);
	}
	else{
		getCommandList()->CopyBufferRegion(dstResource, 0, uploadBuffer, 0, size);
	}

	Barrier::add(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, targetState, getCommandList());

	runCommandList();

	flushGraphicsQueue();//inefficient

	uploadBuffer->Release();//inefficient


}

