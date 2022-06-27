#pragma once
#include <list>
#include <vector>
#include <string>
#include <array>
#include <DirectXMath.h>
#include "../3DModel/Model.h"

using namespace std;
using namespace DirectX;

enum class MapObjects
{
	NONE,
	WALL,
	GOAL,
	ENEMY,
};

//オブジェクトの名前たち
class MapObjectNames
{
	static string wall;
	static string goal;
	static string enemy;
};

//マップエディタ内で改変などを行うクラス
class MapEditorObject
{
public:
	
	EachInfo piece;
	bool isActive;
	bool OnCollisionMouse(float posX, float posY);
	void Init(XMFLOAT3 &position);
	string PositionToString();
};

//外部保存用クラス
class MapPreservation
{
	XMFLOAT3 position;

};

class MapEditorObjects
{
	static list<MapEditorObject> wall;
	static list<MapEditorObject> goal;
	static list<MapEditorObject> enemy;

	static Model wallModel;
	static Model goalModel;
	static Model enemyModel;

	static XMFLOAT3 lineMousePos;
	static bool isLinePut;

	
public:
	static MapObjects activeType;
	static void loadModels();
	static void LoadFile(string path);
	static void Update(XMFLOAT3 &mousePos);
	static void Draw();
	static void SetObject(XMFLOAT3 &position);
	static void SetObjectLine(XMFLOAT3& position);
	static bool ObjectCollision(XMFLOAT3& mousePos);
	static void EraseObject();
	static void OutputFile();
	static void DeleteObjects();
};
