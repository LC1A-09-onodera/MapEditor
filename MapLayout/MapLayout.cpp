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

Model MapEditorObjects::wallModel;
Model MapEditorObjects::goalModel;
Model MapEditorObjects::enemyModel;
MapObjects MapEditorObjects::activeType;

XMFLOAT3 MapEditorObjects::lineMousePos;
bool MapEditorObjects::isLinePut;
const int MapEditorObjects::MapW = 25;
const int MapEditorObjects::MapH = 25;
bool MapEditorObject::OnCollisionMouse(float posX, float posY)
{
	const float mapObjectR = 0.53f;
	XMFLOAT3 pos;
	pos.x = posX;
	pos.y = posY;
	pos.z = 0.0f;
	if (Lenght(piece.position, pos) < mapObjectR)
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
		isLinePut = true;
		if (!ObjectCollision(mousePos))
		{
			SetObject(mousePos);
		}
	}
	else if (!Input::Mouse(MouseButton::LBUTTON))
	{
		isLinePut = false;
	}
	if (!Input::MouseTrigger(MouseButton::LBUTTON) && Input::Mouse(MouseButton::LBUTTON) && isLinePut)
	{
		SetObjectLine(mousePos);
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
	if (position.x > MapW || position.x < -MapW || position.y > MapH || position.y < -MapH)
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
	lineMousePos = position;
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

void MapEditorObjects::SetObjectLine(XMFLOAT3& position)
{
	if (position.x > MapW || position.x < -MapW || position.y > MapH || position.y < -MapH)
	{
		return;
	}
	const float MapObjectR = 1.0f;
	if (lineMousePos.x + MapObjectR < position.x)
	{
		lineMousePos.x = lineMousePos.x + MapObjectR;
	}
	else if (lineMousePos.x - MapObjectR > position.x)
	{
		lineMousePos.x = lineMousePos.x - MapObjectR;
	}
	else if (lineMousePos.y + MapObjectR < position.y)
	{
		lineMousePos.y = lineMousePos.y + MapObjectR;
	}
	else if (lineMousePos.y - MapObjectR > position.y)
	{
		lineMousePos.y = lineMousePos.y - MapObjectR;
	}
	else
	{
		return;
	}
	SetObject(lineMousePos);
}

bool MapEditorObjects::ObjectCollision(XMFLOAT3& mousePos)
{
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			wall.erase(itr);
			lineMousePos = mousePos;
			return true;
		}
	}
	for (auto itr = goal.begin(); itr != goal.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			goal.erase(itr);
			lineMousePos = mousePos;
			return true;
		}
	}
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		if (itr->OnCollisionMouse(mousePos.x, mousePos.y))
		{
			enemy.erase(itr);
			lineMousePos = mousePos;
			return true;
		}
	}
	return false;
}

void MapEditorObjects::OutputFile()
{
	string fileName = "stage1.txt";
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
