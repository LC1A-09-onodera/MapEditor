#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include "Seling.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
#include <algorithm>

void Seling::ForceUpdate()
{
	if (!isMoveForce) return;

	subForce.x = -(addForce.x / 50);
	subForce.y = -(addForce.y / 50);
	subForce.z = -(addForce.z / 50);

	addForce.x += subForce.x;
	addForce.y += subForce.y;
	addForce.z += subForce.z;

	if (Lenght(addForce) < 0.05f)
	{
		isMoveForce = false;
	}
}

void Seling::LoadModel()
{
	seling.CreateModel("seling", ShaderManager::playerShader, false);
	shotModel.CreateModel("maru", ShaderManager::playerShader, false);
	shieldModel.CreateModel("shield", ShaderManager::shieldShader, false);
	shieldModel.each.scale = { 2.0f, 2.0f, 1.0f };
}

void Seling::Init()
{
	subForce = { 0.01f, 0.01f, 0.01f };
	maxForce = { 0.7f, 0.7f, 0.7f };
	frontDirection = { 0, 0 ,1.0f };
	isShield = false;
	enemy.Init(XMFLOAT3(0, 0, 10));

	playerShieldKey.KeyBoradInit(5, KeyCode::A, KeyCode::B, KeyCode::C, KeyCode::Space);
	playerShieldKey.PadInit(2, PadKeyCode::Button01, PadKeyCode::ButtonRB);
	
}

void Seling::Update()
{
	Move();

	ForceUpdate();

	ForceAttach();

	//ShotInitAndUpdate();

	//ShieldInitAndUpdate();

	enemy.Update(shieldPos, isShield);
}

void Seling::Draw(bool isRCamera)
{
	if (isRCamera)
	{
		seling.Update(&seling.each, isRCamera);
		Draw3DObject(seling);
	}
	else if (!isRCamera)
	{
		seling.Update(&seling.each, isRCamera);
		Draw3DObject(seling);
	}

	if (isShot)
	{
		shotModel.each.position = ConvertXMFLOAT3toXMVECTOR(shotPos);
		shotModel.Update(&shotModel.each, isRCamera);
		Draw3DObject(shotModel);
	}

	if (isShield)
	{
		shieldModel.each.position = ConvertXMFLOAT3toXMVECTOR(shieldPos);
		shieldModel.Update(&shieldModel.each, isRCamera);
		Draw3DObject(shieldModel);
	}
	//enemy.Draw();
}

void Seling::ForceAttach()
{
	seling.each.position.m128_f32[0] += addForce.x;
	seling.each.position.m128_f32[1] += addForce.y;
	seling.each.position.m128_f32[2] += addForce.z;
}

void Seling::Move()
{
	if (VoiceReciver::GetRight() || Input::KeyTrigger(DIK_D))
	{
		angle += addShieldRotaion;
	}
	if (VoiceReciver::GetLeft() || Input::KeyTrigger(DIK_A))
	{
		angle -= addShieldRotaion;
	}
	frontDirection = { ShlomonMath::Sin(angle), 0, ShlomonMath::Cos(angle) };

	if (VoiceReciver::GetFront() || Input::Key(DIK_W))
	{
		AddForce(XMFLOAT3(frontDirection.x * addForcePower, frontDirection.y * addForcePower, frontDirection.z * addForcePower));
	}
	if (VoiceReciver::GetBack() || Input::Key(DIK_S))
	{
		AddForce(XMFLOAT3(frontDirection.x * -addForcePower, frontDirection.y * -addForcePower, frontDirection.z * -addForcePower));
	}
	VoiceReciver::SetRight(false);
	VoiceReciver::SetLeft(false);
	VoiceReciver::SetFront(false);
	VoiceReciver::SetBack(false);
}

void Seling::ShotInit()
{
	shotPos = ConvertXMVECTORtoXMFLOAT3(seling.each.position);
	isShot = true;
}

void Seling::ShotUpdate()
{
	shotPos.z += 0.1f;
}

void Seling::ShotInitAndUpdate()
{
	if (Input::Key(DIK_R))
	{
		ShotInit();
	}
	if (VoiceReciver::GetIsShot())
	{
		ShotInit();
		VoiceReciver::SetIsShot(false);
	}
	if (isShot)
	{
		ShotUpdate();
	}
}

