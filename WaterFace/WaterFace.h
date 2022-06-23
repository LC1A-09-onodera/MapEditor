#pragma once
#include "../3DModel/Model.h"
#include "../PostEffect/PostEffect.h"

class WaterConstBuff0 : public ConstBufferDataB0
{
public:
	UINT frameTime;
};

class WaterEachInfo : public EachInfo
{
public:
	UINT frame;
	void CreateConstBuff0();
	void CreateConstBuff1();
	void CreateConstBuff2();
	void ConstInit();
};

class WaterFaceModel : public Model
{
public:
	WaterEachInfo each;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	UINT frameTime = 0;
	//EachInfo each;
	void CreateModel(const char* name, HLSLShader& shader,PostEffect &postEffect, bool smoothing = false);
	bool LoadTexture(const string& directPath, const string& filename, PostEffect &postEffect);
	void LoadMaterial(const string& directoryPath, const string& filename, PostEffect& postEffect);
	void InitializeGraphicsPipeline(HLSLShader& shader, PostEffect &postEffect);
	void Update();
	void PreDraw();
	void PostDraw();
	void Draw(WaterEachInfo&each, PostEffect& postEffect);
};



class WaterFace
{
public:
	WaterFaceModel waterModel;

	void LoadModel(HLSLShader &useShader, PostEffect &postEffect);
	void Init();
	void Update();
	void Draw(PostEffect& postEffect, XMVECTOR &selingPos);
};
