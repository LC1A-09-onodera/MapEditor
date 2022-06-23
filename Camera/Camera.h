#include <DirectXMath.h>
#include <array>
#include "../BaseDirectX/vec3.h"
#include "../BaseDirectX/DX12operator.h"
using namespace DirectX;

class Camera
{
public:
	//�ʒu
	Vec3 eye;
	Vec3 eyePos;
	Vec3 eyeStartPosition;
	Vec3 eyeGoalPosition;
	float shakePower;
	bool isShake;
	//�ǂ��𒍎��_�Ƃ��邩
	Vec3 target;
	Vec3 targetStartPosition;
	Vec3 targetGoalPosition;
	//�J�����̌����H
	Vec3 up;
	Vec3 cameraUp;
	//�r���[�s��̍쐬
	XMMATRIX matView;
	//�r���{�[�h
	XMMATRIX matBillboard;
	//Y���r���{�[�h
	XMMATRIX matBillboardY;

	XMMATRIX rotM;
	XMVECTOR vv0;

	bool isControl = true;
	float targetR = 10.0f;
	float rotationXZ = 0.0f;
	XMFLOAT3 rotation = {0, 0.2f, -1.0f};
	void CameraTargetRot();

	float cameraRightAngle;
	float cameraUpAngle;


	float rotX;
	float rotY;
	float rotZ;

	float cameraAngleHorizonal;
	float cameraAngleVertical;

	std::array<float, 2> mouseClickPos = {0, 0};
	std::array<float, 2> mouseMoveAmount = {0, 0};
	void Init();
	void Update();
	void EyeMove(Vec3 moveAmount);
	void EyeAndTargetMove(XMFLOAT3 moveAmount);
	void TargetMove(XMFLOAT3 moveAmount);
	float TargetLength();
	void eyeMoveCircleHorizonal(XMFLOAT3 Amount);
	void eyeMoveCircleVertical(XMFLOAT3 Amount);
	void SetTarget(const XMFLOAT3& target);
	void QuaternionRotation(const float &RightAngle = NULL, const float &UpAngle = NULL);
	void ShakeUpdate();
	void SetShake(float shakePower);
	void SetTarget(XMFLOAT3 target);
	void MouseWheelY();
	void MouseRightPushMove();
	XMFLOAT3 GetTargetDirection();
	XMFLOAT3 GetMousePosition();
	XMFLOAT3 EyeToMouseVec();
	XMFLOAT3 MousePosition(float z);
};

class Cameras
{
public:
	static Camera camera;
	static Camera rCamera;
};