#pragma once
#include "../3DModel/Model.h"

class EnemyModel : public Model
{

};

class BulletModel : public Model
{

};

class EnemyModels
{
public:
	static EnemyModel normalEnemy;
	static BulletModel bullet;
	static void LoadModels();
};

class Bullet
{
public:
	EachInfo bulletEach;
	XMFLOAT3 direction = {0, 0, 0};
	XMFLOAT3 pos = {0, 0, 0};
	bool isActive = false;
	int life = 0;
	static const int lifemax = 180;
	void Init();
	void Update(XMFLOAT3& shieldPos, bool isShieldActive);
	void Draw();
	bool Collision(XMFLOAT3 &position);
};

class Enemy
{
	EachInfo enemyEach;
	list<Bullet> bullets;
	bool isShot;
	int shotInterval;
	const int intervalMax = 30;
public:
	void Init(XMFLOAT3 &pos);
	void Update(XMFLOAT3 &shieldPos, bool isShieldActive);
	void Draw();
	void ShotInit();
};
