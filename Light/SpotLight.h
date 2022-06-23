#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class SpotLight
{
public:
	struct ConstBufferData
	{
		
		XMVECTOR lightv;
		//float pad1;
		XMFLOAT3 lightpos;
		float pad2;
		XMFLOAT3 lightcolor;
		float pad3;
		XMFLOAT3 lightatten;
		float pad4;
		XMFLOAT2 lightfactoranglecos;
		unsigned int active;
		float pad5;
	};
private:
	XMVECTOR lightdir = {1, 0, 0, 0};
	XMFLOAT3 lightpos = {0, 0, 0};
	XMFLOAT3 lightcolor = {0, 0, 0};
	XMFLOAT3 lightAtten = {1.0f, 1.0f, 1.0f};
	//開始角度、終了角度
	XMFLOAT2 lightFactorAngleCos = {0.5f, 0.2f};
	bool active = false;

public:
	inline void SetLightDir(const XMVECTOR &lightdir){this->lightdir = lightdir;}
	inline const XMVECTOR &GetLightDir(){return lightdir;};
	inline void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; }
	inline const XMFLOAT3& GetLightPos() { return lightpos; };
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; };
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; }
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; };
	inline void SetLightFactorAngle(const XMFLOAT2& lightFactorAngle) { this->lightFactorAngleCos.x = cosf(XMConvertToRadians(lightFactorAngle.x));
																		this->lightFactorAngleCos.y = cosf(XMConvertToRadians(lightFactorAngle.y));}
	inline const XMFLOAT2& GetLightFactorAngleCos() { return lightFactorAngleCos; };
	inline void SetActive(bool active){this->active = active;}
	inline bool isActive(){return active;}
};