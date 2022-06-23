#pragma once
///<summary>
///�����蔻��v���~�e�B�u
///</summary>
#include <DirectXMath.h>

///<summary>
/// ����
///</summary>
struct Sphere
{
	DirectX::XMVECTOR centor = {0, 0, 0, 1};
	float rad = 1.0f;
};

///<summary>
/// ����
///</summary>
struct Plane
{
	DirectX::XMVECTOR normal = {0, 1, 0, 0};
	float dis = 0.0f;
};

/// <summary>
/// �O�p�`
/// </summary>
struct Triangle
{
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;

	DirectX::XMVECTOR normal;

	void ComputeNormal();
};

struct Box2D
{
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 vertPos[4];
	float RX = 0.1f;
	float RY;
	float RZ = 0.1f;
	float R;
	DirectX::XMFLOAT3 vec;
	void BoxInit(DirectX::XMFLOAT3 position);
	void Update(DirectX::XMFLOAT3 position);
};

/// <summary>
/// ���C(������)
/// </summary>
struct Ray
{
	//�n�_
	DirectX::XMVECTOR start = {0, 0, 0, 1};
	//����
	DirectX::XMVECTOR direct = {1, 0, 0, 0};
};

class Collision
{
public:
	/// <summary>
	/// ���̂Ƌ���
	/// </summary>
	static bool HitSphereAndSphere(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	/// ���ʂƋ���
	///</summary>
	static bool HitSphereAndPlane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);
	
	///<summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	///</summary>
	static void ClosesPtPointToTriangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	///<summary>
	/// ���Ɩ@���t���̎O�p�`�̓����蔻��
	///</summary>
	static bool HitSphereAndTriangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// ���C�ƕ��ʂ̓����蔻��
	///</summary>
	static bool HitRayAndPlane(const Ray &ray, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// ���C�ƎO�p�`����
	///</summary>
	static bool HitRayAndTriangle(const Ray &ray, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// ���C�Ƌ��̂̔���
	///</summary>
	static bool HitRayAndSphere(const Ray &ray, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	static bool CollisionBoxAndBox(Box2D box1, Box2D box2);
};