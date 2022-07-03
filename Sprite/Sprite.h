#pragma once
//#include <Windows.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)
#include "../BaseDirectX/Input.h"
#include <wrl.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include "../BaseDirectX/BaseDirectX.h"
#include "SpriteCommon.h"
#include "../Texture/Texture.h"
#include "../3DModel/Model.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	static SpriteCommon common;
	Tex tex;
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;	// エラーオブジェクト
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> spriteRootSignature;
	ComPtr<ID3DBlob> rootSigBlob;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// 頂点バッファの設定
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//ビューのアドレスを保存
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ルートパラメーター
	CD3DX12_ROOT_PARAMETER rootParams[2];
	bool back;
	float rotation = 0.0f;
	int texNum;
	//射影行列
	XMVECTOR position{};
	XMMATRIX matProjection{};
	XMMATRIX matWorld{};
	XMFLOAT2 texLeftTop = {0,0};
	XMFLOAT2 texSize = {100,100};

	//スプライトの生成
	void CreateSprite(const wchar_t* graph, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff = nullptr, bool back = false, bool TexSize = true);
	void CreateSprite(Tex tex, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff = nullptr, bool back = false, bool TexSize = true);
	//スプライト用パイプラインの設定コマンド
	
	//スプライト用描画コマンド
	//描画毎フレーム処理
	void SpriteDraw();
	//座標変更
	void Move();
	//表示サイズの変更
	void ChangeSize(float wid,float hei);
	void ChangeSizeOther(float wid, float hei);
	
	//毎フレーム処理
	private:
	void SpritePipelineEdit();
};