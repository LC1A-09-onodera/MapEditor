#include "selingEnemy.h"
#include "../Shader/ShaderManager.h"
#include "../3DObjectParticle/3DObjectParticle.h"

EnemyModel EnemyModels::normalEnemy;
BulletModel EnemyModels::bullet;

void Bullet::Init()
{
	bulletEach.ConstInit();
	isActive = true;
	life = lifemax;
	direction = {0, 0, -1};
}

void Bullet::Update(XMFLOAT3& shieldPos, bool isShieldActive)
{
	life--;
	if (life <= 0)
	{
		isActive = false;
	}
	pos = {pos.x + direction.x * 0.1f, pos.y + direction.y * 0.1f, pos.z + direction.z * 0.1f};
	bulletEach.position = {pos.x, pos.y, pos.z, 1.0f};
	if (Collision(shieldPos) && isActive && isShieldActive)
	{
		ObjectParticles::triangle.Init(shieldPos, 5, ObjectParticle3D::ParticleType::Exprotion);
		isActive = false;
	}
}

void Bullet::Draw()
{
	if (!isActive) return;
	EnemyModels::bullet.Update(&bulletEach);
	Draw3DObject(EnemyModels::bullet);
}

bool Bullet::Collision(XMFLOAT3& position)
{
	if (Lenght(pos, position) < 1.0f)
	{
		return true;
	}
	return false;
}

void Enemy::Init(XMFLOAT3& pos)
{
	enemyEach.ConstInit();
	enemyEach.position = ConvertXMFLOAT3toXMVECTOR(pos);
	isShot = false;
}

void Enemy::Update(XMFLOAT3& shieldPos, bool isShieldActive)
{
	shotInterval++;
	if (shotInterval > intervalMax)
	{
		shotInterval = 0;
		ShotInit();
	}
	auto itr = bullets.begin();
	for (; itr != bullets.end(); ++itr)
	{
		itr->Update(shieldPos, isShieldActive);
	}
}

void Enemy::Draw()
{
	EnemyModels::normalEnemy.Update(&enemyEach);
	Draw3DObject(EnemyModels::normalEnemy);
	auto itr = bullets.begin();
	for (; itr != bullets.end(); ++itr)
	{
		itr->Draw();
	}
}

void Enemy::ShotInit()
{
	Bullet bullet;
	bullet.bulletEach.position = enemyEach.position;
	bullet.pos = ConvertXMVECTORtoXMFLOAT3(enemyEach.position);
	bullet.Init();
	isShot = true;
	bullets.push_back(bullet);
}

void EnemyModels::LoadModels()
{
	normalEnemy.CreateModel("Hole", ShaderManager::playerShader);
	bullet.CreateModel("maru", ShaderManager::playerShader);
}
