#pragma once
#include "../3DModel/Model.h"
#include "../sampleObject/sampleObject.h"
#include "../Collition/CollisionPrimitive.h"
#include "../Sound/Sound.h"
//#include <list>

using namespace DirectX;

//const int MAX_STAN_COUNT = 300;			//�s���s�\�t���[��
//const int MAX_INVICIBLE_COUNT = 150;	//���G�t���[��
//const float MAX_ENEMY_FORCE = 10.0f;	//�G�Ɠ����������ɐ�����ԗ́i10.0->9.0->8.0,,,,�j
//
//const float RESISTANCE_VALUE = 1.0f;	//Force�����Z����萔
//const float MINIMUM_FORCE = 2.0f;		//���l�ȉ��ɂȂ����狭���I��Force��0�ɂ���
//
//const float MAX_SPEED = 0.1f;			//���@�X�s�[�h

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

	const int MAX_STAN_COUNT = 30;			//�s���s�\�t���[��
	const int MAX_INVICIBLE_COUNT = 30;		//���G�t���[��

	const float MAX_ENEMY_FORCE = 4.0f;		//�G�Ɠ����������ɐ�����ԗ́i10.0->9.0->8.0,,,,�j

	const float RESISTANCE_VALUE = 1.0f;	//Force�����Z����萔
	const float MINIMUM_FORCE = 2.0f;		//���l�ȉ��ɂȂ����狭���I��Force��0�ɂ���

	const float MAX_SPEED = 0.5f;			//���@�X�s�[�h

	XMFLOAT3 pos;			//�v���C���[�̍��W
	XMFLOAT3 vec3;			//�����Ă�������i���K���ρj
	XMFLOAT3 hitEnemypos;	//���������G�̍��W
	XMFLOAT3 hitBombpos;	//�����������������̔��e�̍��W
	XMFLOAT3 lastVec3;		//�Ō�Ɍ����Ă�������
	int activeCount;		//�s���s�\�J�E���g
	int invincibleCount;	//���G�J�E���g
	float bombForce;		//�{���̗͕ۑ��p
	float enemyForce;		//�G�̗͕ۑ��p
	bool isActive;			//�s���ł��邩�ǂ���
	bool isHitBomb;			//�{���ɓ������Ĕ�΂���Ă邩�ǂ���
	bool isHitEnemy;		//�G�ɓ������Ĕ�΂���Ă邩�ǂ���
	bool isInvincible;		//���G���ǂ���
	bool isShoot;			//�ˌ������ǂ����i�e�����邩�j
	bool isDetonating;		//�N���������ǂ���

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
	//������
	void Init();
	//�X�V
	void Update(bool isBombAlive);
	//�`��
	void Draw();

public:
	/*----------�����擾�p��----------*/

	XMFLOAT3 GetPos() { return pos; }						//�v���C���[�̍��W					���킩���
	XMFLOAT3 GetVec3() { return vec3; }						//�����Ă�������i���K���ρj			���킩���
	XMFLOAT3 GetLastVec3() { return lastVec3; }				//�Ō�Ɍ����Ă��������i���K���ρj	���킩���
	bool IsActive() { return isActive; }					//�s���ł��邩						���킩���
	bool IsInvincible() { return isInvincible; }			//���G���ǂ���						���킩���

	bool IsShootTrigger() { return isShoot; }				//�ˌ������u��						���킩���
	bool IsDetonatingTrigger() { return isDetonating; }		//�N�������u��						���킩���

	/*----------�����擾�p��----------*/


	void HitBomb(const float& BombForce, XMFLOAT3 bombPos);	//Hit���ɗ͓���ČĂ�ł�


private://�`�F�b�N��������Ȃ��ď����������Ă܂��I�I�I�I�I�J�X
	void CheakIsInput();								//���͂����邩
	void CheakIsActive();								//�ړ��\��Ԃ�
	void CheakShootTrigger(bool isBombAlive);			//�ˌ�������
	void CheakDetonatingTrigger(bool isBombAlive);		//�N��������
	void CheakHitEnemy();								//�G������������
	void CheakHitHole();								//���Ƃ̔���
	void CheakHitDeathLine();							//���Ƃ̔���

	void CalcActiveCount();								//�J�E���g�̌v�Z
	void AddBombForce();								//�{���Ɠ��������ۂɌv�Z
	void AddEnemyForce();								//�G�Ɠ��������ۂɌv�Z

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