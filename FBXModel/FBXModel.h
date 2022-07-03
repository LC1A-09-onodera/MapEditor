#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:26495)
#pragma warning(disable:26451)
#include <fbxsdk.h>
#include <DirectXTex.h>
#pragma warning(pop)

struct Node
{
	//名前
	std::string name;
	//スケール
	DirectX::XMVECTOR scaling = {1, 1, 1, 0};
	//回転角
	DirectX::XMVECTOR rotation = {0, 0, 0 ,0};
	//ローカル移動
	DirectX::XMVECTOR translation = {0, 0 ,0, 1};
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node *parent = nullptr;
};

struct Bone
{
	std::string name;
	DirectX::XMMATRIX invInitialPose;
	FbxCluster *fbxCuster;
	Bone(const std::string& name)
	{
		this->name = name;
	}
};

class FBXModel
{
public:
	static const int MAX_BONE_INDICES = 4;
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];//ボーン番号
		float boneWeight[MAX_BONE_INDICES];//ボーン重み
	};

	friend class FbxLoader;

	void CreateBuffers(ID3D12Device *dev);
	void Draw(ID3D12GraphicsCommandList *cmdList);
	std::vector<Bone>& GetBornes() { return bones; }
	const DirectX::XMMATRIX &GetModelTransform(){return meshNode->globalTransform;}
	FbxScene *GetFbxScene(){return fbxScene;}
	~FBXModel();
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	string name;
	vector<Node> nodes;
	std::shared_ptr<Node> meshNode;
	vector<VertexPosNormalUvSkin> vertices;
	vector<unsigned short> indices;
	XMFLOAT3 ambient = {1, 1, 1};
	XMFLOAT3 diffuse = {1, 1, 1};
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> texbuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	std::vector<Bone> bones;
	FbxScene *fbxScene = nullptr;
};