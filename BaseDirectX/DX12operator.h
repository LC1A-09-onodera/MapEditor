#pragma once
#include <DirectXMath.h>
#include <cstdlib>
#define PI 3.141592f

using namespace DirectX;

namespace ShlomonMath
{
	static const float Cos(float angle)
	{
		return static_cast<float>(cos(angle * PI / 180.0f));
	}
	static const float Sin(float angle)
	{
		return static_cast<float>(sin(angle * PI / 180.0f));
	}
	static uint64_t xor64() {
		static uint64_t x = 88172645463325252ULL;
		x = x ^ (x << 7);
		return x = x ^ (x >> 9);
	}
	static uint32_t xor () {
		static uint32_t y = 2463534242;
		y = y ^ (y << 13); y = y ^ (y >> 17);
		return y = y ^ (y << 5);
	}
}

static const XMFLOAT3 operator +(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

static const XMFLOAT3 operator -(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

static const XMFLOAT3 operator *(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}

static const XMFLOAT3 operator /(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	result.z = v1.z / v2.z;
	return result;
}

static const XMFLOAT3 operator /(const XMFLOAT3 v1, float s)
{
	XMFLOAT3 result;
	result.x = v1.x / s;
	result.y = v1.y / s;
	result.z = v1.z / s;
	return result;
}

static const XMFLOAT3 operator +(const XMFLOAT3 v1, const XMVECTOR v2)
{
	XMFLOAT3 result;
	result.x = v1.x + v2.m128_f32[0];
	result.y = v1.y + v2.m128_f32[1];
	result.z = v1.z + v2.m128_f32[2];
	return result;
}

static const XMFLOAT3 operator +(const XMVECTOR v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v2.x + v1.m128_f32[0];
	result.y = v2.y + v1.m128_f32[1];
	result.z = v2.z + v1.m128_f32[2];
	return result;
}

static const XMFLOAT3 operator -(const XMFLOAT3 v1, const XMVECTOR v2)
{
	XMFLOAT3 result;
	result.x = v1.x - v2.m128_f32[0];
	result.y = v1.y - v2.m128_f32[1];
	result.z = v1.z - v2.m128_f32[2];
	return result;
}

static const XMFLOAT3 operator -(const XMVECTOR v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v2.x - v1.m128_f32[0];
	result.y = v2.y - v1.m128_f32[1];
	result.z = v2.z - v1.m128_f32[2];
	return result;
}

static const XMFLOAT3 operator +(const XMFLOAT3 v1, const float v2)
{
	XMFLOAT3 result;
	result.x = v1.x + v2;
	result.y = v1.y + v2;
	result.z = v1.z + v2;
	return result;
}

static const XMFLOAT3 operator +(const float v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v2.x + v1;
	result.y = v2.y + v1;
	result.z = v2.z + v1;
	return result;
}

static const XMFLOAT3 operator -(const XMFLOAT3 v1, const float v2)
{
	XMFLOAT3 result;
	result.x = v1.x - v2;
	result.y = v1.y - v2;
	result.z = v1.z - v2;
	return result;
}

static const XMFLOAT3 operator -(const float v1, const XMFLOAT3 v2)
{
	XMFLOAT3 result;
	result.x = v2.x - v1;
	result.y = v2.y - v1;
	result.z = v2.z - v1;
	return result;
}

static const XMFLOAT3 operator *(const XMFLOAT3& v1, float s)
{
	XMFLOAT3 v;
	v.x = v1.x * s;
	v.y = v1.y * s;
	v.z = v1.z * s;
	return v;
}

static const XMFLOAT3 operator *(float s, const XMFLOAT3& v1)
{
	XMFLOAT3 v;
	v.x = v1.x * s;
	v.y = v1.y * s;
	v.z = v1.z * s;
	return v;
}

//static XMVECTOR operator + (const XMVECTOR v1, const XMFLOAT3& v2)
//{
//	XMVECTOR v;
//	v.m128_f32[0] += v2.x;
//	v.m128_f32[1] += v2.y;
//	v.m128_f32[2] += v2.z;
//	return v;
//}

static XMVECTOR operator - (const XMVECTOR v1, const XMFLOAT3& v2)
{
	XMVECTOR v;
	v.m128_f32[0] -= v2.x;
	v.m128_f32[1] -= v2.y;
	v.m128_f32[2] -= v2.z;
	return v;
}

static const XMFLOAT3 ConvertXMVECTORtoXMFLOAT3(const XMVECTOR& v)
{
	XMFLOAT3 result;
	result.x = v.m128_f32[0];
	result.y = v.m128_f32[1];
	result.z = v.m128_f32[2];
	return result;
}

static const XMVECTOR ConvertXMFLOAT3toXMVECTOR(const XMFLOAT3& v)
{
	XMVECTOR result;
	result.m128_f32[0] = v.x;
	result.m128_f32[1] = v.y;
	result.m128_f32[2] = v.z;
	return result;
}

static XMFLOAT3 Normalize(const XMFLOAT3& v)
{
	float leg = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	XMFLOAT3 result;
	result.x = v.x / leg;
	result.y = v.y / leg;
	result.z = v.z / leg;
	return result;
}

static XMFLOAT3 Lerp(const XMFLOAT3& start, XMFLOAT3& end, const float t)
{
	XMFLOAT3 result, result2;
	result = start * (1.0f - t);
	result2 = end * t;
	result.x = result.x + result2.x;
	result.y = result.y + result2.y;
	result.z = result.z + result2.z;
	return result;
}

static XMFLOAT3 Cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}
namespace ShlomonMath
{
	static XMFLOAT3 EaseInQuad(const XMFLOAT3& start, XMFLOAT3& end, const float t)
	{
		XMFLOAT3 result;
		XMFLOAT3 sub = end - start;
		result.x = start.x + ((t * t) * sub.x);
		result.y = start.y + ((t * t) * sub.y);
		result.z = start.z + ((t * t) * sub.z);
		return result;
	}

	static XMFLOAT3 EaseOutQuad(const XMFLOAT3& start, XMFLOAT3& end, const float t)
	{
		XMFLOAT3 result;
		XMFLOAT3 sub = end - start;
		result.x = start.x + ((1.0f - (1.0f - t) * (1.0f - t)) * sub.x);
		result.y = start.y + ((1.0f - (1.0f - t) * (1.0f - t)) * sub.y);
		result.z = start.z + ((1.0f - (1.0f - t) * (1.0f - t)) * sub.z);
		return result;
	}

	static XMFLOAT3 EaseInOutQuad(const XMFLOAT3& start, XMFLOAT3& end, const float t)
	{
		XMFLOAT3 result;
		XMFLOAT3 sub = end - start;
		if (t < 0.5f)
		{
			result.x = start.x + (2.0f * (t * t) * sub.x);
			result.y = start.y + (2.0f * (t * t) * sub.y);
			result.z = start.z + (2.0f * (t * t) * sub.z);
		}
		else
		{
			result.x = static_cast<float>(start.x + ((1.0f - pow(static_cast<double>(-2.0f) * t + static_cast<double>(2.0f), 2) / 2.0f) * sub.x));
			result.y = static_cast<float>(start.y + ((1.0f - pow(static_cast<double>(-2.0f) * t + static_cast<double>(2.0f), 2) / 2.0f) * sub.y));
			result.z = static_cast<float>(start.z + ((1.0f - pow(static_cast<double>(-2.0f) * t + static_cast<double>(2.0f), 2) / 2.0f) * sub.z));
		}
		return result;
	}

	static XMFLOAT3 EaseInBack(const XMFLOAT3& start, XMFLOAT3& end, const float t)
	{
		XMFLOAT3 result;
		XMFLOAT3 sub = end - start;
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;
		result.x = start.x + ((c3 * t * t * t - c1 * t * t) * sub.x);
		result.y = start.y + ((c3 * t * t * t - c1 * t * t) * sub.y);
		result.z = start.z + ((c3 * t * t * t - c1 * t * t) * sub.z);
		return result;
	}

	static XMFLOAT3 EaseOutBack(const XMFLOAT3& start, XMFLOAT3& end, const float t)
	{
		XMFLOAT3 result;
		XMFLOAT3 sub = end - start;
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		result.x = static_cast<float>(start.x + (1.0f + c3 * pow(t - static_cast<double>(1.0f), 3.0f) + c1 * pow(t - static_cast<double>(1.0f), 2.0f)) * sub.x);
		result.y = static_cast<float>(start.y + (1.0f + c3 * pow(t - static_cast<double>(1.0f), 3.0f) + c1 * pow(t - static_cast<double>(1.0f), 2.0f)) * sub.y);
		result.z = static_cast<float>(start.z + (1.0f + c3 * pow(t - static_cast<double>(1.0f), 3.0f) + c1 * pow(t - static_cast<double>(1.0f), 2.0f)) * sub.z);
		return result;
	}

	static XMFLOAT3 Homing(const XMFLOAT3& nowPosition, const XMFLOAT3& target, const XMFLOAT3 speed)
	{
		XMFLOAT3 toTarget;
		toTarget = target - nowPosition;
		XMFLOAT3 cross = Cross(toTarget, speed);
		return cross;
	}
}

static float Lenght(XMVECTOR position1, XMVECTOR position2)
{
	float result;
	XMFLOAT3 leng;
	leng.x = position1.m128_f32[0] - position2.m128_f32[0];
	leng.y = position1.m128_f32[1] - position2.m128_f32[1];
	leng.z = position1.m128_f32[2] - position2.m128_f32[2];
	result = sqrtf((leng.x * leng.x) + (leng.y * leng.y) + (leng.z * leng.z));
	return result;
}

static float Lenght(XMFLOAT3 position1, XMFLOAT3 position2)
{
	float result;
	XMFLOAT3 leng;
	leng.x = position1.x - position2.x;
	leng.y = position1.y - position2.y;
	leng.z = position1.z - position2.z;
	result = sqrtf((leng.x * leng.x) + (leng.y * leng.y) + (leng.z * leng.z));
	return result;
}

static float Lenght(XMVECTOR position1, XMFLOAT3 position2)
{
	float result;
	XMFLOAT3 leng;
	leng.x = position1.m128_f32[0] - position2.x;
	leng.y = position1.m128_f32[1] - position2.y;
	leng.z = position1.m128_f32[2] - position2.z;
	result = sqrtf((leng.x * leng.x) + (leng.y * leng.y) + (leng.z * leng.z));
	return result;
}

static float Lenght(XMFLOAT3 &v)
{
	float result;
	XMFLOAT3 leng;
	leng.x = v.x;
	leng.y = v.y;
	leng.z = v.z;
	result = sqrtf((leng.x * leng.x) + (leng.y * leng.y) + (leng.z * leng.z));
	return result;
}

static float length(const float v1, const float v2)
{
	float result = 0.0f;

	return result;
};

static XMFLOAT2 Normalize(const float v1, const float v2)
{
	XMFLOAT2 result = {};

	return result;
}

static XMFLOAT2 Dot(const float v1, const float v2)
{
	XMFLOAT2 result = {};

	return result;
}

static float Magnitude(const XMFLOAT3& v)
{
	float res = v.x * v.x + v.y * v.y + v.z * v.z;
	return sqrtf(res);
}

static float Dot(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static XMFLOAT3 GetRandom(int length)
{
	XMFLOAT3 result;
	result.x = (float)(std::rand() % (length * 2)) - length + 0.5f;
	result.y = (float)(std::rand() % (length * 2)) - length + 0.5f;
	result.z = (float)(std::rand() % (length * 2)) - length + 0.5f;
	return result;
}
