#pragma once
#include "BaseCollision.h"
#include <DirectXMath.h>

class Model;

struct RaycastHit
{
	Model *model = nullptr;
	BaseCollider *collider = nullptr;
	DirectX::XMVECTOR inter;
	float distance = 0.0f;
};