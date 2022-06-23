#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    return tex0.Sample(smp, input.uv);
	//マルチテクスチャ用
	//float4 texcolor0 = tex0.Sample(smp, input.uv);
	//float4 texcolor1 = tex1.Sample(smp, input.uv);
	//float4 color = texcolor0;
	//if (fmod(input.uv.y, 0.1f) < 0.05f)
	//{
	//	color = texcolor1;
	//}
	//return float4(texcolor0.rgb, 1);

	//平行ブラー
	/*float4 color = tex0.Sample(smp, input.uv);
	float offsetU = 1.5f / 1280.0f;
	float offsetV = 1.5f / 720.0f;
	color += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));
	color += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));
	color += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));
	color += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));
	color += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));
	color += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));
	color += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));
	color += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));
	color /= 9.0f;
	return color;*/

	/*float4 color;
	color  = weight0.x * tex0.Sample(smp, input.uv + float2(1.0f / 1280.0f, 0.0f));
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
	return color;*/
	
	//水面ぽいの
	/*float2 windowSize;
	windowSize.x = 1280;
	windowSize.y = 720;
	float speed = 1.0f;
	float ripple = 1.0f;
	float2 uv = input.uv;
	uint frameCount = frameTime;
	float time = frameCount * 0.01;
	float2 pos = input.svpos.xy / windowSize * 12.0 - 20.0;
	float2 tmp = pos;
	float speed2 = speed * 2.0;
	float inten = 0.015f;
	float col = 0;
	float distortion = 0.2f;
	for (int i = 0; i < Iterations; ++i)
	{
		float t = time * (1.0 - (3.2 / (float(i) + speed)));
		tmp = pos + float2(
		cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
		sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple));
		tmp += time;
		col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
	}
	col /= float(Iterations);
	col = saturate(1.5 - sqrt(col));
	uv += col * distortion;
	return tex0.Sample(smp, uv) + col * 0.1f;*/

	//モザイク
	/*float2 windowSize;
	windowSize.x = 1280;
	windowSize.y = 720;
	int blockSize = 10;
	float2 uv = input.uv * windowSize;
	uv /= blockSize;
	uv = floor(uv) * blockSize;
	uv /= windowSize;
	return tex0.Sample(smp, uv);*/
}