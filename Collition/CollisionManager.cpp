#include "CollisionManager.h"
#include "BaseCollision.h"
#include "CollisionPrimitive.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitinifo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider *>::iterator it;
	std::forward_list<BaseCollider *>::iterator it_hit;
	float distance = maxDistance;
	XMVECTOR inter;

	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider *colA = *it;
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere *sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			if(!Collision::HitRayAndSphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if(tempDistance >= distance) continue;
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;

		}
	}
	if (result && hitinifo)
	{
		hitinifo->distance = distance;
		hitinifo->inter = inter;
		hitinifo->collider = *it_hit;
		hitinifo->model = hitinifo->collider->GetModel();
	}
	return result;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider *>::iterator itA;
	std::forward_list<BaseCollider *>::iterator itB;

	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{
		itB = itA;
		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider *colA = *itA;
			BaseCollider *colB = *itB;

			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				Sphere *sphereA = dynamic_cast<Sphere *>(colA);
				Sphere* sphereB = dynamic_cast<Sphere *>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::HitSphereAndSphere(*sphereA, *sphereB, &inter))
				{
					colA->GetModel()->OnCollision(CollisionInfo(colB->GetModel(), colB, inter));
					colB->GetModel()->OnCollision(CollisionInfo(colA->GetModel(), colA, inter));
				}
			}
		}
	}
}