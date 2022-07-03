#pragma once
#include "../FBXModel/FBXModel.h"
#include <wrl.h>
#include <d3d12.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include <DirectXMath.h>
#include <string>

class FBXObject
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	ComPtr<ID3D12Resource> constBuffTrabsform;
public:
	static const int MAX_BONES = 32;
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList *cmdList);
	void SetModel(FBXModel* model){this->model = model;}
	void PlayAnimation();
	static void SetDevice(ID3D12Device *dev){FBXObject::dev = dev;}
	static void CreateGraphicsPipeline();
	XMFLOAT3 scale = {1, 1, 1};
	XMFLOAT3 rotation = {0, 0, 0};
	XMFLOAT3 position = {0, 0, 0};
private:
	~FBXObject();
	static ID3D12Device *dev;
	static ComPtr<ID3D12RootSignature> rootSignature;
	static ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12Resource> constBufferSkin;
	XMMATRIX matWorld;
	FBXModel *model = nullptr;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	FbxTime currentTime;
	bool isPlay = false;
};