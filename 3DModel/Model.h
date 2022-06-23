#pragma once
#include "../BaseDirectX/BaseDirectX.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Light/Light.h"
#include "../Shader/Shader.h"
#include "../Collition/CollisionInfo.h"

using namespace std;
class BaseCollider;

struct ConstBufferData
{
	XMMATRIX mat;
	XMFLOAT4 weight0;
	XMFLOAT4 weight1;
	UINT frameTime;
};

struct ConstBufferDataSP
{
	XMFLOAT4 color;
	XMMATRIX mat;
};

class ConstBufferDataB0
{
public:
	XMMATRIX viewproj;//�r���[�v���W�F�N�V�����s��
	XMMATRIX world;//���[���h�s��
	XMFLOAT3 cameraPos;//�J�������W
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

struct ConstBufferDataB3
{
public:
	XMMATRIX rViewproj;//�r���[�v���W�F�N�V�����s��
	XMMATRIX rWorld;//���[���h�s��
	XMFLOAT3 rCameraPos;//�J�������W
};

struct Material
{
	string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;

	float alpha;
	string texFilename;

	Material()
	{
		ambient = {0.3f, 0.3f, 0.3f};
		diffuse = {0.0f, 0.0f, 0.0f};
		specular = {0.0f, 0.0f, 0.0f};
		alpha = 1.0;
	}
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

class BaseObject
{
	Vertex *vert;
	unsigned short *indices;
};

class Mesh
{
public:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// ���_�f�[�^�z��
	std::vector<Vertex> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
};
class EachInfo
{
public:
	ComPtr<ID3D12Resource> constBuff0;
	ComPtr<ID3D12Resource> constBuff1;
	ComPtr<ID3D12Resource> constBuff2;
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMVECTOR position = { 0,0,0 };
	void CreateConstBuff0();
	void CreateConstBuff1();
	void CreateConstBuff2();
	void ConstInit();
};
class Model
{
public:
	Model() = default;

	virtual ~Model();
	//virtual bool Initialize();

	static void SetLight(Light *light);
	Mesh mesh;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	EachInfo each;
	XMMATRIX matWorld;
	//Object3d *parent = nullptr;
	static Light *light;
	//const char *name = nullptr;
	BaseCollider *collider = nullptr;
	//----��{�������-----------
	float radi = 2.0f;
	bool billboard = false;
	//-----------------------
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//�}�e���A��
	Material material;
	//�}�e���A���̐�
	int materialCount = 0;

	void Init(int index);
	void CreateModel(const char *name, HLSLShader &shader, bool smoothing = false, bool triangle = true);
	//void Update();
	virtual void Update(EachInfo *each = nullptr, bool rCamera = false);
	//�X���[�W���O
	unordered_map<unsigned short, vector<unsigned short>> smoothData;
	inline size_t GetVertexCount();
	//�G�b�W�������f�[�^�̒ǉ�
	void AddAmoothData(unsigned short indexPosition, unsigned short indexVertex);
	void CalculateSmoothedVertexNormals();
	bool InitializeGraphicsPipeline(HLSLShader &shader, bool triangle = true);
	//bool LoadTexture(const wchar_t *texName = nullptr);
	bool LoadTexture(const string &directPath, const string &filename);
	bool InitializeDescriptorHeap();
	void LoadMaterial(const string &directoryPath, const string &filename);
	ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "", ComPtr<ID3DBlob> sBlob = nullptr /*, LPCWSTR PshaderName = L"", LPCSTR Ptarget = "", ComPtr<ID3DBlob> psBlob = nullptr, LPCWSTR GshaderName = L"", LPCSTR Gtarget = "", ComPtr<ID3DBlob> gsBlob = nullptr*/);
	const XMMATRIX &GetMatWorld() { return matWorld; }
	void SetCollider(BaseCollider *collider);
	virtual void OnCollision(const CollisionInfo &info){}
	inline const std::vector<Vertex> &GetVertices(){return mesh.vertices;}
	inline const std::vector<unsigned short> &GetIndices(){return mesh.indices;}
};
void Set3DDraw(const Model &model, bool triangle = true);
void Draw3DObject(const Model &model, bool triangle = true);
bool ObjectColition(Model& object1, Model& object2);
bool CiycleColition(const XMFLOAT3 &object1, const XMFLOAT3 &object2 , float radi1, float radi2);