void Seling::ShieldInit()
{
	isShield = true;
	shieldTime = shieldMaxTime;
	shieldModel.each.scale = { 0.01f, 0.01f, 0.01f };
	scaleStart = { 0.02f, 0.02f, 0.01f };
	scaleEnd = { 2.0f, 2.0f, 1.0f };
	easeTime = 0.0f;
}

void Seling::ShieldUpdate()
{
	XMFLOAT3 pos;
	pos = ConvertXMVECTORtoXMFLOAT3(seling.each.position);
	shieldModel.each.rotation.y = angle + 180;
	XMFLOAT3 addPos;
	addPos = { frontDirection.x * shieldR, 0, frontDirection.z * shieldR };
	pos = pos + addPos;
	shieldPos = pos;
	shieldTime--;
	easeTime += 0.05f;
	if (easeTime <= 1.0f)
	{
		shieldModel.each.scale = ShlomonMath::EaseInQuad(scaleStart, scaleEnd, easeTime);
	}
	else
	{
		shieldModel.each.scale = ShlomonMath::EaseInQuad(scaleStart, scaleEnd, 1.0f);
	}
	if (shieldTime <= 0)
	{
		isShield = false;
	}
}

void Seling::ShieldInitAndUpdate()
{
	if (playerShieldKey.GetKeyDown())
	{
		ShieldInit();
	}
	if (Input::Key(DIK_R))
	{
		ShieldInit();
	}
	if (VoiceReciver::GetWall())
	{
		ShieldInit();
		VoiceReciver::SetWall(false);
	}
	if (isShield)
	{
		ShieldUpdate();
	}
}

void ShieldModel::CreateModel(const char* name, HLSLShader& shader, bool smoothing)
{
	InitializeDescriptorHeap();
	InitializeGraphicsPipeline(shader);
	ifstream file;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resource/Model/" + modelname + "/";
	file.open(directoryPath + filename);

	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> pos;
	vector<XMFLOAT3> normal;
	vector<XMFLOAT2> uv;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{   //マテリアル
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}
		if (key == "v")
		{   //座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			pos.emplace_back(position);//追加
		}
		if (key == "vt")
		{
			XMFLOAT2 texcood{};//UV受け取る
			line_stream >> texcood.x;
			line_stream >> texcood.y;

			texcood.y = 1.0f - texcood.y;//ｖを反転
			uv.emplace_back(texcood);

		}
		if (key == "vn")
		{   //法線
			XMFLOAT3 normals{};
			line_stream >> normals.x;
			line_stream >> normals.y;
			line_stream >> normals.z;
			normal.emplace_back(normals);
		}
		if (key == "f")
		{   //インデックス
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//count += 1;
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcood;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcood;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex verte{};
				verte.pos = pos[indexPosition - 1];
				verte.normal = normal[indexNormal - 1];
				verte.uv = uv[indexTexcood - 1];

				mesh.vertices.emplace_back(verte);
				if (smoothing)
				{
					AddAmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
				}
				mesh.indices.emplace_back((unsigned short)mesh.indices.size());
			}
		}
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	};
	Init(1);
	file.close();
}

bool ShieldModel::InitializeGraphicsPipeline(HLSLShader& shader)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	if (shader.vsBlob == nullptr)
	{
		assert(0);
	}
	if (shader.psBlob == nullptr)
	{
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(shader.vsBlob.Get());
	/*if (shader.hsBlob != nullptr)
	{
		gpipeline.HS = CD3DX12_SHADER_BYTECODE(shader.hsBlob.Get());
	}
	if (shader.dsBlob != nullptr)
	{
		gpipeline.DS = CD3DX12_SHADER_BYTECODE(shader.dsBlob.Get());
	}*/
	if (shader.gsBlob != nullptr)
	{
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(shader.gsBlob.Get());
	}
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(shader.psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	//CD3DX12_ROOT_PARAMETER rootparams[3];
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = BaseDirectX::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成-------------------------
	result = BaseDirectX::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}
