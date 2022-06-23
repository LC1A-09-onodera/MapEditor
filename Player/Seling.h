#pragma once
#include "../3DModel/Model.h"
#include "../BaseDirectX/DX12operator.h"
#include "../Enemy/selingEnemy.h"
#include "../Rewired/Rewired.h"

class ShieldModel : public Model
{
public:
	
	bool InitializeGraphicsPipeline(HLSLShader& shader);
	void CreateModel(const char* name, HLSLShader& shader, bool smoothing);
};

class Seling
{
	
	int nowNumber;
	XMFLOAT3 frontDirection;
	float angle;

	bool isMoveForce;
	XMFLOAT3 addForce;
	XMFLOAT3 subForce;
	XMFLOAT3 maxForce;

	Model shotModel;
	XMFLOAT3 shotPos;
	bool isShot;

	ShieldModel shieldModel;
	bool isShield;
	XMFLOAT3 shieldPos;
	XMFLOAT3 scaleStart;
	XMFLOAT3 scaleEnd;
	float easeTime;
	int shieldTime;
	const int shieldMaxTime = 180;

	Enemy enemy;
	Rewired playerShieldKey;

	const float addShieldRotaion = 30.0f;
	const float shieldR = 2.2f;
	const float addForcePower = 0.02f;
public:
	Model seling;
	void AddForce(XMFLOAT3& force)
	{
		if (this->addForce.x < maxForce.x && this->addForce.x > -maxForce.x)
		{
			this->addForce.x = this->addForce.x + force.x;
		}
		if (this->addForce.y < maxForce.y && this->addForce.y > -maxForce.y)
		{
			this->addForce.y = this->addForce.y + force.y;
		}
		if (this->addForce.z < maxForce.z && this->addForce.z > -maxForce.z)
		{
			this->addForce.z = this->addForce.z + force.z;
		}
		isMoveForce = true;
	}
	XMFLOAT3 GetDirection()
	{
		return Normalize(frontDirection);
	}
	void ForceUpdate();
	void LoadModel();
	void Init();
	void Update();
	void Draw(bool isRCamera = false);

	void ForceAttach();
	void Move();

	//上下する動き
	void VerticalWaterResist();
	void HolizontalWaterResist();

	void CreateSimpleHarmonicMotion();

	//弾が出る
	void ShotInit();
	void ShotUpdate();
	void ShotInitAndUpdate();

	//シールドが出る
	void ShieldInit();
	void ShieldUpdate();
	void ShieldInitAndUpdate();
};