#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../BaseDirectX/Input.h"
#include <wrl.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include "../BaseDirectX/BaseDirectX.h"
#include "../BaseDirectX/viewport.h"

class SpriteCommon
{
public:
	
	//グラフィックスパイプライン
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	//スプライト用デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	//static D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc;
	static D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	//パイプラインステート
	static ComPtr<ID3D12PipelineState> spritePipelineState;
	//射影行列
	static XMMATRIX matProjection;

	static ComPtr<ID3D12DescriptorHeap> DescHeap;
	static const int SRVCount = 512;//テクスチャ最大値
	static ComPtr<ID3D12Resource> TexBuff[SRVCount];

	void Init(ComPtr<ID3DBlob> vsBlob, ComPtr<ID3DBlob> psBlob);
	HRESULT LoadSprtieGraph(UINT texNumber, const wchar_t *filename);
	SpriteCommon();
};