#include "Model.h"
#include "../Camera/Camera.h"
#include "../Collition/BaseCollision.h"
#include "../Collition/CollisionManager.h"

using namespace std;
Light* Model::light = nullptr;
Model::~Model()
{
	if (collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}
inline size_t Model::GetVertexCount()
{
	return mesh.vertices.size();
}
void Model::AddAmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		//共通頂点コレクション
		vector<unsigned short>& v = itr->second;
		//全頂点の法線のコレクション
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(mesh.vertices[index].normal.x, mesh.vertices[index].normal.y, mesh.vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v)
		{
			mesh.vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Model::SetLight(Light* light)
{
	Model::light = light;
}

void Model::Init(int index)
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh.indices.size());
	&CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	&CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	mesh.vbView.SizeInBytes = sizeVB;
	mesh.ibView.SizeInBytes = sizeIB;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeVB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.vertBuff));
	Vertex* vertMap = nullptr;
	BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(BaseDirectX::result))
	{
		copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
		mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
	}
	//インデックスバッファの生成
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeIB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.indexBuff));
	unsigned short* indexMap = nullptr;
	BaseDirectX::result = mesh.indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(BaseDirectX::result))
	{
		copy(mesh.indices.begin(), mesh.indices.end(), indexMap);
		mesh.indexBuff->Unmap(0, nullptr);
	}

	// GPU上のバッファに対応した仮想メモリを取得
	mesh.vbView.BufferLocation = mesh.vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeVB;
	mesh.vbView.StrideInBytes = sizeof(Vertex);
	mesh.ibView.BufferLocation = mesh.indexBuff->GetGPUVirtualAddress();
	mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;
	each.ConstInit();
	UINT descHadleIncSize = BaseDirectX::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuDescHandleCBV = BaseDirectX::basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	cpuDescHandleCBV.ptr += index * descHadleIncSize;

	gpuDescHandleCBV = BaseDirectX::basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	gpuDescHandleCBV.ptr += index * descHadleIncSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = each.constBuff0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)each.constBuff0->GetDesc().Width;
	BaseDirectX::dev->CreateConstantBufferView(&cbvDesc, cpuDescHandleCBV);
	//name = typeid(*this).name();
}

void Model::CreateModel(const char* name, HLSLShader& shader, bool smoothing, bool triangle)
{
	InitializeDescriptorHeap();
	InitializeGraphicsPipeline(shader, triangle);
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
				/*if (count > 3) {
					const uint16_t index1 = mesh.vertices.size() - 4;
					const uint16_t index2 = mesh.vertices.size() - 2;

					mesh.indices.emplace_back(index1);
					mesh.indices.emplace_back(index2);
				}*/
			}
			//count = 0;
		}
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	};
	Init(1);
	file.close();
}

void Model::Update(EachInfo* each, bool rCamera)
{
	if (each != nullptr)
	{
		this->each = *each;
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Cameras::camera.eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = cameraPos;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye.v;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
		if (collider)
		{
			collider->Update();
		}
	}
	else
	{
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Cameras::camera.eye.v;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = cameraPos;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye.v;
			}
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
		if (collider)
		{
			collider->Update();
		}
	}
}

bool Model::InitializeGraphicsPipeline(HLSLShader& shader, bool triangle)
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
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

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
	if (triangle)
	{
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
	else 
	{
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	}
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

bool Model::LoadTexture(const string& directPath, const string& filename)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	string filepath = directPath + filename;

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))
	{
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = BaseDirectX::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return result;
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
		return result;
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

	//BaseDirectX::dev->CreateShaderResourceView(texbuff.Get(), &srvDesc, cpuDescHandleSRV[materialCount]);
	BaseDirectX::dev->CreateShaderResourceView(texbuff.Get(), &srvDesc, cpuDescHandleSRV);
	return true;
}

