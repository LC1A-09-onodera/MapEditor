#include "CollisionPrimitive.h"
#include "../BaseDirectX/DX12operator.h"

using namespace DirectX;

bool Collision::HitSphereAndSphere(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMVECTOR* inter)
{
	return false;
}

bool Collision::HitSphereAndPlane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter)
{
	XMVECTOR distV = XMVector3Dot(sphere.centor, plane.normal);
	float dist = distV.m128_f32[0] - plane.dis;
	if (fabsf(dist) > sphere.rad)
	{
		return false;
	}
	if (inter)
	{
		*inter = -dist * plane.normal + sphere.centor;
	}
	return true;
}

void Collision::ClosesPtPointToTriangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::HitSphereAndTriangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter)
{
	//接点p
	XMVECTOR point;
	ClosesPtPointToTriangle(sphere.centor, triangle, &point);
	//差分のベクトル
	XMVECTOR v = point - sphere.centor;
	v = XMVector3Dot(v,v);

	if (v.m128_f32[0] > sphere.rad * sphere.rad)
	{
		return false;
	}

	if (inter)
	{
		*inter = point;
	}
	return true;
}

bool Collision::HitRayAndPlane(const Ray &ray, const Plane &plane, float *distance, DirectX::XMVECTOR *inter)
{
	const float epslion = 1.0e-5f;//誤差の値
	//内積
	float dot1 = XMVector3Dot(plane.normal, ray.direct).m128_f32[0];
	if (dot1 > -epslion)
	{
		return false;//裏面との接触
	}
	//面の法線とレイの内積
	float dot2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//始点と平面の距離(平面の方向)
	float dis = dot2 - plane.dis;
	//始点方向からの距離
	float t = dis / -dot1;
	if (t < 0)
	{
		return false;
	}
	if (distance)
	{
		*distance = t;
	}
	if (inter)
	{
		*inter = ray.start + t * ray.direct;
	}
	return true;

	return false;
}

bool Collision::HitRayAndTriangle(const Ray &ray, const Triangle &triangle, float *distance, DirectX::XMVECTOR *inter)
{
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.dis = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	if (!HitRayAndPlane(ray, plane, distance, &interPlane))
	{
		return false;//レイと平面が当たってない
	}
	const float epsilon = 1.0e-5f;//誤差用
	XMVECTOR m;
	//辺p0_p1
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;//辺の外側
	}
	//p1_p2
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;//辺の外側
	}
	//p2_p0
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0- triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;//辺の外側
	}

	if (inter)
	{
		*inter = interPlane;
	}
	return true;
}

bool Collision::HitRayAndSphere(const Ray &ray, const Sphere &sphere, float *distance, DirectX::XMVECTOR *inter)
{
	XMVECTOR m = ray.start - sphere.centor;
	float b = XMVector3Dot(m, ray.direct).m128_f32[0];
	float c = XMVector3Dot(m,m).m128_f32[0] - sphere.rad * sphere.rad;
	//sphere外かつ離れる方向
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}
	float discr = b * b -c;
	if (discr < 0.0f)
	{
		return false;
	}

	float t = -b - sqrtf(discr);
	if (t < 0)
	{
		t = 0.0f;
	}
	if (distance)
	{
		*distance = t;
	}
	if (inter)
	{
		*inter = ray.start * t * ray.direct;
	}
	return true;
}

void Triangle::ComputeNormal()
{
	XMVECTOR p0_p1 = p1 - p0;
	XMVECTOR p0_p2 = p2 - p0;

	//外積でベクトルを出す
	normal = XMVector3Cross(p0_p1, p0_p2);
	normal = XMVector3Normalize(normal);
}

void Box2D::BoxInit(XMFLOAT3 position)
{
	center = position;
	XMFLOAT3 vert0 = center + XMFLOAT3(RZ, 0, RX);//右上
	vertPos[0] = vert0;
	XMFLOAT3 vert1 = center + XMFLOAT3(RZ, 0, -RX);//右下
	vertPos[1] = vert1;
	XMFLOAT3 vert2 = center + XMFLOAT3(-RZ, 0, RX);//左上
	vertPos[2] = vert2;
	XMFLOAT3 vert3 = center + XMFLOAT3(-RZ, 0, RX);//左下
	vertPos[3] = vert3;
}

void Box2D::Update(XMFLOAT3 position)
{
	center = position;
	XMFLOAT3 vert0 = center + XMFLOAT3(RZ, 0, RX);//右上
	vertPos[0] = vert0;
	XMFLOAT3 vert1 = center + XMFLOAT3(RZ, 0, -RX);//右下
	vertPos[1] = vert1;
	XMFLOAT3 vert2 = center + XMFLOAT3(-RZ, 0, RX);//左上
	vertPos[2] = vert2;
	XMFLOAT3 vert3 = center + XMFLOAT3(-RZ, 0, RX);//左下
	vertPos[3] = vert3;
}

bool Collision::CollisionBoxAndBox(Box2D box1, Box2D box2)
{
	float distanceZ, distanceX;
	distanceZ = static_cast<float>(fabs(static_cast<double>(box1.center.z - box2.center.z)));
	distanceX = static_cast<float>(fabs(static_cast<double>(box1.center.x - box2.center.x)));
	float addRX, addRZ;
	addRZ = box1.RZ + box2.RZ;
	addRX = box1.RX + box2.RX;

	if (distanceZ < addRZ && distanceX < addRX)
	{
		return true;
	}
	return false;
}