#pragma once
#include "../3DModel/Model.h"
#include "../sampleObject/sampleObject.h"
#include "../Collition/CollisionPrimitive.h"
#include "../Sound/Sound.h"
//#include <list>

using namespace DirectX;

//const int MAX_STAN_COUNT = 300;			//行動不能フレーム
//const int MAX_INVICIBLE_COUNT = 150;	//無敵フレーム
//const float MAX_ENEMY_FORCE = 10.0f;	//敵と当たった時に吹っ飛ぶ力（10.0->9.0->8.0,,,,）
//
//const float RESISTANCE_VALUE = 1.0f;	//Forceを減算する定数
//const float MINIMUM_FORCE = 2.0f;		//数値以下になったら強制的にForceを0にする
//
//const float MAX_SPEED = 0.1f;			//自機スピード

static XMFLOAT3 VecNormaliz(XMFLOAT3& vector)
{
	float leg = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x = vector.x / leg;
	vector.y = vector.y / leg;
	vector.z = vector.z / leg;
	return vector;
}

class Player final
{
private:
	Player();
	~Player();

	const int MAX_STAN_COUNT = 30;			//行動不能フレーム
	const int MAX_INVICIBLE_COUNT = 30;		//無敵フレーム

	const float MAX_ENEMY_FORCE = 4.0f;		//敵と当たった時に吹っ飛ぶ力（10.0->9.0->8.0,,,,）

	const float RESISTANCE_VALUE = 1.0f;	//Forceを減算する定数
	const float MINIMUM_FORCE = 2.0f;		//数値以下になったら強制的にForceを0にする

	const float MAX_SPEED = 0.5f;			//自機スピード

	XMFLOAT3 pos;			//プレイヤーの座標
	XMFLOAT3 vec3;			//向いている方向（正規化済）
	XMFLOAT3 hitEnemypos;	//当たった敵の座標
	XMFLOAT3 hitBombpos;	//爆風が当たった時の爆弾の座標
	XMFLOAT3 lastVec3;		//最後に向いていた方向
	int activeCount;		//行動不能カウント
	int invincibleCount;	//無敵カウント
	float bombForce;		//ボムの力保存用
	float enemyForce;		//敵の力保存用
	bool isActive;			//行動できるかどうか
	bool isHitBomb;			//ボムに当たって飛ばされてるかどうか
	bool isHitEnemy;		//敵に当たって飛ばされてるかどうか
	bool isInvincible;		//無敵かどうか
	bool isShoot;			//射撃中かどうか（弾があるか）
	bool isDetonating;		//起爆したかどうか

private:
	XMVECTOR oldPlayerPos;
	Box2D playerCollision;
	SampleObject player;
	int particleTime;
	bool isParticle;

public:
	Player(const Player& obj) = delete;
	Player& operator=(const Player& obj) = delete;
	static Player* GetPlayer();
	//初期化
	void Init();
	//更新
	void Update(bool isBombAlive);
	//描画
	void Draw();

public:
	/*----------↓情報取得用↓----------*/

	XMFLOAT3 GetPos() { return pos; }						//プレイヤーの座標					がわかるよ
	XMFLOAT3 GetVec3() { return vec3; }						//向いている方向（正規化済）			がわかるよ
	XMFLOAT3 GetLastVec3() { return lastVec3; }				//最後に向いていた方向（正規化済）	がわかるよ
	bool IsActive() { return isActive; }					//行動できるか						がわかるよ
	bool IsInvincible() { return isInvincible; }			//無敵かどうか						がわかるよ

	bool IsShootTrigger() { return isShoot; }				//射撃した瞬間						がわかるよ
	bool IsDetonatingTrigger() { return isDetonating; }		//起爆した瞬間						がわかるよ

	/*----------↑情報取得用↑----------*/


	void HitBomb(const float& BombForce, XMFLOAT3 bombPos);	//Hit時に力入れて呼んでね


private://チェックだけじゃなくて処理も入ってます！！！！！カス
	void CheakIsInput();								//入力があるか
	void CheakIsActive();								//移動可能状態か
	void CheakShootTrigger(bool isBombAlive);			//射撃したか
	void CheakDetonatingTrigger(bool isBombAlive);		//起爆したか
	void CheakHitEnemy();								//敵が当たったか
	void CheakHitHole();								//穴との判定
	void CheakHitDeathLine();							//線との判定

	void CalcActiveCount();								//カウントの計算
	void AddBombForce();								//ボムと当たった際に計算
	void AddEnemyForce();								//敵と当たった際に計算

private:
	bool CheakHit(float r1, float r2, XMFLOAT3 pos1, XMFLOAT3 pos2) {
		float diff;
		diff = sqrtf(
			(pos1.x - pos2.x) * (pos1.x - pos2.x) +
			(pos1.y - pos2.y) * (pos1.y - pos2.y) +
			(pos1.z - pos2.z) * (pos1.z - pos2.z));

		float r = r1 + r2;

		if (r < diff) { return false; }
		else { return true; }
	}
};