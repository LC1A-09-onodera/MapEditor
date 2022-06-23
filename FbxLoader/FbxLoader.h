#pragma once
#include <fbxsdk.h>
#include <string>
#include <d3d12.h>
#include <d3dx12.h>
#include "../FBXModel/FBXModel.h"

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	void Initialize(ID3D12Device* dev);
	void Finalize();
	FBXModel* LoadModelFromFile(const std::string& modelName);
	void ParseNodeRecursive(FBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);
	//メッシュ読み込み
	void ParsMesh(FBXModel* model, FbxNode* fbxNode);
	//頂点情報の取得
	void ParseMeshVertices(FBXModel* model, FbxMesh* fbxMesh);
	//ポリゴンの作成
	void ParseMeshFaces(FBXModel* model, FbxMesh* fbxMesh);
	//マテリアル読み込み
	void ParseMaterial(FBXModel* model, FbxNode* fbxNode);
	void LoadTexture(FBXModel* model, const std::string& fullPath);
	std::string ExtractFileName(const std::string& path);

	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);
	//スキン読み込み
	void ParseSkin(FBXModel* model, FbxMesh* fbxMesh);
	static const std::string baseDirectory;
private:
	using string = std::string;
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	ID3D12Device* dev = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

	static const string defaultTextureFileName;
};
