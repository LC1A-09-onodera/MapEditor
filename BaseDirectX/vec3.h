#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Vec3
{
public:
	XMFLOAT3 v;

	Vec3(float x = 0, float y = 0, float z = 0)
	{
		v.x = x;
		v.y = y;
		v.z = z;
	};

	float length() const
	{
		return sqrtf(this->v.x * this->v.x + this->v.y * this->v.y + this->v.z * this->v.z);
	};

	Vec3 nomalize()
	{
		Vec3 v;
		v.v.x = this->v.x / length();
		v.v.y = this->v.y / length();
		v.v.x = this->v.z / length();
		return v;
	}

	float dot(const Vec3& other) const
	{
		return this->v.x * other.v.x + this->v.y * other.v.y + this->v.z * other.v.z;
	}

	Vec3 cross(const Vec3& other) const
	{
		Vec3 v;
		v.v.x = this->v.y * other.v.z - this->v.z * other.v.y;
		v.v.y = this->v.z * other.v.x - this->v.x * other.v.z;
		v.v.z = this->v.x * other.v.y - this->v.y * other.v.x;
		return v;
	}

	Vec3 operator + (const Vec3& other) const
	{
		Vec3 v;
		v.v.x = this->v.x + other.v.x;
		v.v.y = this->v.y + other.v.y;
		v.v.z = this->v.z + other.v.z;
		return v;
	}

	Vec3 operator - (const Vec3& other) const
	{
		Vec3 v;
		v.v.x = this->v.x - other.v.x;
		v.v.y = this->v.y - other.v.y;
		v.v.z = this->v.z - other.v.z;
		return v;
	}

	//+=のオーバーロード
	Vec3& operator += (const Vec3& other)
	{
		this->v.x += other.v.x;
		this->v.y += other.v.y;
		this->v.z += other.v.z;
		return *this;
	}

	Vec3& operator -= (const Vec3& other)
	{
		this->v.x -= other.v.x;
		this->v.y -= other.v.y;
		this->v.z -= other.v.z;
		return *this;
	}

	Vec3& operator *= (float other)
	{
		this->v.x *= other;
		this->v.y *= other;
		this->v.z *= other;
		return *this;
	}

	Vec3& operator /= (float other)
	{
		this->v.x /= other;
		this->v.y /= other;
		this->v.z /= other;
		return *this;
	}

};


