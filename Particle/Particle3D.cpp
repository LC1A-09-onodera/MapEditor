#include "Particle3D.h"
#include "../BaseDirectX/viewport.h"
#include "../Camera/Camera.h"
#include "../DX12operator.h"
#include "../BaseDirectX/Library.h"

//const XMFLOAT3 operator+(const XMFLOAT3 &lhs, const XMFLOAT3 &rhs)
//{
//	XMFLOAT3 result;
//	result.x = lhs.x + rhs.x;
//	result.y = lhs.y + rhs.y;
//	result.z = lhs.z + rhs.z;
//	return result;
//}

/// <summary>
/// 静的メンバ変数の実体
/// </summary>

ID3D12Device *ParticleManager::device = nullptr;
//UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList *ParticleManager::cmdList = nullptr;
XMMATRIX ParticleManager::matView{};
XMMATRIX ParticleManager::matProjection{};
XMFLOAT3 ParticleManager::eye = { 0, 0, 0.0f };
XMFLOAT3 ParticleManager::target = { 0, 20, 0 };
XMFLOAT3 ParticleManager::up = { 0, 1, 0 };
//ビルボード
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();

std::shared_ptr<ParticleIndi> ParticleControl::attackEffect = nullptr;
std::shared_ptr<ParticleIndi> ParticleControl::expEffect = nullptr;
std::shared_ptr<ParticleIndi> ParticleControl::flashEffect = nullptr;
std::shared_ptr<ParticleIndi>  ParticleControl::rockOnEffect = nullptr;
std::shared_ptr<ParticleIndi> ParticleControl::numbers[10];

