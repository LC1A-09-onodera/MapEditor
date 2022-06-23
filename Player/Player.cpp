#include "Player.h"
#include "../BaseDirectX/Input.h"
#include "../Enemy/Enemy.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"
#include "../Shader/ShaderManager.h"
#include "../Hole/Hole.h"
#include "../imgui/ImguiControl.h"

Player::Player()
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 vec3 = { 0,0,0 };
	int activeCount = 0;
	int invincibleCount = 0;
	bool isActive = true;
	bool isInvincible = false;
	bool isShoot = false;
	bool isDetonating = false;
}

Player::~Player()
{
}

Player* Player::GetPlayer()
{
	static Player player;
	return &player;
}

//初期化
void Player::Init()
{
	player.CreateModel("Block", ShaderManager::playerShader);
	//water.CreateWater();

	pos = { 0,0,0 };			//プレイヤーの座標
	vec3 = { 0,0,0 };			//向いている方向（正規化済）
	hitEnemypos = { 0,0,0 };	//当たった敵の座標
	hitBombpos = { 0,0,0 };		//爆風が当たった時の爆弾の座標
	lastVec3 = { 0,0,1 };		//最後に向いていた方向
	activeCount = 0;			//行動不能カウント
	invincibleCount = 0;		//無敵カウント
	bombForce = 0;				//ボムの力保存用
	enemyForce = 0;				//敵の力保存用
	isActive = true;			//行動できるかどうか
	isHitBomb = false;			//ボムに当たって飛ばされてるかどうか
	isHitEnemy = false;			//敵に当たって飛ばされてるかどうか
	isInvincible = false;		//無敵かどうか
	isShoot = false;			//射撃中かどうか（弾があるか）
	isDetonating = false;		//起爆したかどうか
}

