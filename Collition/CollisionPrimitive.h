#pragma once
///<summary>
///当たり判定プリミティブ
///</summary>
#include <DirectXMath.h>

///<summary>
/// 球体
///</summary>
struct Sphere
{
	DirectX::XMVECTOR centor = {0, 0, 0, 1};
	float rad = 1.0f;
};

///<summary>
/// 平面
///</summary>
struct Plane
{
	DirectX::XMVECTOR normal = {0, 1, 0, 0};
	float dis = 0.0f;
};

/// <summary>
/// 三角形
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
/// レイ(半直線)
/// </summary>
struct Ray
{
	//始点
	DirectX::XMVECTOR start = {0, 0, 0, 1};
	//方向
	DirectX::XMVECTOR direct = {1, 0, 0, 0};
};

class Collision
{
public:
	/// <summary>
	/// 球体と球体
	/// </summary>
	static bool HitSphereAndSphere(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	/// 平面と球体
	///</summary>
	static bool HitSphereAndPlane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);
	
	///<summary>
	/// 点と三角形の最近接点を求める
	///</summary>
	static void ClosesPtPointToTriangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	///<summary>
	/// 級と法線付きの三角形の当たり判定
	///</summary>
	static bool HitSphereAndTriangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// レイと平面の当たり判定
	///</summary>
	static bool HitRayAndPlane(const Ray &ray, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// レイと三角形判定
	///</summary>
	static bool HitRayAndTriangle(const Ray &ray, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	///<summary>
	/// レイと球体の判定
	///</summary>
	static bool HitRayAndSphere(const Ray &ray, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	static bool CollisionBoxAndBox(Box2D box1, Box2D box2);
};