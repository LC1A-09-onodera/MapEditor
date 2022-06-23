#pragma once
#include <forward_list>
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include <d3d12.h>

class BaseCollider;

class CollisionManager
{
public:
	static CollisionManager *GetInstance();
public:
	inline void AddCollider(BaseCollider* collider)
	{
		colliders.push_front(collider);
	}
	inline void RemoveCollider(BaseCollider* collider)
	{
		colliders.remove(collider);
	}
	bool Raycast(const Ray &ray, RaycastHit *hitinifo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
	void CheckAllCollisions();
private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager &) = default;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = default;
	std::forward_list<BaseCollider *> colliders;
};