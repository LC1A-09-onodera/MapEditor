#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);
//Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
	float4 color;
	color = weight0.x * tex0.Sample(smp, input.uv + float2(1.0f / 1280.0f, 0.0f));
	color += weight0.y * tex0.Sample(smp, input.uv + float2(2.0f / 1280.0f, 0.0f));
	color += weight0.z * tex0.Sample(smp, input.uv + float2(3.0f / 1280.0f, 0.0f));
	color += weight0.x * tex0.Sample(smp, input.uv + float2(-1.0f / 1280.0f, 0.0f));
	color += weight0.y * tex0.Sample(smp, input.uv + float2(-2.0f / 1280.0f, 0.0f));
	color += weight0.z * tex0.Sample(smp, input.uv + float2(-3.0f / 1280.0f, 0.0f));
	color += weight0.x * tex0.Sample(smp, input.uv + float2(0.0f, 1.0f / 720.0f));
	color += weight0.y * tex0.Sample(smp, input.uv + float2(0.0f, 2.0f / 720.0f));
	color += weight0.z * tex0.Sample(smp, input.uv + float2(0.0f, 3.0f / 720.0f));
	color += weight0.x * tex0.Sample(smp, input.uv + float2(0.0f, -1.0f / 720.0f));
	color += weight0.y * tex0.Sample(smp, input.uv + float2(0.0f, -2.0f / 720.0f));
	color += weight0.z * tex0.Sample(smp, input.uv + float2(0.0f, -3.0f / 720.0f));
	return color;
}