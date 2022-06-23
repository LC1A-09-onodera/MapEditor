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

//������
void Player::Init()
{
	player.CreateModel("Block", ShaderManager::playerShader);
	//water.CreateWater();

	pos = { 0,0,0 };			//�v���C���[�̍��W
	vec3 = { 0,0,0 };			//�����Ă�������i���K���ρj
	hitEnemypos = { 0,0,0 };	//���������G�̍��W
	hitBombpos = { 0,0,0 };		//�����������������̔��e�̍��W
	lastVec3 = { 0,0,1 };		//�Ō�Ɍ����Ă�������
	activeCount = 0;			//�s���s�\�J�E���g
	invincibleCount = 0;		//���G�J�E���g
	bombForce = 0;				//�{���̗͕ۑ��p
	enemyForce = 0;				//�G�̗͕ۑ��p
	isActive = true;			//�s���ł��邩�ǂ���
	isHitBomb = false;			//�{���ɓ������Ĕ�΂���Ă邩�ǂ���
	isHitEnemy = false;			//�G�ɓ������Ĕ�΂���Ă邩�ǂ���
	isInvincible = false;		//���G���ǂ���
	isShoot = false;			//�ˌ������ǂ����i�e�����邩�j
	isDetonating = false;		//�N���������ǂ���
}

//�X�V
void Player::Update(bool isBombAlive)
{
	/*�ړ�*/
	CheakIsInput();
	//�s���\���������łȂ����
	CheakIsActive();

	/*���˃g���K�[*/
	CheakShootTrigger(isBombAlive);

	/*�N���g���K�[*/
	CheakDetonatingTrigger(isBombAlive);

	/*���@���G�ɓ����������̔���*/
	CheakHitEnemy();

	/*���@�����ɓ����������̔���*/
	CheakHitHole();

	/*���@������̐��ɓ����������̔���*/
	CheakHitDeathLine();

	/*�s���s�\�Ƃ����G�Ƃ�*/
	CalcActiveCount();

	/*�{���̗͌v�Z*/
	AddBombForce();

	/*�G�̗͌v�Z*/
	AddEnemyForce();

	player.each.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//�`��
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
		//�{���̗͂���
		bombForce = BombForce;

		hitBombpos = bombPos;

		bombForce *= 3.0f;

		//�ړ�����
		isActive = false;

		//������ђ��ɂ���
		isHitBomb = true;
	}
}

void Player::CheakIsInput()
{
	/*�ړ�*/
	//���͂Ȃ���΃��Z�b�g
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
	//�s���\���������łȂ����
	if (isActive && !isHitBomb)
	{
		pos.x += vec3.x * MAX_SPEED;
		pos.z += -vec3.z * MAX_SPEED;
	}
}

void Player::CheakShootTrigger(bool isBombAlive)
{
	/*���˃g���K�[*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_B))
	{
		if (!isBombAlive && isActive) { isShoot = true; }
	}
	else { isShoot = false; }
}

void Player::CheakDetonatingTrigger(bool isBombAlive)
{
	/*�N���g���K�[*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_B))
	{
		if (isBombAlive && isActive) { isDetonating = true; }
	}
	else { isDetonating = false; }
}

void Player::CheakHitEnemy()
{
	//�s���s�\�������G����������ђ��͔��薳��
	if (!isActive || isInvincible || isHitBomb) { return; }

	bool isHit = false;
	auto itr = Enemys::enemys.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		XMFLOAT3 enemyPos = itr->GetPosition();
		if (std::isnan(enemyPos.x)) { return; }
		if (std::isnan(enemyPos.z)) { return; }

		//2�_�Ԃ̋����Ɣ���i�~�j
		isHit = CheakHit(1.2f, 1.2f, pos, enemyPos);

		//�������ĂȂ��������蒼��
		if (!isHit) { continue; }

		//����������s���s�\�ɂ���
		hitEnemypos = itr->GetPosition();
		isActive = false;
		break;
	}

	//�N�Ƃ�������Ȃ������ꍇ�A�I��
	if (isActive) { return; }

	enemyForce = MAX_ENEMY_FORCE;
}

void Player::CheakHitHole()
{
	//���G���͔��薳��
	if (isInvincible) { return; }

	bool isHit = false;
	auto itr = Holes::holes.begin();
	for (; itr != Holes::holes.end(); ++itr)
	{
		XMFLOAT3 holePos = itr->GetPosition();
		if (std::isnan(holePos.x)) { return; }
		if (std::isnan(holePos.z)) { return; }

		//2�_�Ԃ̋����Ɣ���i�~�j
		isHit = CheakHit(1.2f, 1.2f, pos, holePos);

		//�������ĂȂ��������蒼��
		if (!isHit) { continue; }

		//����������^�񒆂Ń��X�|�[�����čs���s�\��
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
	/*�s���s�\�Ƃ����G�Ƃ�*/
	//���G���s���\��
	if (isInvincible && isActive)
	{
		if (invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { invincibleCount = 0; isInvincible = false; }
	}

	//�s���s�\��
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
	//������ђ������v�Z
	if (!isHitBomb) { return; }

	//�ړ�
	//�ړ�
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitBombpos.x;
	move.z = pos.z - hitBombpos.z;
	VecNormaliz(move);
	pos.x += bombForce * move.x;
	pos.z += bombForce * move.z;

	//���Z
	bombForce -= RESISTANCE_VALUE;

	//���ʂ����������0�ɂ��Ĉړ��ĊJ
	if (bombForce < MINIMUM_FORCE)
	{
		bombForce = 0;
		isActive = true;
		isHitBomb = false;
	}
}

void Player::AddEnemyForce()
{
	//�s���s�\�������v�Z
	if (isActive) { return; }

	//�ړ�
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitEnemypos.x;
	move.z = pos.z - hitEnemypos.z;
	VecNormaliz(move);
	pos.x += enemyForce * move.x;
	pos.z += enemyForce * move.z;

	//���Z
	enemyForce -= RESISTANCE_VALUE;

	//���ʂ����������0�ɂ���
	if (enemyForce < MINIMUM_FORCE) { enemyForce = 0; }
}
