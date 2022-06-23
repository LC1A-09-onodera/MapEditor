#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);
//Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
	//ÉÇÉUÉCÉN
	float2 windowSize;
	windowSize.x = 1280;
	windowSize.y = 720;
	int blockSize = 10;
	float2 uv = input.uv * windowSize;
	uv /= blockSize;
	uv = floor(uv) * blockSize;
	uv /= windowSize;
	return tex0.Sample(smp, uv);
}