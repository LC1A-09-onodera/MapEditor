#include"ShaderManager.h"

HLSLShader ShaderManager::playerShader;
HLSLShader ShaderManager::waterShader;
HLSLShader ShaderManager::normalPlaneShader;
HLSLShader ShaderManager::postWater;
HLSLShader ShaderManager::postMosaic;
HLSLShader ShaderManager::postBlur;
HLSLShader ShaderManager::postNormal;
HLSLShader ShaderManager::shieldShader;
void ShaderManager::LoadShaders()
{
	playerShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	playerShader.psBlob = LoadShader(L"Resource/HLSL/OBJPixelShader.hlsl", "ps_5_0");
	waterShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	waterShader.psBlob = LoadShader(L"Resource/HLSL/WaterSamplePS.hlsl", "ps_5_0");
	normalPlaneShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	normalPlaneShader.psBlob = LoadShader(L"Resource/HLSL/NormalPlane.hlsl", "ps_5_0");
	shieldShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	shieldShader.psBlob = LoadShader(L"Resource/HLSL/SamplePS.hlsl", "ps_5_0");

	postWater.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postWater.psBlob = LoadShader(L"Resource/HLSL/PostWaterPS.hlsl", "ps_5_0");
	postMosaic.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postMosaic.psBlob = LoadShader(L"Resource/HLSL/PostMosaicPS.hlsl", "ps_5_0");
	postBlur.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postBlur.psBlob = LoadShader(L"Resource/HLSL/PostBlurPS.hlsl", "ps_5_0");
	postNormal.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postNormal.psBlob = LoadShader(L"Resource/HLSL/PostEffectTestPS.hlsl", "ps_5_0");
}
