#include "Matrix4.h"

Matrix4 Identity()
{
	static const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

Matrix4 Scale(const Vec3& s)
{
	Matrix4 result
	{
		s.v.x,0.0f,0.0f,0.0f,
		0.0f,s.v.y,0.0f,0.0f,
		0.0f,0.0f,s.v.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

Matrix4 RotX(float angle)
{
	float sind = sin(angle);
	float cosd = cos(angle);

	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosd,sind,0.0f,
		0.0f,-sind,cosd,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

Matrix4 RotY(float angle)
{
	float sind = sin(angle);
	float cosd = cos(angle);
	Matrix4 result
	{
		cosd,0.0f,-sind,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sind,0.0f,cosd,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

Matrix4 RotZ(float angle)
{
	float sind = sin(angle);
	float cosd = cos(angle);
	Matrix4 result
	{
		cosd,sind,0.0f,0.0f,
		-sind,cosd,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

Matrix4 Translate(const Vec3& t)
{
	Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.v.x,t.v.y,t.v.z,0.0f,
	};
	return result;
}

Vec3 Transform(const Vec3& v, const Matrix4& m)
{
	float w = v.v.x * m.r[0].m128_f32[3] + v.v.y * m.r[1].m128_f32[3] + v.v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];
	Vec3 result
	{
		(v.v.x * m.r[0].m128_f32[0] + v.v.y * m.r[1].m128_f32[0] + v.v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.v.x * m.r[0].m128_f32[1] + v.v.y * m.r[1].m128_f32[1] + v.v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.v.x * m.r[0].m128_f32[2] + v.v.y * m.r[1].m128_f32[2] + v.v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w,
	};
	return result;
}

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;
	return result *= m2;
}

Vec3 operator*(const Vec3& v, const Matrix4& m)
{
	return Transform(v, m);
}
