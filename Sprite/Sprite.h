#pragma once
//#include <Windows.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../BaseDirectX/Input.h"
#include <wrl.h>
#include <d3dx12.h>
#include "../BaseDirectX/BaseDirectX.h"
#include "SpriteCommon.h"
#include "../Texture/Texture.h"
#include "../3DModel/Model.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	static SpriteCommon common;
	Tex tex;
	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> spriteRootSignature;
	ComPtr<ID3DBlob> rootSigBlob;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ���_�o�b�t�@�̐ݒ�
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�r���[�̃A�h���X��ۑ�
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���[�g�p�����[�^�[
	CD3DX12_ROOT_PARAMETER rootParams[2];
	bool back;
	float rotation = 0.0f;
	int texNum;
	//�ˉe�s��
	XMVECTOR position{};
	XMMATRIX matProjection{};
	XMMATRIX matWorld{};
	XMFLOAT2 texLeftTop = {0,0};
	XMFLOAT2 texSize = {100,100};

	//�X�v���C�g�̐���
	void CreateSprite(const wchar_t* graph, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff = nullptr, bool back = false, bool TexSize = true);
	void CreateSprite(Tex tex, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff = nullptr, bool back = false, bool TexSize = true);
	//�X�v���C�g�p�p�C�v���C���̐ݒ�R�}���h
	
	//�X�v���C�g�p�`��R�}���h
	//�`�斈�t���[������
	void SpriteDraw();
	//���W�ύX
	void Move();
	//�\���T�C�Y�̕ύX
	void ChangeSize(float wid,float hei);
	void ChangeSizeOther(float wid, float hei);
	
	//���t���[������
	private:
	void SpritePipelineEdit();
};