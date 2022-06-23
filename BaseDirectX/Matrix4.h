#pragma once
#include "vec3.h"

using Matrix4 = DirectX::XMMATRIX;
Matrix4 Identity();
Matrix4 Scale(const Vec3& s);
Matrix4 RotX(float angle);
Matrix4 RotY(float angle);
Matrix4 RotZ(float angle);

Matrix4 Translate(const Vec3& t);
Vec3 Transform(const Vec3& v, const Matrix4& m);
//Matrix4& operator*= (Matrix4& m1, const Matrix4& m2);
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
Vec3 operator * (const Vec3& v, const Matrix4& m);