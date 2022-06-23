#include "SphereCollider.h"

void SphereCollider::Update()
{
	const XMMATRIX &matWorld = model->GetMatWorld();

	Sphere::centor = matWorld.r[3] + offset;
	Sphere::rad = radius;
}