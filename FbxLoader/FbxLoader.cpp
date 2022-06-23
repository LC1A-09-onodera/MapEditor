#include "FbxLoader.h"
#include <cassert>

const std::string FbxLoader::baseDirectory = "Resource/FBX/";
using namespace DirectX;
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* dev)
{
	assert(fbxManager == nullptr);
	this->dev = dev;
	fbxManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FBXModel* FbxLoader::LoadModelFromFile(const string& modelName)
{
	const string directoryPath = baseDirectory + modelName + "/";
	const string fileName = modelName + ".fbx";
	const string fullpath = directoryPath + fileName;
	if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}
	FbxScene *fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);
	//モデル作成
	FBXModel* model = new FBXModel();
	model->name = modelName;
	//ノード数の獲得
	int nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	ParseNodeRecursive(model, fbxScene->GetRootNode());

	model->CreateBuffers(dev);

	model->fbxScene = fbxScene;
	return model;
}

void FbxLoader::ParseNodeRecursive(FBXModel* model, FbxNode* fbxNode, Node* parent)
{
	//ノード追加
	model->nodes.emplace_back();
	Node& node = model->nodes.back();
	//ノード名の取得
	node.name = fbxNode->GetName();
	//ローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();
	node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
	node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
	node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };

	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;
	if (parent)
	{
		node.parent = parent;
		node.globalTransform *= parent->globalTransform;
	}

	FbxNodeAttribute* fbxnodeAtribute = fbxNode->GetNodeAttribute();
	if (fbxnodeAtribute)
	{
		if (fbxnodeAtribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			model->meshNode.reset(&node);
			ParsMesh(model, fbxNode);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, fbxNode->GetChild(i));
	}

}

void FbxLoader::ParsMesh(FBXModel* model, FbxNode* fbxNode)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	ParseMeshVertices(model, fbxMesh);
	ParseMeshFaces(model, fbxMesh);
	ParseMaterial(model, fbxNode);
	ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(FBXModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;

	const int controlPosintsCount = fbxMesh->GetControlPointsCount();
	FBXModel::VertexPosNormalUvSkin vert{};
	model->vertices.resize(controlPosintsCount, vert);
	FbxVector4* pCoord = fbxMesh->GetControlPoints();
	for (int i = 0; i < controlPosintsCount; i++)
	{
		FBXModel::VertexPosNormalUvSkin& vertex = vertices[i];
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

void FbxLoader::ParseMeshFaces(FBXModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->indices;

	assert(indices.size() == 0);
	//面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	const int textureUVCount = fbxMesh->GetTextureUVCount();
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	for (int i = 0; i < polygonCount; i++)
	{
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);
		for (int j = 0; j < polygonSize; j++)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			FBXModel::VertexPosNormalUvSkin& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}
			if (j < 3)
			{
				indices.push_back(index);
			}
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FbxLoader::ParseMaterial(FBXModel* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
		bool textureLoaded = false;
		if (material)
		{
			FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);
			//環境光
			FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
			model->ambient.x = (float)ambient.Get()[0];
			model->ambient.y = (float)ambient.Get()[1];
			model->ambient.z = (float)ambient.Get()[2];
			//拡散反射光
			FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
			model->diffuse.x = (float)diffuse.Get()[0];
			model->diffuse.y = (float)diffuse.Get()[1];
			model->diffuse.z = (float)diffuse.Get()[2];

			//ディフューズテクスチャ
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char* filepath = texture->GetFileName();
					string path_str(filepath);
					string name = ExtractFileName(path_str);
					LoadTexture(model, baseDirectory + model->name + "/" + name);
					textureLoaded = true;
				}
			}
		}
		if (!textureLoaded)
		{
			LoadTexture(model, baseDirectory + defaultTextureFileName);
		}
	}
}

void FbxLoader::LoadTexture(FBXModel* model, const std::string& fullPath)
{
	HRESULT result = S_FALSE;

	TexMetadata& metadata = model->metadata;
	ScratchImage& scrachImg = model->scratchImg;

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scrachImg);
	if (FAILED(result))
	{
		assert(0);
	}
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - 1);
	}
	return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}
}

void FbxLoader::ParseSkin(FBXModel* model, FbxMesh* fbxMesh)
{
	FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
	if (fbxSkin == nullptr)
	{
		for (int i = 0; i < model->vertices.size(); i++)
		{
			model->vertices[i].boneIndex[0] = 0;
			model->vertices[i].boneWeight[0] = 1.0f;
		}
		return;
	}

	std::vector<Bone>& bones = model->bones;
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	for (int i = 0; i < clusterCount; i++)
	{
		//ボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
		//ノードの名前の取得
		const char* boneName = fbxCluster->GetLink()->GetName();
		//新しくボーンの追加、参照を得る
		bones.emplace_back(Bone(boneName));
		Bone& bone = bones.back();

		bone.fbxCuster = fbxCluster;
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);
		XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);
		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
	}

	struct WeightSet
	{
		UINT index;
		float weight;
	};

	std::vector<std::list<WeightSet>> weightLists(model->vertices.size());

	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
		int controlPositIndicesCount = fbxCluster->GetControlPointIndicesCount();
		int* controlPointIndices = fbxCluster->GetControlPointIndices();

		double* controlPointWeight = fbxCluster->GetControlPointWeights();

		for (int j = 0; j < controlPositIndicesCount; j++)
		{
			int vertIndex = controlPointIndices[j];
			float weight = (float)controlPointWeight[j];
			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
		}
	}

	auto& vertices = model->vertices;
	for (int i = 0; i < vertices.size(); i++)
	{
		auto& weightList = weightLists[i];
		weightList.sort([](auto const& lhs, auto const& rhs)
			{
				return lhs.weight > rhs.weight;
			});
		int weightArrayIndex = 0;
		for (auto& WeightSet : weightList)
		{
			vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
			if (++weightArrayIndex >= FBXModel::MAX_BONE_INDICES)
			{
				float weight = 0.0f;
				for (int j = 1; j < FBXModel::MAX_BONE_INDICES; j++)
				{
					weight += vertices[i].boneWeight[j];
				}
				vertices[i].boneWeight[0] = 1.0f - weight;
				break;
			}
		}
	}
}
