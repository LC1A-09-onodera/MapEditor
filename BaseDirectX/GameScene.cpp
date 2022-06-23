#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../imgui/ImguiControl.h"
#include "vec3.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../Sound/Sound.h"
#include "../MapLayout/MapLayout.h"

GameScene::GameScene()
{
	SceneNum = TITLE;
}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
	
}

void GameScene::SceneManageUpdateAndDraw()
{
	Input::Update();
	WindowsAPI::CheckMsg();
	light->Update();
	switch (SceneNum)
	{
	case TITLE:
		TitleUpdate();
		TitleDraw();
		break;
	case SELECT:
		SelectUpdate();
		SelectDraw();
		break;
	case GAME:
		GameUpdate();
		GameDraw();
		break;
	case RESULT:
		ResultUpdate();
		ResultDraw();
		break;
	case END:
		EndUpdate();
		EndDraw();
		break;
	default:
		break;
	}
}

void GameScene::Init()
{
	BaseDirectX::Set();
	//�T�E���h
	Sound::CreateVoice();
	//SRV�̃A�h���X�m��
	BaseDirectX::GetAdress();
	//�J����������
	Cameras::camera.Init();
	Cameras::camera.eye = { 0, 0, 20.0f };
	Cameras::camera.target = { 0, 0, 0 };
	Cameras::camera.up = { 0.0f, 1.0f, 0.0f };
	Cameras::camera.Update();
	Cameras::rCamera.Init();
	Cameras::rCamera.eye = { 0, 0, 20.0f };
	Cameras::rCamera.target = { 0, 0, 0 };
	Cameras::rCamera.up = { 0.0f, 1.0f, 0.0f };
	Cameras::rCamera.Update();
	//Imgui�̏�����
	Imgui::Init();
	//���C�g�̏�����
	Light::StaticInitialize(BaseDirectX::dev.Get());
	ShaderManager::LoadShaders();
	//�C���v�b�g������
	Input::KeySet(WindowsAPI::w, WindowsAPI::hwnd);
	//FBX�n
	FbxLoader::GetInstance()->Initialize(BaseDirectX::dev.Get());
	FBXObject::SetDevice(BaseDirectX::dev.Get());
	FBXObject::CreateGraphicsPipeline();
	//���C�g������
	light.reset(Light::Create());
	//���f�����ׂĂɃ��C�g��K�p
	Model::SetLight(light.get());
	//�|�X�g�G�t�F�N�g�̏�����
	PostEffects::Init();
	MapEditorObjects::loadModels();
	sample.CreateModel("maru", ShaderManager::playerShader);
	sample.each.scale = {0.1f, 0.1f, 0.1f};
	mapFrameV.CreateModel("Map", ShaderManager::playerShader, false, false);
	mapFrameH.CreateModel("Map", ShaderManager::playerShader, false, false);
	mapFrameV.each.rotation.x = 90.0f;
	mapFrameH.each.rotation.x = 0.0f;
	mapFrameH.each.rotation.y = -90.0f;
	mapFrameH.each.rotation.z = -90.0f;
	player.CreateModel("maru", ShaderManager::playerShader);
	player.each.scale = { 0.5f, 0.5f, 0.5f };
}

void GameScene::TitleUpdate()
{
	XMFLOAT3 mousePos = Cameras::camera.MousePosition(0.0f);
	sample.each.position = ConvertXMFLOAT3toXMVECTOR(mousePos);
	sample.each.position.m128_f32[0] -= Cameras::camera.mouseMoveAmount[0];
	sample.each.position.m128_f32[1] -= Cameras::camera.mouseMoveAmount[1];
	sample.Update();
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	mapFrameV.Update();
	mapFrameH.Update();
	MapEditorObjects::Update(XMFLOAT3(mousePos.x - Cameras::camera.mouseMoveAmount[0], mousePos.y - Cameras::camera.mouseMoveAmount[1], mousePos.z));
	LightUpdate();
	Cameras::camera.MouseWheelY();
	Cameras::camera.MouseRightPushMove();
	Cameras::camera.Update();
}

void GameScene::SelectUpdate()
{

}

void GameScene::GameUpdate()
{

}

void GameScene::ResultUpdate()
{

}

void GameScene::EndUpdate()
{

}

void GameScene::TitleDraw()
{
	BaseDirectX::UpdateFront();

	Draw3DObject(sample);
	Draw3DObject(mapFrameV, false);
	Draw3DObject(mapFrameH, false);
	player.Update();
	Draw3DObject(player);
	MapEditorObjects::Draw();
	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{

	BaseDirectX::UpdateFront();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
{

	BaseDirectX::UpdateFront();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	BaseDirectX::UpdateFront();

	//PostEffect��Draw
	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::EndDraw()
{
	BaseDirectX::UpdateFront();

	Imgui::DrawImGui();
	//�`��R�}���h�����܂�
	BaseDirectX::UpdateBack();
}

void GameScene::LightUpdate()
{
	light->SetPointLightActive(0, false);
	light->SetSpotLightActive(0, false);
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	light->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	light->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	light->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	light->SetSpotLightAngle(0, XMFLOAT2(spotLightAngle));
}
