#pragma once
#include "Matrix4.h"
struct Quaternion
{
	float x, y, z, w;
};

Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
Quaternion quaternion(Vec3& v, float angle);

float dot(const Quaternion& q1, const Quaternion& q2);
float Length(const Quaternion& q);
Quaternion Normalize(const Quaternion& q);
Quaternion Conjugate(const Quaternion& q);
Quaternion operator + (const Quaternion& q);
Quaternion operator - (const Quaternion& q);
Quaternion operator += (Quaternion& q1, const Quaternion& q2);
Quaternion operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion operator *= (Quaternion& q, float s);
Quaternion operator /= (Quaternion& q, float s);
Quaternion operator *= (Quaternion& q, const Quaternion& q2);
Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q, float s);
Quaternion operator * (float s, const Quaternion& q);
Quaternion operator / (const Quaternion& q1, float s);

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
Quaternion Lerp(const Quaternion& q1, const Quaternion q2, float t);
Matrix4 Rotate(const Quaternion& q);
Quaternion quaternion(const Matrix4& m);
Vec3 GetAxis(const Quaternion& q);