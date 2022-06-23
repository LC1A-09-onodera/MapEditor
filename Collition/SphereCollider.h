#pragma once
#include "BaseCollision.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>
using namespace DirectX;

class SphereCollider : public BaseCollider, public Sphere
{
public:
	SphereCollider(XMVECTOR offset = { 0, 0 ,0 ,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	void Update() override;
	inline void SetRadius(float radius) {this->radius = radius;}

private:
	XMVECTOR offset;
	float radius;
};