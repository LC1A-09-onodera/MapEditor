#include "RenderTarget.h"
#include "../WindowsAPI/WinAPI.h"

const float RenderTarget::clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

void RenderTarget::TexInit()
{
	//テクスチャバッファの作成
	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, WindowsAPI::window_width, (UINT)WindowsAPI::window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0), D3D12_HEAP_FLAG_NONE, &texDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor), IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(BaseDirectX::result));

		//イメージデータの作成
		const UINT pixelCount = WindowsAPI::window_width * WindowsAPI::window_height;
		const UINT rowPitch = sizeof(UINT) * WindowsAPI::window_width;
		const UINT depthPitch = rowPitch * WindowsAPI::window_height;
		UINT* img = new UINT[pixelCount];
		for (int j = 0; j < pixelCount; j++)
		{
			img[j] = 0x000000ff;
		}
		BaseDirectX::result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(BaseDirectX::result));
		delete[] img;
	}
}

void RenderTarget::RTVInit()
{
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = RenderTarget::renderNum;
	BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(BaseDirectX::result));
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		BaseDirectX::dev->CreateRenderTargetView(texBuff[i].Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, BaseDirectX::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}