//更新
void Player::Update(bool isBombAlive)
{
	/*移動*/
	CheakIsInput();
	//行動可能かつ吹っ飛んでなければ
	CheakIsActive();

	/*発射トリガー*/
	CheakShootTrigger(isBombAlive);

	/*起爆トリガー*/
	CheakDetonatingTrigger(isBombAlive);

	/*自機が敵に当たった時の判定*/
	CheakHitEnemy();

	/*自機が穴に当たった時の判定*/
	CheakHitHole();

	/*自機が周りの線に当たった時の判定*/
	CheakHitDeathLine();

	/*行動不能とか無敵とか*/
	CalcActiveCount();

	/*ボムの力計算*/
	AddBombForce();

	/*敵の力計算*/
	AddEnemyForce();

	player.each.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//描画
void Player::Draw()
{
	if (isActive)
	{
		if (!isInvincible) { Draw3DObject(player); }
		else if (invincibleCount % 2 == 0) { Draw3DObject(player); }
	}
	else
	{
		if (activeCount % 3 == 0) { Draw3DObject(player); }
	}
}

void Player::HitBomb(const float& BombForce, XMFLOAT3 bombPos)
{
	if (isActive)
	{
		//ボムの力を代入
		bombForce = BombForce;

		hitBombpos = bombPos;

		bombForce *= 3.0f;

		//移動制限
		isActive = false;

		//吹っ飛び中にする
		isHitBomb = true;
	}
}

void Player::CheakIsInput()
{
	/*移動*/
	//入力なければリセット
	if (DirectInput::leftStickX() == 0.0f &&
		DirectInput::leftStickY() == 0.0f)
	{
		vec3 = { 0,0,0 };
	}
	else
	{
		vec3.x = DirectInput::leftStickX();
		vec3.z = DirectInput::leftStickY();

		lastVec3 = vec3;
		VecNormaliz(lastVec3);
	}

	if (!Input::Key(DIK_LEFT) &&
		!Input::Key(DIK_RIGHT) &&
		!Input::Key(DIK_UP) &&
		!Input::Key(DIK_DOWN))
	{
		if (DirectInput::leftStickX() == 0.0f &&
			DirectInput::leftStickY() == 0.0f)
		{
			vec3 = { 0,0,0 };
		}
	}
	else
	{
		if (DirectInput::leftStickX() == 0.0f &&
			DirectInput::leftStickY() == 0.0f)
		{
			if (Input::Key(DIK_LEFT)) { vec3.x = -1.0f; }
			if (Input::Key(DIK_RIGHT)) { vec3.x = 1.0f; }
			if (Input::Key(DIK_UP)) { vec3.z = -1.0f; }
			if (Input::Key(DIK_DOWN)) { vec3.z = 1.0f; }

			lastVec3 = vec3;
			VecNormaliz(lastVec3);
		}
	}
}

void Player::CheakIsActive()
{
	//行動可能かつ吹っ飛んでなければ
	if (isActive && !isHitBomb)
	{
		pos.x += vec3.x * MAX_SPEED;
		pos.z += -vec3.z * MAX_SPEED;
	}
}

void Player::CheakShootTrigger(bool isBombAlive)
{
	/*発射トリガー*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_B))
	{
		if (!isBombAlive && isActive) { isShoot = true; }
	}
	else { isShoot = false; }
}

void Player::CheakDetonatingTrigger(bool isBombAlive)
{
	/*起爆トリガー*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_B))
	{
		if (isBombAlive && isActive) { isDetonating = true; }
	}
	else { isDetonating = false; }
}

void Player::CheakHitEnemy()
{
	//行動不能時か無敵時か吹っ飛び中は判定無視
	if (!isActive || isInvincible || isHitBomb) { return; }

	bool isHit = false;
	auto itr = Enemys::enemys.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		XMFLOAT3 enemyPos = itr->GetPosition();
		if (std::isnan(enemyPos.x)) { return; }
		if (std::isnan(enemyPos.z)) { return; }

		//2点間の距離と判定（円）
		isHit = CheakHit(1.2f, 1.2f, pos, enemyPos);

		//当たってなかったらやり直し
		if (!isHit) { continue; }

		//当たったら行動不能にする
		hitEnemypos = itr->GetPosition();
		isActive = false;
		break;
	}

	//誰とも当たらなかった場合、終了
	if (isActive) { return; }

	enemyForce = MAX_ENEMY_FORCE;
}

void Player::CheakHitHole()
{
	//無敵時は判定無視
	if (isInvincible) { return; }

	bool isHit = false;
	auto itr = Holes::holes.begin();
	for (; itr != Holes::holes.end(); ++itr)
	{
		XMFLOAT3 holePos = itr->GetPosition();
		if (std::isnan(holePos.x)) { return; }
		if (std::isnan(holePos.z)) { return; }

		//2点間の距離と判定（円）
		isHit = CheakHit(1.2f, 1.2f, pos, holePos);

		//当たってなかったらやり直し
		if (!isHit) { continue; }

		//当たったら真ん中でリスポーンして行動不能に
		pos = { 0,0,0 };
		isActive = false;
		break;
	}
}

void Player::CheakHitDeathLine()
{
	if (pos.x > Imgui::dethLine ||
		pos.x < -Imgui::dethLine ||
		pos.z > Imgui::dethLine ||
		pos.z < -Imgui::dethLine)
	{
		pos = { 0,0,0 };
		isActive = false;
	}
}

void Player::CalcActiveCount()
{
	/*行動不能とか無敵とか*/
	//無敵かつ行動可能時
	if (isInvincible && isActive)
	{
		if (invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { invincibleCount = 0; isInvincible = false; }
	}

	//行動不能時
	else if (!isActive)
	{
		if (activeCount < MAX_STAN_COUNT) { activeCount++; }
		else
		{
			activeCount = 0;
			isInvincible = true;
			isActive = true;
		}
	}
}

void Player::AddBombForce()
{
	//吹っ飛び中だけ計算
	if (!isHitBomb) { return; }

	//移動
	//移動
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitBombpos.x;
	move.z = pos.z - hitBombpos.z;
	VecNormaliz(move);
	pos.x += bombForce * move.x;
	pos.z += bombForce * move.z;

	//減算
	bombForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にして移動再開
	if (bombForce < MINIMUM_FORCE)
	{
		bombForce = 0;
		isActive = true;
		isHitBomb = false;
	}
}

void Player::AddEnemyForce()
{
	//行動不能時だけ計算
	if (isActive) { return; }

	//移動
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitEnemypos.x;
	move.z = pos.z - hitEnemypos.z;
	VecNormaliz(move);
	pos.x += enemyForce * move.x;
	pos.z += enemyForce * move.z;

	//減算
	enemyForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にする
	if (enemyForce < MINIMUM_FORCE) { enemyForce = 0; }
}
