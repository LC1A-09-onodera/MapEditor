#pragma once

class Vec2
{
public:
	float x;
	float y;

	Vec2(float x = 0,float y = 0):
		x(x),
		y(y)
	{

	}

	Vec2 operator + (const Vec2& other) const
	{
		Vec2 v;
		v.x = this->x + other.x;
		v.y = this->y + other.y;
		return v;
	}

	Vec2 operator - (const Vec2& other) const
	{
		Vec2 v;
		v.x = this->x - other.x;
		v.y = this->y - other.y;
		return v;
	}

	Vec2 operator * (float other)
	{
		Vec2 v;
		v.x = this->x * other;
		v.y = this->y * other;
		return v;
	}

	Vec2 operator / (float other)
	{
		Vec2 v;
		v.x = this->x / other;
		v.y = this->y / other;
		return v;
	}

	//+=のオーバーロード
	Vec2 &operator += (const Vec2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vec2 &operator -= (const Vec2 &other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vec2 &operator *= (const Vec2 &other)
	{
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	Vec2 &operator /= (const Vec2 &other)
	{
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}

	float dot(const Vec2 &other) const
	{
		return x * other.x + y * other.y;
	}

	float cross(const Vec2 &other)const
	{
		return x * other.y - y * other.x;
	}

	//==演算子
	bool operator == (const Vec2& other)const
	{
		return this->x == other.x && this->y == other.y;
	}

	bool operator != (const Vec2& other)const
	{
		return !(this->x == other.x && this->y == other.y);
	}
};