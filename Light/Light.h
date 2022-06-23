#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include "../Light/PointLight.h"
#include "../Light/SpotLight.h"
#include "../Light/CircleShadow.h"
#include "../BaseDirectX//DX12operator.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Light
{
private:
	static ID3D12Device *device;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;
	
public:
	~Light();
	struct ConstBufferData
	{
		XMVECTOR lightv;	//ライトの方向ベクトル
		XMFLOAT3 lightcolor;//ライトの色
		float pad1;
		PointLight::ConstBuffData pointLights[PointLightNum];
		//float pad2;
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};
	static void StaticInitialize(ID3D12Device *device);
	void Update();
	void Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex);
	static Light *Create();
	void SetLightDir(const XMVECTOR &lightdir);
	void SetLightColor(const XMFLOAT3 &lightcolor);
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3 &lightpos);
	void SetPointLightColor(int index, const XMFLOAT3 &lightcolor);
	void SetPointLightAtten(int index, const XMFLOAT3 &lightatten);

	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR &lightdir);
	void SetSpotLightPos(int index, const XMFLOAT3 &lightpos);
	void SetSpotLightColor(int index, const XMFLOAT3 &lightcolor);
	void SetSpotLightAtten(int index, const XMFLOAT3 &lightatten);
	void SetSpotLightAngle(int index, const XMFLOAT2 &lightFactorAngle);

	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const XMFLOAT3 &casterPos);
	void SetCircleShadowDir(int index, XMVECTOR &lightdir);
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	void SetCircleShadowAtten(int index, const XMFLOAT3 lightAtten);
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2 &lightfactorAngle);
	void SetLightDir(XMFLOAT3 dir){lightdir = ConvertXMFLOAT3toXMVECTOR(dir);}

private:
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライト方向ベクトル
	XMVECTOR lightdir = {0, 0, 1.0f, 0};
	//ライトの色
	XMFLOAT3 lightcolor = {0.5f, 0.5f, 0.5f};
	//ダーティフラグ
	bool dirty = false;
	void CreateBuff();
	void Init();
	void TransConstBuffer();
	
	PointLight pointLight[PointLightNum];
	SpotLight spotLights[SpotLightNum];
	CircleShadow circleShadows[CircleShadowNum];
};