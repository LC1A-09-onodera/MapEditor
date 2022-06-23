#pragma once
#include "CollistionType.h"
#include "../3DModel/Model.h"

class BaseCollider
{
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetModel(Model* model)
	{
		this->model = model;
	}
	inline Model* GetModel()
	{
		return model;
	}
	virtual void Update() = 0;
	inline CollisionShapeType GetShapeType()
	{
		return shapeType;
	}
protected:
	Model *model = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};