bool ParticleManager::StaticInitialize(ID3D12Device *device,  int window_width, int window_height,XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	// nullptrチェック
	assert(device);

	ParticleManager::device = device;

	// デスクリプタヒープの初期化
	//InitializeDescriptorHeap();

	// カメラ初期化
	InitializeCamera(window_width, window_height,eye, target, up);

	// パイプライン初期化
	//InitializeGraphicsPipeline();

	// テクスチャ読み込み
	//LoadTexture(particleManager,texName);

	// モデル生成
	//CreateModel();

	return true;
}
void PreDraw(ID3D12GraphicsCommandList *cmdList, const ParticleIndi *particle)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList == nullptr);

	// コマンドリストをセット
	ParticleManager::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(particle->pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(particle->rootsignature.Get());
	// プリミティブ形状を設定
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

}
void ParticleDraw(ID3D12GraphicsCommandList *cmdList, const ParticleIndi *particle)
{
	PreDraw(cmdList, particle);
	// nullptrチェック
	assert(ParticleManager::device);
	assert(ParticleManager::cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &particle->vbView);
	// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap *ppHeaps[] = { particle->descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, particle->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, particle->gpuDescHandleSRV);
	// 描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
	cmdList->DrawInstanced((UINT)std::distance(particle->particles.begin(), particle->particles.end()), 1, 0, 0);
	PostDraw();
}
void PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList = nullptr;
}
void ParticleManager::SetEye(XMFLOAT3 eye)
{
	ParticleManager::eye = eye;

	//UpdateViewMatrix();
}
void ParticleManager::SetTarget(XMFLOAT3 target)
{
	ParticleManager::target = target;

	//UpdateViewMatrix();
}
void ParticleManager::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}
void ParticleManager::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;
	SetEye(eye_moved);
}
void ParticleManager::InitializeCamera(int window_width, int window_height,XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	// ビュー行列の生成
	UpdateViewMatrix(eye, target, up, false);

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}
void ParticleManager::CameraUpdate()
{
	//ビュー変換行列を作り直す
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);
	//z軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//x軸
	XMVECTOR cameraAxisX;
	//外積
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//y軸
	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	matView = XMMatrixTranspose(matCameraRot);
	//平行移動の逆
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;
}
void ParticleManager::UpdateViewMatrix(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up , bool isBillbord)
{
	//ビュー変換行列を作り直す
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);
	//z軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//x軸
	XMVECTOR cameraAxisX;
	//外積
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//y軸
	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	matView = XMMatrixTranspose(matCameraRot);
	//平行移動の逆
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;
	//ビルボードの計算
	if (!isBillbord)return;
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	//y軸ビルボード
	XMVECTOR yBillCameraAxisX;
	XMVECTOR yBillCameraAxisY;
	XMVECTOR yBillCameraAxisZ;
	yBillCameraAxisX = cameraAxisX;
	yBillCameraAxisY = XMVector3Normalize(upVector);
	yBillCameraAxisZ = XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);
	matBillboardY.r[0] = yBillCameraAxisX;
	matBillboardY.r[1] = yBillCameraAxisY;
	matBillboardY.r[2] = yBillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
	
}
ParticleIndi *ParticleIndi::Create(const wchar_t *texName)
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleIndi *particleMan = new ParticleIndi();
	if (particleMan == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!particleMan->Initialize(texName)) {
		delete particleMan;
		assert(0);
		return nullptr;
	}

	return particleMan;
}
void ParticleIndi::CreateModel()
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = ParticleManager::device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	VertexPos *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}
bool ParticleIndi::InitializeDescriptorHeap()
{
	
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = ParticleManager::device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = ParticleManager::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}
bool ParticleIndi::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/HLSL/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = D3DCompileFromFile(
		L"Resource/HLSL/ParticleGS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/HLSL/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	//半透明合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	//減算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

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
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = ParticleManager::device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = ParticleManager::device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}

	return true;
}
bool ParticleIndi::Initialize(const wchar_t *texName)
{
	//デスクリプタ
	InitializeDescriptorHeap();
	//グラフィックスパイプライン生成
	InitializeGraphicsPipeline();
	//テクスチャ
	LoadTexture(texName);

	CreateModel();
	// nullptrチェック
	assert(ParticleManager::device);

	HRESULT result;
	// 定数バッファの生成
	result = ParticleManager::device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	return true;
	
}
void ParticleIndi::Update(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up, XMFLOAT3 *pos, bool isBilbord)
{
	HRESULT result;
	
	//パーティクルの消滅
	particles.remove_if(
		[](Particle &x)
		{
			return x.frame >= x.num_frame;
		}
	);
	//全パーティクルの更新
	for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		//フレームの増加
		it->frame++;
		//速度に加速度を追加
		it->velocity = it->velocity + it->accel;
		if (pos != nullptr)
		{
			it->position.z = pos->z;
		}
		//移動
		it->position = it->position + it->velocity;
		//スケールの変更
		float f = (float)it->num_frame / it->frame;
		//スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
	}
	
	ParticleManager::UpdateViewMatrix(eye, target, up, isBilbord);

	//頂点バッファへデータ転送
	VertexPos *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			vertMap->pos = it->position;
			vertMap->scale = it->scale;
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	//constMap->color = color;
	constMap->mat = ParticleManager::matView * ParticleManager::matProjection;	// 行列の合成
	constMap->matBillboard = ParticleManager::matBillboard;
	constMap->alpha = this->alpha;
	constBuff->Unmap(0, nullptr);
	
}
bool ParticleIndi::LoadTexture(const wchar_t *texName)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		texName, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}

	const Image *img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = ParticleManager::device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		assert(0);
	}

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		assert(0);
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	ParticleManager::device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);

	return true;
}
void ParticleIndi::Add(int life, XMFLOAT3 &position, XMFLOAT3 &velocity, XMFLOAT3 &accel, float start_scale, float end_scale)
{
	//要素追加
	particles.emplace_front();
	//追加した要素
	Particle &p = particles.front();
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.scale = p.s_scale;
}
void ParticleIndi::StartParticle( const XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 10.0f;
		//{-5.0f~+5.0f}で出現
		XMFLOAT3 pos{};//初期値
		/*pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;*/
		/*pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;*/
		pos = emitterPosition;
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};//初速
		/*vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;*/
		vel.x = static_cast<float>(rand() % 10);
		vel.x = static_cast<float>(-vel.x / 100.0f * 1.4f);
		vel.y = static_cast<float>(rand() % 15);
		vel.y = static_cast<float>(vel.y / 150.0f * 1.4f);
		vel.z = 0.0f;

		XMFLOAT3 acc{};//加速度
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::CiycleParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	for (int i = 0; i < 100; i++)
	{
		const float rnd_pos = 10.0f;
		//{-5.0f~+5.0f}で出現
		XMFLOAT3 pos{};//初期値
		pos = emitterPosition;
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};//初速
		XMVECTOR a;
		a.m128_f32[0] = cosf((float)i * 0.36f * 3.1415f);
		a.m128_f32[1] = sinf((float)i * 0.36f * 3.1415f);
		a.m128_f32[2] = 0;
		XMVector3Normalize(a);
		vel.x = a.m128_f32[0] / 2;
		vel.y = a.m128_f32[1] / 2;
		vel.z = a.m128_f32[2];

		XMFLOAT3 acc{};//加速度
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::ContinueParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	pos = emitterPosition;
	pos.y -= 1;
	const float rnd_acc = 0.001f;
	acc.y = static_cast<float>(rand() % 3 + 1);
	acc.y = acc.y / 80.0f;
	Add(life, pos, vel, acc, startSize, endSize);
}
void ParticleIndi::FrameParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life, int count)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	for (int i = 0; i < count; i++)
	{
		pos.x = emitterPosition.x + ((rand() % 4 - 2.0f) / 10.0f);
		pos.y = emitterPosition.y + ((rand() % 4 - 2.0f) / 10.0f);
		pos.z = emitterPosition.z;
		vel.y = rand() % 3 / 10.0f;
		acc.x = ((rand() % 6) - 3.0f) / 300.0f;
		acc.y = rand() % 2 / 100.0f;
		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::JumpParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	int count = 20;

	for (int i = 0; i < count; i++)
	{
		pos = emitterPosition;
		vel.y = static_cast<float>(rand() % 3 / 40.0f);
		acc.x = static_cast<float>((((rand() % 4) - 2.0f) / 500.0f));
		acc.y = static_cast<float>(rand() % 2);
		acc.y = static_cast<float>(acc.y / (rand() % 10));
		acc.y = static_cast<float>(acc.y / 80.0f);
		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::LuckParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	int count = 20;

	for (int i = 0; i < count; i++)
	{
		pos = emitterPosition;
		vel.z = cosf((int)i * (360.0f / count) * 3.141592f / 180.0f);
		vel.y = sinf((int)i * (360.0f / count) * 3.141592f / 180.0f);
		vel.z = (vel.z * (rand() % 3 + 0.1f)) / 10.0f;
		vel.y = (vel.y * (rand() % 3 + 0.1f)) / 10.0f;

		acc.z = -vel.z / 50.0f;
		acc.y = -vel.y / 50.0f;
		
		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::DownParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	int count = 20;

	for (int i = 0; i < count; i++)
	{
		pos = emitterPosition;
		pos.y = pos.y + 1.0f;
		pos.z += rand() % 11 - 5;
		pos.z += rand() % 9 - 4 / 10.0f;
		vel.y = -(rand() % 3 + 0.4f) / 10.0f;

		/*acc.z = -vel.z / 50.0f;
		acc.y = -vel.y / 50.0f;*/

		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::UI(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	pos = emitterPosition;
	Add(life, pos, vel, acc, startSize, endSize);
}
void ParticleIndi::UI2(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	pos = emitterPosition;
	pos.y += 4.0f;
	Add(1, pos, vel, acc, startSize, endSize);
}
void ParticleIndi::UpParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	float y,z;
	int count = 20;

	for (int i = 0; i < count; i++)
	{
		pos = emitterPosition;
		
		z = cosf(i * (360.0f / count) * 3.141592f / 180.0f);
		y = sinf(i * (360.0f / count) * 3.141592f / 180.0f);
		
		pos.z += z * life;
		pos.y += y * life;
		vel.z = -z;
		vel.y = -y;
		/*acc.z = -z / 60.0f;
		acc.y = -y / 60.0f;*/

		Add(life, pos, vel, acc, startSize, endSize);

	}
	
	
}
void ParticleIndi::RainParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life, int count)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	for (int i = 0;i < count;i++)
	{
		pos = emitterPosition;
		pos.z = static_cast<float>(rand() % 100 - 50);
		vel.y = rand() % 3 + 0.2f;
		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::BaffParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life, int count)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};
	float y, z;
	for (int i = 0; i < count; i++)
	{
		pos = emitterPosition;
		z = cosf(i * (360.0f / count) * 3.141592f / 180.0f);
		y = sinf(i * (360.0f / count) * 3.141592f / 180.0f);

		pos.z += z * 4.0f;

		vel.y = rand() % 3 / 10.0f;
		Add(life, pos, vel, acc, startSize, endSize);
	}
}
void ParticleIndi::RockOn(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	pos = emitterPosition;
	pos.x -= 4.0f;
	Add(1, pos, vel, acc, startSize, endSize);
}
void ParticleIndi::StarParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life, int count)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	for (int i = 0; i < count; i++)
	{
		int randam = rand();
		pos = emitterPosition;
		vel.z = static_cast<float>(randam % 3 - 1 + (randam % 10 / 10) + (randam % 10 / 100));
		vel.y = static_cast<float>(randam % 3 + (randam % 10 / 10) + (randam % 10 / 100));
		acc.y = -static_cast<float>((randam % 10 / 10));
	}
	Add(life, pos, vel, acc, startSize, endSize);
}
void ParticleIndi::BackParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	float randam = static_cast<float>(rand());
	pos = emitterPosition;
	pos.z += 200;
	pos.x += 10;
	pos.y += (int)randam % 200 - 100 + (int)randam % 10 / 10;
	vel.z -= 1.0f;

	
	Add(life, pos, vel, acc, startSize, endSize);
}