bool Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = BaseDirectX::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void Model::LoadMaterial(const string& directoryPath, const string& filename)
{
	ifstream file;
	file.open(directoryPath + filename);
	if (file.fail())
	{
		assert(0);
	}

	string line;
	while (getline(file, line))
	{
		istringstream line_striam(line);
		string key;
		getline(line_striam, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_striam >> material.name;
		}

		if (key == "Ka")
		{
			line_striam >> material.ambient.x;
			line_striam >> material.ambient.y;
			line_striam >> material.ambient.z;
		}

		if (key == "Kd")
		{
			line_striam >> material.diffuse.x;
			line_striam >> material.diffuse.y;
			line_striam >> material.diffuse.z;
		}

		if (key == "Ks")
		{
			line_striam >> material.specular.x;
			line_striam >> material.specular.y;
			line_striam >> material.specular.z;
		}

		if (key == "map_Kd")
		{
			line_striam >> material.texFilename;
			LoadTexture(directoryPath, material.texFilename);
		}
	}
	materialCount += 1;
	file.close();
}

void Set3DDraw(const Model& model, bool triangle)
{
	BaseDirectX::cmdList->IASetIndexBuffer(&model.mesh.ibView);
	BaseDirectX::cmdList->SetPipelineState(model.pipelinestate.Get());
	BaseDirectX::cmdList->SetGraphicsRootSignature(model.rootsignature.Get());
	//三角形リスト
	if (triangle == true)
	{
		BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	}

	else if (triangle == false)
	{
		BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}

void Draw3DObject(const Model& model, bool triangle)
{
	Set3DDraw(model, triangle);
	BaseDirectX::cmdList->IASetVertexBuffers(0, 1, &model.mesh.vbView);
	BaseDirectX::cmdList->IASetIndexBuffer(&model.mesh.ibView);
	ID3D12DescriptorHeap* ppHeap[] = { model.descHeap.Get() };
	BaseDirectX::cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
	//BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(0, model.gpuDescHandleCBV);
	BaseDirectX::cmdList->SetGraphicsRootConstantBufferView(0, model.each.constBuff0->GetGPUVirtualAddress());
	Model::light->Draw(BaseDirectX::cmdList.Get(), 3);
	BaseDirectX::cmdList->SetGraphicsRootConstantBufferView(1, model.each.constBuff1->GetGPUVirtualAddress());
	BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(2, model.gpuDescHandleSRV);
	BaseDirectX::cmdList->DrawIndexedInstanced((UINT)model.mesh.indices.size(), 1, 0, 0, 0);
}

bool ObjectColition(Model& object1, Model& object2)
{
	XMFLOAT3 pos1 = { object1.matWorld.r[3].m128_f32[0], object1.matWorld.r[3].m128_f32[1], object1.matWorld.r[3].m128_f32[2] };
	XMFLOAT3 pos2 = { object2.matWorld.r[3].m128_f32[0], object2.matWorld.r[3].m128_f32[1], object2.matWorld.r[3].m128_f32[2] };
	//座標の差
	XMVECTOR position_sub = XMVectorSet(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z, 0);
	position_sub = XMVector3Length(position_sub);
	float distance = position_sub.m128_f32[0];

	if (distance <= object1.radi + object2.radi)
	{
		return true;
	}

	return false;
}

bool CiycleColition(const XMFLOAT3& object1, const XMFLOAT3& object2, float radi1, float radi2)
{
	//座標の差
	XMFLOAT3 position_sub = { object1.x - object2.x, object1.y - object2.y, object1.z - object2.z };
	float distance = sqrtf(position_sub.x * position_sub.x + position_sub.y * position_sub.y + position_sub.z * position_sub.z);;

	if (distance <= radi1 + radi2)
	{
		return true;
	}

	return false;
}
void Model::SetCollider(BaseCollider* collider)
{
	collider->SetModel(this);
	this->collider = collider;
	CollisionManager::GetInstance()->AddCollider(collider);
	collider->Update();
}

void EachInfo::CreateConstBuff0()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
}

void EachInfo::CreateConstBuff1()
{
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff1));
}

void EachInfo::CreateConstBuff2()
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataB3) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff2));
}

void EachInfo::ConstInit()
{
	CreateConstBuff0();
	CreateConstBuff1();
	CreateConstBuff2();
}
