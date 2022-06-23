#include <sstream>
#include "MapLayout.h"
#include "../BaseDirectX/DX12operator.h"
#include "../BaseDirectX/Input.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderManager.h"
#include <fstream>

string MapObjectNames::wall = "wall";
string MapObjectNames::goal = "goal";
string MapObjectNames::enemy = "enemy";

list<MapEditorObject> MapEditorObjects::wall;
list<MapEditorObject> MapEditorObjects::goal;
list<MapEditorObject> MapEditorObjects::enemy;
list<list<MapEditorObject>::iterator> MapEditorObjects::wallDeleteItr;
list<list<MapEditorObject>::iterator> MapEditorObjects::goalDeleteItr;
list<list<MapEditorObject>::iterator> MapEditorObjects::enemyDeleteItr;

Model MapEditorObjects::wallModel;
Model MapEditorObjects::goalModel;
Model MapEditorObjects::enemyModel;
MapObjects MapEditorObjects::activeType;

bool MapEditorObject::OnCollisionMouse(float posX, float posY)
{
	XMFLOAT3 pos;
	pos.x = posX;
	pos.y = posY;
	pos.z = 0.0f;
	if (Lenght(piece.position, pos) < 0.95f)
	{
		return true;
	}
	return false;
}

void MapEditorObject::Init(XMFLOAT3& position)
{
	piece.ConstInit();
	piece.position = ConvertXMFLOAT3toXMVECTOR(position);
	piece.rotation.x = 90;
	isActive = true;
}

string MapEditorObject::PositionToString()
{
	ostringstream oss1;
	oss1 << -piece.position.m128_f32[0];
	ostringstream oss2;
	oss2 << piece.position.m128_f32[1];
	ostringstream oss3;
	oss3 << piece.position.m128_f32[2];
	string str;
	str = oss1.str() + " " + oss2.str() + " " + oss3.str();
	return str;
}

void MapEditorObjects::loadModels()
{
	wallModel.CreateModel("MapWall", ShaderManager::playerShader);
	goalModel.CreateModel("MapGoal", ShaderManager::playerShader);
	enemyModel.CreateModel("MapEnemy", ShaderManager::playerShader);
}

void MapEditorObjects::LoadFile(string path)
{
}

void MapEditorObjects::Update(XMFLOAT3& mousePos)
{
	if (Input::KeyTrigger(DIK_1))
	{
		activeType = MapObjects::WALL;
	}
	if (Input::KeyTrigger(DIK_2))
	{
		activeType = MapObjects::GOAL;
	}
	if (Input::KeyTrigger(DIK_3))
	{
		activeType = MapObjects::ENEMY;
	}
	if (Input::KeyTrigger(DIK_0))
	{
		activeType = MapObjects::NONE;
	}
	if (Input::MouseTrigger(MouseButton::LBUTTON))
	{
		if (ObjectCollision(mousePos))
		{
			EraseObject();
		}
		else
		{
			SetObject(mousePos);
		}
	}
	if (Input::Key(DIK_A) && Input::Key(DIK_L))
	{
		OutputFile();
	}
	if (Input::Key(DIK_D))
	{
		auto itr = wall.begin();
		itr->piece.position.m128_f32[0] += 1.0f;
	}
}

void MapEditorObjects::Draw()
{
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		wallModel.Update(&itr->piece);
		Draw3DObject(wallModel);
	}
	for (auto itr = goal.begin(); itr != goal.end(); ++itr)
	{
		goalModel.Update(&itr->piece);
		Draw3DObject(goalModel);
	}
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		enemyModel.Update(&itr->piece);
		Draw3DObject(enemyModel);
	}
}

void MapEditorObjects::SetObject(XMFLOAT3& position)
{
	if (position.x > 25.0f || position.x < -25.0f || position.y > 25.0f || position.y < -25.0f)
	{
		return;
	}
	//ƒXƒNƒŠ[ƒ“Œn
	POINT mouse = WindowsAPI::GetMousePos();
	XMFLOAT2 mouseFloat;
	mouseFloat = XMFLOAT2(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
	if (mouseFloat.x < 0.0f || mouseFloat.x > WindowsAPI::window_width || mouseFloat.y < 0.0f || mouseFloat.y > WindowsAPI::window_height)
	{
		return;
	}
	MapEditorObject object;
	object.Init(position);
	if (activeType == MapObjects::WALL)
	{
		MapEditorObjects::wall.push_back(object);
	}
	else if (activeType == MapObjects::GOAL)
	{
		MapEditorObjects::goal.push_back(object);
	}
	else if (activeType == MapObjects::ENEMY)
	{
		MapEditorObjects::enemy.push_back(object);
	}
}

bool MapEditorObjects::ObjectCollision(XMFLOAT3& mousePos)
{
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			wallDeleteItr.push_back(itr);
			return true;
		}
	}
	for (auto itr = goal.begin(); itr != goal.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			goalDeleteItr.push_back(itr);
			return true;
		}
	}
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			enemyDeleteItr.push_back(itr);
			return true;
		}
	}
	return false;
}

void MapEditorObjects::EraseObject()
{
	for (auto itr = wallDeleteItr.begin(); itr != wallDeleteItr.end(); ++itr)
	{
		wall.erase(*itr);
	}
	for (auto itr = goalDeleteItr.begin(); itr != goalDeleteItr.end(); ++itr)
	{
		goal.erase(*itr);
	}
	for (auto itr = enemyDeleteItr.begin(); itr != enemyDeleteItr.end(); ++itr)
	{
		enemy.erase(*itr);
	}
	wallDeleteItr.clear();
	goalDeleteItr.clear();
	enemyDeleteItr.clear();
}

void MapEditorObjects::OutputFile()
{
	string fileName = "test.txt";
	ofstream ofs(fileName);
	if (!ofs) return;
	//ofs << "wall" << std::endl;
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		ofs << "wall " + itr->PositionToString() << std::endl;
	}

	for (auto itr = goal.begin(); itr != goal.end(); ++itr)
	{
		ofs << "goal " + itr->PositionToString() << std::endl;
	}
	
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		ofs << "enemy " + itr->PositionToString() << std::endl;
	}
}

void MapEditorObjects::DeleteObjects()
{
	wall.clear();
	goal.clear();
	enemy.clear();
}
