#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)
#include <wrl.h>
#include <d3dx12.h>
#include <vector>
#include <xaudio2.h>
#include <fstream>
#include "vec3.h"
#include "../WindowsAPI/WinAPI.h"

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequires) {};
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	STDMETHOD_(void, OnBufferStart) (THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnBufferEnd) (THIS_ void *pBufferContext) { delete[] pBufferContext; };
	STDMETHOD_(void, OnLoopEnd) (THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnVoiceError) (THIS_ void *pBufferCountext, HRESULT Error) {};
};

struct PeraVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class BaseDirectX
{
public:
	static XMFLOAT4 backColor;
	static HRESULT result;
	static ComPtr<ID3D12Device> dev;
	static ComPtr<IDXGIFactory6> dxgiFactory;
	static ComPtr<IDXGISwapChain4> swapchain;
	static ComPtr<ID3D12CommandAllocator> cmdAllocator;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	static ComPtr<ID3D12CommandQueue> cmdQueue;
	static ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	static vector<IDXGIAdapter1 *> adapters;
	static D3D_FEATURE_LEVEL levels[4];
	static D3D_FEATURE_LEVEL featureLevel;
	static IDXGIAdapter1 *tmpAdapter;
	static D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;
	static DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
	static ComPtr<IDXGISwapChain1> swapchain1;
	static ComPtr<ID3D12Fence> fence;
	static UINT64 fenceVal;
	static vector<ComPtr<ID3D12Resource>> backBuffers;
	static D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	static ComPtr<ID3D12Resource> depthBuffer;
	static CD3DX12_RESOURCE_DESC depthResDesc;
	static D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	static ComPtr<ID3D12DescriptorHeap> dsvHeap;
	static D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	//static const int ObjectNum = 200;
	static const int TexBufferNum = 100;
	static D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV[TexBufferNum];
	static D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV[TexBufferNum];
	static D3D12_CPU_DESCRIPTOR_HANDLE dsvH;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineMeth;
	static D3D12_RENDER_TARGET_BLEND_DESC &blenddesc;
	static ComPtr<ID3DBlob> vsBlob;
	static ComPtr<ID3DBlob> psBlob;
	static ComPtr<ID3DBlob> errorBlob;
	// 頂点レイアウト
	static D3D12_INPUT_ELEMENT_DESC inputLayout[3];
	static CD3DX12_DESCRIPTOR_RANGE descRangeCRV;
	static CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	static CD3DX12_ROOT_PARAMETER rootparam[2];
	static ComPtr<ID3D12PipelineState> pipelinestate;
	static ComPtr<ID3D12PipelineState> pipelinestateMeth;
	static ComPtr<ID3D12RootSignature> rootsignature;
	//テクスチャサンプラー
	static D3D12_STATIC_SAMPLER_DESC samplerDesc;
	//定数バッファデスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	static XMMATRIX matProjection;
	//ビュー行列の作成
	//static XMMATRIX matView;
	static float angle;
	static float clearColor[4];

	static void DebugLayerOn();
	static void Set();
	static void UpdateFront();
	static void UpdateBack();
	static void BackGroundFront();
	static void GetAdress();
};