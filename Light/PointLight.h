#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class PointLight
{
private:

public:
	struct ConstBuffData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pod2;
		XMFLOAT3 lighttatten;
		unsigned int active;
	};
	inline void SetLightPos(const XMFLOAT3 &lightpos){this->lightpos = lightpos;}
	inline const XMFLOAT3 &GetLightPos(){return lightpos;}
	inline void SetLightColor(const XMFLOAT3 &lightcolor){this->lightcolor = lightcolor;}
	inline const XMFLOAT3 &GetColor(){return lightcolor;}
	inline void SetLightAtten(const XMFLOAT3 &lightatten){this->lighttatten = lightatten;}
	inline const XMFLOAT3 &GetAtten(){return lighttatten;}
	inline void SetActive(bool active){this->active = active;}
	inline bool isActive(){return active;}
private:
	//座標
	XMFLOAT3 lightpos = { 0, 0, 0 };
	//ライトの色
	XMFLOAT3 lightcolor = { 1, 1, 1 };
	//距離減衰係数
	XMFLOAT3 lighttatten = { 1.0f, 1.0f, 1.0f};
	bool active = false;
};