void ParticleIndi::FlashParticle(const DirectX::XMFLOAT3 emitterPosition, float startSize, float endSize, int life)
{
	XMFLOAT3 pos{};
	XMFLOAT3 vel{};
	XMFLOAT3 acc{};

	float randam = static_cast<float>(rand());
	pos = emitterPosition;
	vel.x = 1.0f;
	acc.x = -0.01f;
	Add(life, pos, vel, acc, startSize, endSize);
	vel.x = -1.0f;
	acc.x = 0.01f;
	Add(life, pos, vel, acc, startSize, endSize);
	vel.x = 0.0f;
	acc.x = 0.0f;
	vel.y = -1.0f;
	acc.y = 0.01f;
	Add(life, pos, vel, acc, startSize, endSize);
	vel.y = 1.0f;
	acc.y = -0.01f;
	Add(life, pos, vel, acc, startSize, endSize);
}

ParticleControl::ParticleControl()
{
}

ParticleControl::~ParticleControl()
{
	
}

void ParticleControl::Update()
{
	
	for (int i = 0; i < 10; i++)
	{
		numbers[i]->Update(Cameras::camera.eye.v, Cameras::camera.target.v, Cameras::camera.up.v);
	}
}

void ParticleControl::Init()
{
	// 3Dパーティクル静的初期化
	if (!ParticleManager::StaticInitialize(BaseDirectX::dev.Get(), window_width, window_height, Cameras::camera.eye.v, Cameras::camera.target.v, Cameras::camera.up.v))
	{
		assert(0);
	}
	
	numbers[0].reset(numbers[0]->Create(L"Resource/Img/number_0.png"));
	numbers[1].reset(numbers[0]->Create(L"Resource/Img/number_1.png"));
	numbers[2].reset(numbers[0]->Create(L"Resource/Img/number_2.png"));
	numbers[3].reset(numbers[0]->Create(L"Resource/Img/number_3.png"));
	numbers[4].reset(numbers[0]->Create(L"Resource/Img/number_4.png"));
	numbers[5].reset(numbers[0]->Create(L"Resource/Img/number_5.png"));
	numbers[6].reset(numbers[0]->Create(L"Resource/Img/number_6.png"));
	numbers[7].reset(numbers[0]->Create(L"Resource/Img/number_7.png"));
	numbers[8].reset(numbers[0]->Create(L"Resource/Img/number_8.png"));
	numbers[9].reset(numbers[0]->Create(L"Resource/Img/number_9.png"));
	for (int i = 0; i < 10; i++)
	{
		numbers[i]->alpha = 0.0f;
	}
}

void ParticleControl::Draw()
{
	for (int i = 0; i < 10; i++)
	{
		ParticleDraw(BaseDirectX::cmdList.Get(), numbers[i].get());
	}
}
